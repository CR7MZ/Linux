//命令行版本的进度条
#include<stdio.h>
#include<unistd.h>//常用头文件 只在linux下才有的文件。这样的头文件继承自unix。
int main()
{
  //cache 缓存  vs  buffer 缓冲区
  const char* label="/-\\|";
  char buf[1024]={0};
  int i=0;
  for(i=0;i<100;i++)
  {
  buf[i]='#';
  //\n表示换行，另起一行，光标来到行首
  //\r表示回车，不另起一行，光标来到行首
  printf("[%d%%][%c][%s]\r",i,label[i%4],buf);//i%4d代表依次按顺序显示label的元素。
  fflush(stdout);
  //linux上sleep单位为秒。  sleep操作不属于标准库。
  usleep(500*1000);//usleep单位为微秒。//括号中代表0.5秒
  }
  return 0;
}
