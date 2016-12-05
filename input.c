#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void cleanKeyBuffer() {

    int c=0;
	while (c != '\n' && c != EOF){
		c = getchar();
	}
	
}

int getString(char *string, int len) {

	char *lfPosition = NULL;

	if (fgets(string, len, stdin) != NULL) {
	
		lfPosition = strchr(string, '\n');
		
		if (lfPosition != NULL) {
			*lfPosition = '\0';
		} else {
			cleanKeyBuffer();
		}
		
		return 1;
		
	} else {
	
		cleanKeyBuffer();
		return 0;
		
	}

}

int getInteger() {

	char input[10] = {0};
	
	if (getString(input, sizeof(input))) {
	
		return atoi(input);
		
	} else {
	
		return 0;
		
	}

}

long getLong() {

	char input[10] = {0};
	
	if (getString(input, sizeof(input))) {
	
		return strtol(input, NULL, 10);
		
	} else {
	
		return 0;
		
	}

}

