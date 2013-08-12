#include "AVR-LCD-lib.h"
#include <stdlib.h>  

int main(void) {	
	
	unsigned char *string = "squirrel man";

	lcd_out(string);
	
	 return(0);
}
	
