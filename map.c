#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "map.h"

int make_mapfile(char filename[], char name[], int sizeW, int sizeH) {

	int totalSize = sizeW*sizeH;

	if(totalSize<4) {
		return 1;
	}
	
	FILE *mapFile = fopen(filename, "wb");
	if(mapFile == NULL) {
		
		return 2;
	}
		
	MAP m;
	m.magic[0] = 'm';
	m.magic[1] = 'f';
	m.version = 1;
	strncpy(m.name, name, 255);
	m.name[255] = '\0';
	m.sizeW = sizeW;
	m.sizeH = sizeH;
	m.hero = sizeH * sizeW / 2 + sizeW / 2 - 1;
	
	// Fill with empty chars
	for(int i=0; i<totalSize; i++) {
		m.data[i] = '0';
	}
	
	fwrite(&m.magic, sizeof(char), 2, mapFile);
	fwrite(&m.version, sizeof(int), 1, mapFile);
	fwrite(&m.name, sizeof(char), 255, mapFile);
	fwrite(&m.sizeW, sizeof(int), 1, mapFile);
	fwrite(&m.sizeH, sizeof(int), 1, mapFile);
	fwrite(&m.hero, sizeof(int), 1, mapFile);
	fwrite(m.data, sizeof(char), totalSize, mapFile);

	fclose(mapFile);

	return 0;
	
}
