#include <stdio.h>
#include <dirent.h>
#include <math.h>

int main(void){
	DIR *d;
	struct dirent *dir;
	float temperature, a;
	FILE *temp_file, *type;
	int n;
	
	d = opendir("/sys/class/thermal/");
    /*if (d){
	    while ((dir = readdir(d)) != NULL){
        	printf("%s\n", dir->d_name);
        }	
    	closedir(d);
    }*/
	// Reading CPU Temperature for the first core of type ACPITZ
	temp_file = fopen("/sys/class/thermal/thermal_zone0/temp","r");
	fscanf(temp_file, "%f", &temperature);
	temperature = temperature / 1000;
	a = roundf(temperature * 100) / 100;
	printf("%f", temperature);
	printf("%f", a);
	
	
	return 0;
}
