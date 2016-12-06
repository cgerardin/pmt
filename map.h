#ifndef H_MAP
#define H_MAP

#define PMT_BLOCK_EMPTY	'0'
#define PMT_BLOCK_WALL1	'1'
#define PMT_BLOCK_WALL2	'2'
#define PMT_BLOCK_WALL3	'3'
#define PMT_BLOCK_DOOR	'4'
#define PMT_BLOCK_ROAD	'5'
#define PMT_BLOCK_TREE	'6'
#define PMT_BLOCK_CHEST	'7'
#define PMT_BLOCK_LICHE	'8'

#define PMT_TILE_HERO	'i'
#define PMT_TILE_EMPTY	' '
#define PMT_TILE_WALL1	'-'
#define PMT_TILE_WALL2	'|'
#define PMT_TILE_WALL3	'+'
#define PMT_TILE_DOOR	'#'
#define PMT_TILE_ROAD	'H'
#define PMT_TILE_TREE	'A'
#define PMT_TILE_CHEST	'$'
#define PMT_TILE_LICHE	'&'

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
