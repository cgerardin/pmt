#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "main.h"
#include "input.h"
#include "map.h"

int main(int argc, char *argv[]) {

	int error=PMT_SUCCESS;

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
		return PMT_ERROR;
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
	
		error = make_mapfile(filename, mapName, w, h);
	
		if(error == PMT_WRONG_MAP_FORMAT) {
			printf("\nMap size must be >= 4 !\n");
			return PMT_ERROR;
		} else if(error == PMT_ERROR_OPEN_MAP) {
			printf("\nUnable to open map file !\n");
			return PMT_ERROR;
		}
		
		printf("Done\n");
		
		return PMT_SUCCESS;
		
	}
	
	/*
	 * 1 - Normal mode
	 */
	
	MAP m;
	error = read_mapfile(filename, &m);
	
	if(error == PMT_WRONG_MAP_FORMAT) {
		printf("Wrong map file format !\n");
		return PMT_ERROR;
	} else if(error == PMT_WRONG_MAP_VERSION) {
		printf("Unsuported map file version !\n");
		return PMT_ERROR;
	}
	
	printf("Running '%s' %dx%d map...\n", m.name, m.sizeW, m.sizeH);
	
	
	/*
	 * Display map
	 */
	 
	initscr();
	
	char key=0;
	int totalSize = m.sizeW*m.sizeH;
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
		
		key=getch();
		
		switch(key) {
		
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
				return PMT_SUCCESS;

		}
		
		if(m.data[nextPosition] != BLOCK_WALL) {
			m.hero = nextPosition;
		} else {
			nextPosition = m.hero;
		}
		
		clear();
	
	}
	
}
