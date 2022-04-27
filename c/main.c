#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "gpiolib.h"
#include "cputemp.h"

int main(int argc, char **argv)  {
	int temp;
	int gpiopin = 70;

	gpio_export(gpiopin);
	gpio_direction(gpiopin,1);
	while(1) {
		temp = cpu_temp_read();
		printf("Current cpu temp is %d °C\n", temp/1000);
	
	    if ( temp > 70000 )
			gpio_write(gpiopin,1);
		else if (temp < 45000)
			gpio_write(gpiopin,0);
		
        sleep(1);
    }
    
    return 0;
}

