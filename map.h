#ifndef H_MAP
#define H_MAP

typedef struct {
	char magic[2];
	int version;
	char name[256];
	int sizeW;
	int sizeH;
	int hero;
	char data[5000];	
} MAP;

int make_mapfile(char filename[], char name[], int sizeW, int sizeH);

#endif /* H_MAP */
