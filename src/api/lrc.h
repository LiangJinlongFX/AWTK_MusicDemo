#ifndef _LRC_H
#define _LRC_H

typedef struct _lyric_t {
	long timeLine;
	char verse[256];
	struct _lyric_t* next;
	struct _lyric_t* prev;
}lyric_t;

typedef struct _arrayfile_t {
	unsigned int offset;
	unsigned char* array;

}arrayfile_t;

void OutputLyrics(lyric_t* head);
lyric_t* lyric_analysis(char* str);
lyric_t* lyric_find(lyric_t* head, long time);
int lyric_findwithcounter(lyric_t* head, long time);
void lyric_delete(lyric_t* head);
lyric_t* lyric_load(unsigned char* str);

#endif // !_LRC_H
