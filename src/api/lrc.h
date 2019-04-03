#ifndef _LRC_H
#define _LRC_H

typedef struct _lyric_t {
	long timeLine;
	char verse[256];
	struct _lyric_t* next;
	struct _lyric_t* prev;
}lyric_t;


void OutputLyrics(lyric_t* head);
lyric_t* lyric_analysis(char* str);
lyric_t* lyric_find(lyric_t* head, long time);
lyric_t* lyric_default(void);
void lyric_delete(lyric_t* head);

#endif // !_LRC_H
