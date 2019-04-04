#include "lrc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

static long ms(char origin[9])										//将mm:ss.ms化为毫秒 
{
	long result=0;
	result=atoi(origin)*60*1000+atoi(origin+3)*1000+atoi(origin+6)*10;
	return result;
}

/*   输出模块   */ 
void OutputLyrics(lyric_t* head)
{
	lyric_t* p;
	for(p=head;p!=NULL;p=p->next)
		printf("%ld >> %s\n",p->timeLine,p->verse);
}

/*
 * 对每行lrc歌词提取有效信息
 */
static int LrcAnalysis_line(lyric_t* head ,char* str) {
	lyric_t* p;
	lyric_t* q;
	int i=0;

	//删除末尾的回车
	if(str[strlen(str)-1]=='\n')
		str[strlen(str)-1]='\0';
	
	//让i指向最右边的[]右侧]的右边，即指向该行歌词的第一个字 
	do {
		i++;
	}while(!(str[i-1]==']'&&str[i]!='['));

	//提取信息保存到节点
	p =  (lyric_t*)malloc(sizeof(lyric_t));              //为歌词节点分配内存
	p->timeLine = ms(str+i-9);    						 //解析每行歌词时间轴
	strcpy(p->verse,str+i);     						 //复制每行歌词正文到节点
	p->next = NULL;

	//追加节点
	if(head == NULL)
		head = p;
	else {
		q = head;
		while(q->next != NULL)
			q = q->next;
		q->next = p;
		p->prev = q;
	}
}

lyric_t* lyric_analysis(char* str) {
	char linePointer[256];
	lyric_t* head;

	/* 初始化首节点 */
	head =  (lyric_t*)malloc(sizeof(lyric_t));
	if(head==NULL)
		return NULL;
	head->timeLine = 0;
	strcpy(head->verse,"AWTK");
	head->next = NULL;
	head->prev = NULL;

	FILE *lrc=fopen(str,"r");
	if(lrc==NULL)
		return NULL;
	while(fgets(linePointer,256,lrc) != NULL) {
		if(linePointer[1] >= '0'&&linePointer[2]<='9')
			LrcAnalysis_line(head,linePointer);
    }
	fclose(lrc);

	return head;
	/*		按时间轴排序		*/ 
	// for(p=Head;p!=NULL;p=p->next)						//比较笨拙的选择排序，按时间轴从小到大 
	// {
	// 	tempMin=p;
	// 	for(q=p->next;q!=NULL;q=q->next)
	// 		if(tempMin->timeLine>q->timeLine)			//找最小的 
	// 			tempMin=q;								//寻找比最小还要小的，用临时指针标记 
	// 	if(tempMin!=p)									//如果确实找到了更小的 
	// 	{
	// 		temp=*tempMin;
	// 		*tempMin=*p;
	// 		*p=temp;
	// 		temp.next=tempMin->next;
	// 		tempMin->next=p->next;
	// 		p->next=temp.next;
	// 	}
	// }
}

/*
 * 根据时间查找当前歌词
 * 精度 +-100ms
 * 若存在相同的情况下返回第一句
 * 若+-100ms的范围内都没有合适的则返回NULL
 */
lyric_t* lyric_find(lyric_t* head, long time) {
	lyric_t* p;

	for(p=head;p!=NULL;p=p->next) {
		if((p->timeLine >= time-400)&&(p->timeLine < time+500)) {
			printf("timeLine=%d time=%d\n",p->timeLine,time);
			return p;
		}
	}
	return NULL;
}

/*
 * 删除lrc歌词链表
 */
void lyric_delete(lyric_t* head) {
	lyric_t* p;

	while(head != NULL) {
		p = head->next;
		free(head);
		if(p != NULL)
			p->prev = NULL;
		head = p;
	}
}

#if 0
int main()
{
	lyric_t* p=NULL;
	int i=0;
	lyric_t* q=NULL;

	p = lyric_analysis("E:\\Code_Unit\\123.lrc");

	/*		播放测试或打印到屏幕	*/ 
	//OutputLyrics(p);
	while(1)
	{
		q = lyric_find(p,i);
		if(q!=NULL)
			printf("%ld >> %s\n",i,q->verse);
		i += 100;
		q=NULL;
		Sleep(100);
		if(i>200000)
			break;
	}
    system("PAUSE");
	return 0;
}

#endif