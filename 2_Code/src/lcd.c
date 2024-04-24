/*
 *Date: 03/18/24
 *Name: Hamza Rahmoune
 *Student ID number: 5675996
 *Course number: EE2361
 *Term: Spring 2024
 *Lab/assignment number: Final Project
 *Short Program Description: Implements ST7032 LCD Display functionality
 */

#include <p24FJ64GA002.h>

#include "xc.h"
#include "stdint.h"
#include "gps.h"
#include <math.h> 

double destLatitude;
double destLongitude;

double currLatitude;
double currLongitude;

double distanceToDestination;

void delay_ms(unsigned int ms){
    //Delay in milliseconds
    while(ms-- > 0){
        asm("repeat #15998");
        asm("nop");
    }
}

void lcd_cmd(char command){
    //Send commands to LCD
    I2C2CONbits.SEN = 1; //start
    while(I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01111100; //slave address
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b00000000; //control byte
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = command; //data byte 
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1; //end
    while(I2C2CONbits.PEN == 1);
    IFS3bits.MI2C2IF = 0;
}

void lcd_init(void){
    //Initializes the LCD
    I2C2CONbits.I2CEN = 0;
    I2C2BRG = 157;
    I2C2CONbits.I2CEN = 1;
    IFS3bits.MI2C2IF = 0;
    
    delay_ms(40);
    
    lcd_cmd(0b00111000); // function set
    lcd_cmd(0b00111001); // function set, advance instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(0b01110000); // contrast Low
    lcd_cmd(0b01010110);
    lcd_cmd(0b01101100); // follower control
    
    delay_ms(200);
    
    lcd_cmd(0b00111000); // function set
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    
    delay_ms(1);
}

void lcd_setCursor(char x, char y){
    //Sets the cursor on the LCD for typing on the display
    lcd_cmd((0x80)| (0x40 * (y) + (x)));
}

void lcd_printChar(char myChar){
    //Prints one character onto the screen
    I2C2CONbits.SEN = 1; //start
    while(I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01111100; //slave address
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01000000; //control byte
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = myChar; //data byte 
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1; //end
    while(I2C2CONbits.PEN == 1);
    IFS3bits.MI2C2IF = 0;
}


void lcd_printStr(const char s[]){
    //Prints variable length string onto the screen
    I2C2CONbits.SEN = 1; //start
    while(I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01111100; //slave address
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    int i = 0;

    while(s[i+1] != '\0'){
        I2C2TRN = 0b11000000; //control byte
        while(IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;

        I2C2TRN = s[i]; //data byte 
        while(IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
        
        ++i;
    }
    
    I2C2TRN = 0b01000000; //control byte
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = s[i]; //data byte 
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1; //end
    while(I2C2CONbits.PEN == 1);
    IFS3bits.MI2C2IF = 0;
}

void setTargetDestination(double latitude, double longitude){
    destLatitude = latitude;
    destLongitude = longitude;
}

void getCurrentLocation(){
    currLatitude = getLatitude();
    currLongitude = getLongitude();
}

double dToR(double degrees) { 
    return degrees / 180.0 * M_PI; 
}

void distanceFinder(){
    //Calculates distance to destination *HERE*
    //Haversine Formula: 3440.1 * arccos((sin(lat A) * sin(lat B)) + cos(lat A) * cos(lat B) * cos(long A - long B))
    getCurrentLocation();
    double dinNM = 3440.1 * acos((sin(dToR(currLatitude)) * sin(dToR(destLatitude))) + cos(dToR(currLatitude)) 
            * cos(dToR(destLatitude)) * cos(dToR(currLongitude) - dToR(destLongitude)));
    
    distanceToDestination = 1852 * dinNM; //In meters (conversion)
}

double getDistanceToDestination(){
    distanceFinder();
    return distanceToDestination;
}

