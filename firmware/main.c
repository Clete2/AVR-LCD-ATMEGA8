
#include "AVR-LCD-lib.h"

//Define functions
//==========================================================

//==========================================================

int main(void) {	
	
	unsigned char *string = "squirrel man";

	lcd_out(string);
	
	free(string);
	string = 0;
	
	 return(0);
}
	
