#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/stat.h>
#include "cputemp.h"

int cpu_temp_read(void)
{
	char temp[5] = {0, 0, 0, 0, 0};
	char buf[50];
	int nread, tempfd;
	sprintf(buf, "/sys/devices/virtual/thermal/thermal_zone0/temp");
	tempfd = open(buf, O_RDONLY);
	if(tempfd < 0) {
		fprintf(stderr, "Failed to open cpu temp value\n");
		perror("cpu temp open failed");
	}
 
	do {
		nread = read(tempfd, temp, 5);
	} while (nread == 0);
	if(nread == -1){
		perror("cpu temp Read failed");
		return -1;
	}
	
	close(tempfd);
	return atoi(temp);

}

