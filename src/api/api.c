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


music_info_t* musiclist_default(void) {
  music_info_t* head;
  music_info_t* p;

  head = musiclist_init();
  if(head == NULL)
    return NULL;
  head->index = 0;
  sprintf(head->song_name,"Turn It Up");
  sprintf(head->Artist_name,"s7erre");
  head->total_time = 210000;

  return head;
}