#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "map.h"

int read_mapfile(char filename[], MAP *map) {

	FILE *mapFile = fopen(filename, "rb+");
	
	if(mapFile == NULL) {
		return PMT_ERROR_OPEN_MAP;
	}
	
	fread(&map->magic, sizeof(char), 2, mapFile);
	fread(&map->version, sizeof(int), 1, mapFile);
	fread(&map->name, sizeof(char), 255, mapFile);
	fread(&map->sizeW, sizeof(int), 1, mapFile);
	fread(&map->sizeH, sizeof(int), 1, mapFile);
	fread(&map->hero, sizeof(int), 1, mapFile);

	if(map->magic[0]!='m' && map->magic[1]!='f') {
		return PMT_WRONG_MAP_FORMAT;
	} else if(map->version!=PMT_VERSION) {
		return PMT_WRONG_MAP_VERSION;
	}
	
	fread(&map->data, sizeof(char), map->sizeW*map->sizeH, mapFile);
	
	fclose(mapFile);
	
	return PMT_SUCCESS;

}

int write_mapfile(MAP *map, char filename[]) {

	FILE *mapFile = fopen(filename, "wb");
	
	if(mapFile == NULL) {
		return PMT_ERROR_OPEN_MAP;
	}
	
	fwrite(&map->magic, sizeof(char), 2, mapFile);
	fwrite(&map->version, sizeof(int), 1, mapFile);
	fwrite(&map->name, sizeof(char), 255, mapFile);
	fwrite(&map->sizeW, sizeof(int), 1, mapFile);
	fwrite(&map->sizeH, sizeof(int), 1, mapFile);
	fwrite(&map->hero, sizeof(int), 1, mapFile);
	fwrite(map->data, sizeof(char), map->sizeW*map->sizeH, mapFile);

	fclose(mapFile);
	
	return PMT_SUCCESS;

}

int make_mapfile(char filename[], char name[], int sizeW, int sizeH) {

	int totalSize = sizeW*sizeH;

	if(totalSize<4) {
		return PMT_WRONG_MAP_FORMAT;
	}
	
	FILE *mapFile = fopen(filename, "wb");
	if(mapFile == NULL) {
		return PMT_ERROR_OPEN_MAP;
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
		m.data[i] = PMT_BLOCK_EMPTY;
	}
	
	fwrite(&m.magic, sizeof(char), 2, mapFile);
	fwrite(&m.version, sizeof(int), 1, mapFile);
	fwrite(&m.name, sizeof(char), 255, mapFile);
	fwrite(&m.sizeW, sizeof(int), 1, mapFile);
	fwrite(&m.sizeH, sizeof(int), 1, mapFile);
	fwrite(&m.hero, sizeof(int), 1, mapFile);
	fwrite(m.data, sizeof(char), totalSize, mapFile);

	fclose(mapFile);

	return PMT_SUCCESS;
	
}
