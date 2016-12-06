#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "input.h"
#include "map.h"

int main(int argc, char *argv[]) {

	char filename[255];
	int write_mode = 0;
	
	/*
	 * Args parsing
	 */

	if(argc == 2) {
		strcpy(filename, argv[1]);
	} else if(argc == 3 && strncmp(argv[1], "-w", 2) == 0) {
		strcpy(filename, argv[2]);
		write_mode = 1;
	} else {
		printf("Usage : %s {-w} [map]\n", argv[0]);
		return 1;
	}
	
	strcat(filename, ".pmtmf");
	
	/*
	 * 0 - Write mode
	 */
	 
	if(write_mode) {
	
		char mapName[255];
		int w=0, h=0;
		
		printf("Map name : ");
		getString(mapName, 255);
        
        printf("Width : ");
        w = getInteger();
        printf("Height : ");
        h = getInteger();
	
		printf("Writing %s... ", filename);
	
		int errn = make_mapfile(filename, mapName, w, h);
	
		if(errn == 1) {
			printf("\nMap size must be >= 4 !\n");
			return 1;
		} else if(errn == 2) {
			printf("\nUnable to open map file !\n");
			return 1;
		}
		
		printf("Done\n");
		
		return 0;
		
	}
	
	/*
	 * 1 - Normal mode
	 */
	 
	FILE *mapFile = fopen(filename, "rb+");
	
	if(mapFile == NULL) {
		printf("Unable to open %s map file !\n", filename);
		return 1;
	}
	
	MAP m;
	fread(&m.magic, sizeof(char), 2, mapFile);
	fread(&m.version, sizeof(int), 1, mapFile);
	fread(&m.name, sizeof(char), 255, mapFile);
	fread(&m.sizeW, sizeof(int), 1, mapFile);
	fread(&m.sizeH, sizeof(int), 1, mapFile);
	fread(&m.hero, sizeof(int), 1, mapFile);

	if(m.magic[0]!='m' && m.magic[1]!='f') {
		printf("Wrong map file format !\n");
		return 1;
	} else if(m.version!=1) {
		printf("Unsuported map file version !\n");
		return 1;
	}
	
	printf("Loading '%s' %dx%d map...\n", m.name, m.sizeW, m.sizeH);
	
	int totalSize = m.sizeW*m.sizeH;
	fread(&m.data, sizeof(char), totalSize, mapFile);
	
	fclose(mapFile);
	
	
	/*
	 * Display map
	 */
	 
	initscr();
	
	char k=0;
	int nextPosition=m.hero;
	
	while(1) {
	
		unsigned char displayChar=0;
		int width=0;
		for(int i=0; i<totalSize; i++) {
	
			if(i == m.hero) {
		
				printw("&");
			
			} else {
		
				switch(m.data[i]) {
			
					case '0':
						displayChar = '_';
						break;
					
					case '1':
						displayChar = '#';
						break;
			
				}
			
				printw("%c", displayChar);
			
			}
		
			if(width >= m.sizeW-1) {
				printw("\n");
				width=0;
			} else {
				width++;
			}
		
		}
		
		refresh();
		
		k=getch();
		
		switch(k) {
		
			case 'A':
				if(m.hero - m.sizeW >= 0)
					nextPosition = m.hero - m.sizeW;
				break;
			case 'B':
				if(m.hero <= totalSize-1)
					nextPosition = m.hero + m.sizeW;
				break;
			case 'C':
				if(m.hero%m.sizeW < m.sizeW-1)
					nextPosition = m.hero + 1;
				break;
			case 'D':
				if(m.hero%m.sizeW > 0)
					nextPosition = m.hero - 1;
				break;
			case 'q':
				endwin();
				return 0;

		}
		
		if(m.data[nextPosition] != BLOCK_WALL) {
			m.hero = nextPosition;
		} else {
			nextPosition = m.hero;
		}
		
		
		
		clear();
	
	}
	
	
	endwin();

	return 0;
	
}
