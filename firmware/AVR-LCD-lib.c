#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "AVR-LCD-lib.h"

//Define functions
//==========================================================
int lcd_out(unsigned char *string);
void io_init(void);                         
void lcd_init(void);
void lcd_finish_init(void);
void add_character(unsigned char position, unsigned char * letter);
void send_nibble(unsigned char __rs, unsigned char __data);
void send_string(unsigned char *string);
void send_character(unsigned char position, unsigned char letter);

//==========================================================

/*//  Wrapper function to initialize the LCD display and pass the string 
////
////
*/
int lcd_out(unsigned char *string){	
	
	// initialize the LCD screen - this should be generic for most LCD's using ##??## display type
    io_init();	
	lcd_init();	
	lcd_finish_init();
	
	send_string(string);
	
	return(1);
}
	
	
/*// Takes a string as input and loops through the char[] calling send character for each char
//// could use additional checks around the max size of the array, given a specific LCD
//// 
*/
void send_string(unsigned char* string){
	short numChars = strlen(string);
	short i;
	short pos;
	for(i = 0; i < numChars; i++) {
			pos=i+1;
			send_character(pos, string[i]);
	}
}

//  Sends a single character to the LCD at a given position
//  could use checks to be sure not to breach the max size of the LCD
//
//  **NOTES: position currently unused - this may be useful to add in for more complex writes
//
void send_character(unsigned char position, unsigned char letter){
	unsigned char halfChar = letter >> 4;
	send_nibble(1,halfChar);
	
	halfChar = letter << 4;
	halfChar = halfChar >> 4;
	send_nibble(1,halfChar);
}


void io_init (void) {
    /*
	 PC 7: N/A
	 PC 6: Reset
	 PC 5: Enable
	 PC 4: Register Select
	 PC 3: Data 7
	 PC 2: Data 6
	 PC 1: Data 5
	 PC 0: Data 4  
	 */
	
	DDRD = 0b00000000; // All output
    DDRC = 0b00111111;
}

void lcd_init(void) {
	_delay_ms(15);
	send_nibble(0,0b0010);   //Set to 4 bit operation (note: 1 nibble operation)
	_delay_ms(5);
	
	send_nibble(0,0b0010);   //Function set, 4 bit
	send_nibble(0,0b1000);
}


void lcd_finish_init(void) {
	send_nibble(0,0b0000);   //Display ON, Cursor Off, Cursor Not Blinking
	send_nibble(0,0b1110);
	
	send_nibble(0,0b0000);   //Clear Display
	send_nibble(0,0b0001);
	
	send_nibble(0,0b0000);  //Entry Mode, Increment cursor position, No display shift
	send_nibble(0,0b0110);
}

void send_nibble(unsigned char __rs, unsigned char __data) {
	PORTC = (__rs<<4) | __data | 0b00100000;      // Set RS & Data. Set EN=High
	_delay_ms(1);
	PORTC = (__rs<<4) | __data;                   // Set RS & Data. Set EN=Low
	_delay_ms(1);
}

//
//  Unused - this is for custom characters, would need to extend the library to use it
//
void add_character(unsigned char position, unsigned char * letter) {
	position = position | 0b01000000; // Ensure writing to CGRAM command
	
	unsigned char temp = position >> 4;
	send_nibble(0,temp);
	
	temp = position << 4;
	temp = temp >> 4;
	
	send_nibble(0,temp);
	
	// Write character.
	short i;
	for(i = 0; i < 16; i++) {
		send_nibble(1, letter[i]);
	}
}