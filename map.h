#ifndef H_MAP
#define H_MAP

#define BLOCK_EMPTY	'0'
#define BLOCK_WALL	'1'
#define BLOCK_ROAD	'2'
#define BLOCK_TREE	'3'

typedef struct {
	char magic[2];
	int version;
	char name[256];
	int sizeW;
	int sizeH;
	int hero;
	char data[5000];	
} MAP;

#define PMT_ERROR_OPEN_MAP 1
#define PMT_WRONG_MAP_FORMAT 2
#define PMT_WRONG_MAP_VERSION 3

int read_mapfile(char filename[], MAP *map);

int write_mapfile(MAP *map, char filename[]);

int make_mapfile(char filename[], char name[], int sizeW, int sizeH);

#endif /* H_MAP */
