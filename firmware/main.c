#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>

//Define functions
//==========================================================
void io_init(void);                         
void lcd_init(void);
void lcd_finish_init(void);
void add_character(unsigned char position, unsigned char * letter);
void send_nibble(unsigned char __rs, unsigned char __data);

//==========================================================

int main(void) {	
	
    io_init();
	
	lcd_init();
	
	/*// Smiley face. My own creation.
	unsigned char smileyFace[16] = {0b0001, 0b1011, 0b0000, 0b0000, 
		0b0000, 0b0100, 0b0000, 0b0100,
		0b0001, 0b0001, 0b0001, 0b0001,
		0b0001, 0b0001, 0b0001, 0b1111};
	
	add_character(0b00000000, &smileyFace[0]);*/
	
	lcd_finish_init();
	
	unsigned char letter = 0b00110000; // Start at 0
	while(1) {
		if(!bit_is_clear(PIND,5)) { // Go to next letter
			if(letter == 0b01111010) {
				letter = 0b00110000;
			}
			
			unsigned char temp = letter >> 4;
			send_nibble(1,temp);
			
			temp = letter << 4;
			temp = temp >> 4;
			
			send_nibble(1,temp);
			_delay_ms(150);
			letter++;
			
			// Keep on same square
			send_nibble(0,0b0001);
			send_nibble(0,0b0000);
		}
		
		if(!bit_is_clear(PIND,6)) { // Space
			_delay_ms(150);

			send_nibble(0,0b0001);
			send_nibble(0,0b0100);
			letter = 0b00110000;
			
			_delay_ms(150);
		}
	}
    return(0);
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