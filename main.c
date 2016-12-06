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
	curs_set(0);
	start_color();
	
	char key=0;
	int totalSize = m.sizeW*m.sizeH;
	int nextPosition=m.hero;
	
	init_pair(1, COLOR_CYAN, COLOR_WHITE); // PMT_TILE_HERO
	init_pair(2, COLOR_BLACK, COLOR_WHITE); // PMT_TILE_EMPTY
    init_pair(3, COLOR_BLACK, COLOR_MAGENTA); // PMT_TILE_WALL
    init_pair(4, COLOR_WHITE, COLOR_MAGENTA); // PMT_TILE_DOOR
    init_pair(5, COLOR_YELLOW, COLOR_WHITE); // PMT_TILE_ROAD
    init_pair(6, COLOR_GREEN, COLOR_WHITE); // PMT_TILE_TREE
    init_pair(7, COLOR_BLACK, COLOR_YELLOW); // PMT_TILE_CHEST
    init_pair(8, COLOR_RED, COLOR_WHITE); // PMT_TILE_LICHE
	
	while(1) {
	
		unsigned char displayChar=0;
		int width=0;
		for(int i=0; i<totalSize; i++) {
	
			if(i == m.hero) {
			
				attron(COLOR_PAIR(1));
				
				if(!edit_mode) {
					printw("%c", PMT_TILE_HERO);
				} else {
					printw("*");
				}
			
			} else {
		
				switch(m.data[i]) {

					case PMT_BLOCK_EMPTY:
						attron(COLOR_PAIR(2));
						displayChar = PMT_TILE_EMPTY;
						break;
					
					case PMT_BLOCK_WALL1:
						attron(COLOR_PAIR(3));
						displayChar = PMT_TILE_WALL1;
						break;
						
					case PMT_BLOCK_WALL2:
						attron(COLOR_PAIR(3));
						displayChar = PMT_TILE_WALL2;
						break;
					
					case PMT_BLOCK_WALL3:
						attron(COLOR_PAIR(3));
						displayChar = PMT_TILE_WALL3;
						break;
					
					case PMT_BLOCK_DOOR:
						attron(COLOR_PAIR(4));
						displayChar = PMT_TILE_DOOR;
						break;
						
					case PMT_BLOCK_ROAD:
						attron(COLOR_PAIR(5));
						displayChar = PMT_TILE_ROAD;
						break;
					
					case PMT_BLOCK_TREE:
						attron(COLOR_PAIR(6));
						displayChar = PMT_TILE_TREE;
						break;
						
					case PMT_BLOCK_CHEST:
						attron(COLOR_PAIR(7));
						displayChar = PMT_TILE_CHEST;
						break;
						
					case PMT_BLOCK_LICHE:
						attron(COLOR_PAIR(8));
						displayChar = PMT_TILE_LICHE;
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
			printw("%c : %c (Empty) | %c : %c(Wall 1) | ", PMT_BLOCK_EMPTY, PMT_TILE_EMPTY, PMT_BLOCK_WALL1, PMT_TILE_WALL1);
			printw("%c : %c (Wall 2) | %c : %c (Wall 3) | ", PMT_BLOCK_WALL2, PMT_TILE_WALL2, PMT_BLOCK_WALL3, PMT_TILE_WALL3);
			printw("%c : %c (Door) \n %c : %c (Road) | ", PMT_BLOCK_DOOR, PMT_TILE_DOOR, PMT_BLOCK_ROAD, PMT_TILE_ROAD);
			printw("%c : %c (Tree) | %c : %c (Chest)", PMT_BLOCK_TREE, PMT_TILE_TREE, PMT_BLOCK_CHEST, PMT_TILE_CHEST);
			printw("%c : %c (Liche) \n", PMT_BLOCK_LICHE, PMT_TILE_LICHE);
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
				
			case PMT_BLOCK_EMPTY:
				if(edit_mode)
					m.data[m.hero] = PMT_BLOCK_EMPTY;
				break;
				
			case PMT_BLOCK_WALL1:
				if(edit_mode)
					m.data[m.hero] = PMT_BLOCK_WALL1;
				break;
			
			case PMT_BLOCK_WALL2:
				if(edit_mode)
					m.data[m.hero] = PMT_BLOCK_WALL2;
				break;
				
			case PMT_BLOCK_WALL3:
				if(edit_mode)
					m.data[m.hero] = PMT_BLOCK_WALL3;
				break;
				
			case PMT_BLOCK_DOOR:
				if(edit_mode)
					m.data[m.hero] = PMT_BLOCK_DOOR;
				break;
			
			case PMT_BLOCK_ROAD:
				if(edit_mode)
					m.data[m.hero] = PMT_BLOCK_ROAD;
				break;
				
			case PMT_BLOCK_TREE:
				if(edit_mode)
					m.data[m.hero] = PMT_BLOCK_TREE;
				break;
			
			case PMT_BLOCK_CHEST:
				if(edit_mode)
					m.data[m.hero] = PMT_BLOCK_CHEST;
				break;
				
			case PMT_BLOCK_LICHE:
				if(edit_mode)
					m.data[m.hero] = PMT_BLOCK_LICHE;
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
		
		if(edit_mode || (m.data[nextPosition] != PMT_BLOCK_WALL1 
			&& m.data[nextPosition] != PMT_BLOCK_WALL2 
			&& m.data[nextPosition] != PMT_BLOCK_WALL3)) 
		{
			m.hero = nextPosition;
		} else {
			nextPosition = m.hero;
		}
		
		clear();
	
	}
	
}
