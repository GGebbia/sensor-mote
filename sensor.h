#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <stdlib.h>

#define MAX 10

float cpu_temperature(void){
	
	// Declaring all variables
	FILE *fp;
	int status, count = 0;
	char cpu_temp[PATH_MAX];
	float avg_temp;

	// Use popen to get all thermal temperatures from all cores
	fp = popen("cat /sys/class/thermal/thermal_zone*/temp", "r");
	if (fp == NULL)
		printf("Unable to obtain CPU temperature\n");
	
	// Sum temperature when it's not zero.
	while (fgets(cpu_temp, PATH_MAX, fp) != NULL){
		if (atof(cpu_temp) != 0.00){
			avg_temp += atof(cpu_temp);
			count++;
		}
	}

	status = pclose(fp);
	if (status == -1) {
		printf("Error during pipe closing.\n");
		exit(0);
	} 

	// Averaging temperature and conveting from millicelsius degrees to celsius degrees.
	avg_temp = avg_temp / (1000 * count);
	
	return avg_temp;
	
}


