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
	int make_mode = 0;
	int edit_mode = 0;
	
	/*
	 * Args parsing
	 */

	if(argc == 2) {
		strcpy(filename, argv[1]);
	} else if(argc == 3 && strncmp(argv[1], "-m", 2) == 0) {
		strcpy(filename, argv[2]);
		make_mode = 1;
	} else if(argc == 3 && strncmp(argv[1], "-e", 2) == 0) {
		strcpy(filename, argv[2]);
		edit_mode = 1;
	} else {
		printf("Usage :\n\t%s {-m|-e} [map]\n\nOptions :\n", argv[0]);
		printf("\t-w : Make an empty [map].pmtmf file\n");
		printf("\t-e : Open [map].pmtmf in edit mode\n");
		return PMT_ERROR;
	}
	
	strcat(filename, ".pmtmf");

	if(make_mode) {
	
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

	initscr();
	
	char key=0;
	int totalSize = m.sizeW*m.sizeH;
	int nextPosition=m.hero;
	
	while(1) {
	
		unsigned char displayChar=0;
		int width=0;
		for(int i=0; i<totalSize; i++) {
	
			if(i == m.hero) {
				
				if(!edit_mode) {
					printw("i");
				} else {
					printw("+");
				}
			
			} else {
		
				switch(m.data[i]) {
			
					case '0':
						displayChar = ' ';
						break;
					
					case '1':
						displayChar = '#';
						break;
						
					case '2':
						displayChar = 'O';
						break;
					
					case '3':
						displayChar = 'A';
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
		
		if(edit_mode) {
			printw("%c : Empty | %c : Wall | %c : Road | %c : Tree\n", BLOCK_EMPTY, BLOCK_WALL, BLOCK_ROAD, BLOCK_TREE);
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
				
			case BLOCK_EMPTY:
				if(edit_mode)
					m.data[m.hero] = BLOCK_EMPTY;
				break;
				
			case BLOCK_WALL:
				if(edit_mode)
					m.data[m.hero] = BLOCK_WALL;
				break;
				
			case BLOCK_ROAD:
				if(edit_mode)
					m.data[m.hero] = BLOCK_ROAD;
				break;
				
			case BLOCK_TREE:
				if(edit_mode)
					m.data[m.hero] = BLOCK_TREE;
				break;
				
			case 's':
				if(edit_mode) {
					endwin();
					if(write_mapfile(&m, filename) == PMT_SUCCESS) {
						printf("Map successfully saved.\n");
						return PMT_SUCCESS;
					} else {
						printf("Unable to save map file.\n");
						return PMT_ERROR;
					}
				}
				break;
				
			case 'q':
				endwin();
				return PMT_SUCCESS;

		}
		
		if(m.data[nextPosition] != BLOCK_WALL || edit_mode) {
			m.hero = nextPosition;
		} else {
			nextPosition = m.hero;
		}
		
		clear();
	
	}
	
}
