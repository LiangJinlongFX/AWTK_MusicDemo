/**
 * History:
 * ================================================================
 * 本文件包含了以下功能
 * 程序用到的一些杂项功能函数
 * 音乐链表的相关操作
 */
#include "api.h"
#include <stdio.h>
#include "awtk.h"

void chat_to_wchar(char* str1, wchar_t* str2) {
  unsigned int i;

  i = 0;
  while (str1[i] != '\0') {
    str2[i] = str1[i];
    i++;
  }
  str2[i] = '\0';
}

void time_to_wchar(uint32_t s, wchar_t* str) {
  uint32_t min;
  uint32_t secs;
  char char_str[10];

  s = s / 1000;
  min = s / 60;
  secs = s % 60;
  tk_snprintf(char_str, sizeof(char_str), "%02d:%02d", min, secs);
  chat_to_wchar(char_str, str);
}

void my_str_replace(char* str, char src, char dst) {
	char* p = str;

	while(*p != '\0') {
		if(*p == src)
			*p = dst;
		p++;
	}
}

/**
 * 在链表尾部插入一个节点
 **/
music_info_t* musiclist_insert(music_info_t* pHead) {
  music_info_t* p = NULL;
  music_info_t* New = NULL;
  uint32_t i = 0;

  //为新节点分配空间
  New = TKMEM_ZALLOC(music_info_t);
  if (New == NULL) return NULL;
  //指针移动到尾节点
  p = pHead;
  while (p->next != NULL) {
    p = p->next;
    i++;
  }
  //追加节点
  p->next = New;
  New->index = i + 1;
  New->next = NULL;

  return New;
}

/**
 * 在链表当中查询指定索引的节点
 **/
music_info_t* musiclist_find(music_info_t* pHead, uint32_t index) {
  music_info_t* p;
  p = pHead;
  while (p != NULL) {
    if (p->index == index) return p;
    p = p->next;
  }

  return NULL;
}

/**
 * 获取当前链表的所有节点数
 **/
uint32_t musiclist_count(music_info_t* pHead) {
  uint32_t i = 0;
  music_info_t* p;
  p = pHead;
  while (p != NULL) {
    p = p->next;
    i++;
  }

  return i;
}

/**
 * 打印当前链表的所有节点
 **/
void print_playlist(music_info_t* pHead) {
  music_info_t* p;
  p = pHead;
  while (p != NULL) {
    printf("%d path: %s\n", p->index, p->song_name);
    p = p->next;
  }
}

/**
 * 模拟读取音频文件写到链表当中
 **/
music_info_t* musiclist_default(void) {
  music_info_t* p = NULL;
  music_info_t* head = NULL;

  head = TKMEM_ZALLOC(music_info_t);
  if (head == NULL) return NULL;
  head->index = 0;
  sprintf(head->song_name, "Turn It Up");
  sprintf(head->Artist_name, "s7erre");
  head->total_time = 210000;
  p = musiclist_insert(head);
  if (p == NULL) return head;
  sprintf(p->song_name, "Favor");
  sprintf(p->Artist_name, "Skrillex");
  p->total_time = 210000;
  p = musiclist_insert(head);
  if (p == NULL) return head;
  sprintf(p->song_name, "Feel Alive");
  sprintf(p->Artist_name, "Elysa");
  p->total_time = 210000;
  p = musiclist_insert(head);
  if (p == NULL) return head;
  sprintf(p->song_name, "MISS U");
  sprintf(p->Artist_name, "Cesqeaux");
  p->total_time = 210000;


  return head;
}