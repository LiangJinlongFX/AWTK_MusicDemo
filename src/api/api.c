#include "api.h"
#include "stdio.h"
#include "awtk.h"

void chat_to_wchar(char* str1, wchar_t* str2)
{
  unsigned int i;
  
  i=0;
  while(str1[i] != '\0')
  {
    str2[i] = str1[i];
    i++;
  }
  str2[i] = '\0';
}

void time_to_wchar(uint32_t s, wchar_t* str)
{
    uint32_t min;
    uint32_t secs;
    char char_str[10];

    s = s/1000;
    min = s/60;
    secs = s%60;
    tk_snprintf(char_str,sizeof(char_str),"%02d:%02d",min,secs);
    chat_to_wchar(char_str,str);
}


/* 初始化链表首节点 */
music_info_t* musiclist_init(void) {
   return TKMEM_ZALLOC(music_info_t);
}

/* 在尾部插入一个节点 */
music_info_t* musiclist_insert(music_info_t* pHead) {
  music_info_p p,New;
  uint32_t i=0;
   //为新节点分配空间
  New = TKMEM_ZALLOC(music_info_t);
  if(!New)
    return NULL;
  //指针移动到尾节点
  p = pHead;
  while(p->next != NULL)
  {
    p = p->next;
    i++;
  }
  //追加节点
  p->next = New;
  p->index = i;
  New->next = NULL;

  return New;
}

/* 查找指定索引号的节点 */
music_info_t* musiclist_find(music_info_t* pHead, uint32_t index) {
  uint32_t i=0;
  music_info_p p;
  p = pHead;
  while(p != NULL) {
    if(p->index == index)
      return p;
    p = p->next;
  }

  return NULL;
}

/* 获取当前所有节点数 */
uint32_t musiclist_count(music_info_t* pHead) {
  uint32_t i=0;
  music_info_p p;
  p = pHead;
  while(p != NULL) {
    p = p->next;
    i++;
  }

  return i;
}

music_info_t* musiclist_default(void) {
  music_info_t* head;
  music_info_t* p;

  head = musiclist_init();
  if(head == NULL)
    return NULL;
  head->index = 0;
  sprintf(head->song_name,"Turn It Up");
  sprintf(head->singer_name,"s7erre");
  head->total_time = 210000;
  p = musiclist_insert(head);
  sprintf(p->song_name,"Favor");
  sprintf(p->singer_name,"Skrillex");
  p->total_time = 210000;
  p = musiclist_insert(head);
  sprintf(p->song_name,"Feel Alive");
  sprintf(p->singer_name,"Elysa");
  p->total_time = 210000;
  p = musiclist_insert(head);
  sprintf(p->song_name,"MISS U");
  sprintf(p->singer_name,"Cesqeaux");
  p->total_time = 210000;
  p = musiclist_insert(head);
  sprintf(p->song_name,"Nobody");
  sprintf(p->singer_name,"Danelle Sandoval");
  p->total_time = 210000;
  p = musiclist_insert(head);
  sprintf(p->song_name,"Mind(4B Remix)");
  sprintf(p->singer_name,"4B/Skrillex");
  p->total_time = 210000;
  p = musiclist_insert(head);
  sprintf(p->song_name,"Can't Let You Go");
  sprintf(p->singer_name,"Josef Bamba");
  p->total_time = 210000;
  p = musiclist_insert(head);
  sprintf(p->song_name,"The Middle");
  sprintf(p->singer_name,"Zedd/Grey/Maren Morris");
  p->total_time = 210000;
  p = musiclist_insert(head);
  sprintf(p->song_name,"Drunk");
  sprintf(p->singer_name,"Marky Style");
  p->total_time = 210000;
  p = musiclist_insert(head);
  sprintf(p->song_name,"Test End");
  sprintf(p->singer_name,"AWTK");
  p->total_time = 210000;

  return head;
}