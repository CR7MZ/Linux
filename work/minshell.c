#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<sys/wait.h>
#include<stdlib.h>
//模拟小型shell
char g_command[1024];
int get_command()//获得输入的内容
{
   memset(g_command,'0',sizeof(g_command));//将全局变量初始化，并且最后必须有‘/‘
   printf("[minshell@localhost]$");
   fflush(stdout);
   if(!fgets(g_command,sizeof(g_command)-1,stdin))
   {
       perror("fgets");
       return -1;
   }
   return 0;
}

int Dealcommand(char* command)
{
    if(!command||*command=='\0')
    {
        printf("command fail\n");
        return -1;
    }
    char* argv[32]={0};
    int argc=0;
    while(*command)
    {
        if(!isspace(*command))
        {
            argv[argc]=command;
            argc++;
            while(!isspace(*command)&&*command!='\0')
            {
                command++;
            }
            *command='\0';
        }
        command++;
    }
    argv[argc]=NULL;
    Execlcommand(argv);
    return 0;
}

int Execlcommand(char* argv[])
{
    if(!argv[0])
    {
        return -1;
    }
    pid_t pid=0;
    pid=fork();
    if(pid<0)
    {
        perror("fork");
        return -1;
    }
    else if(pid==0)
    {
        execvp(argv[0],argv);
        exit(0);
    }
    else 
    {
        waitpid(pid,NULL,0);
    }
    return 0;
}
int main()
{
    while(1)
    {
        if(get_command()!=0)
        {
            continue;
        }
        Dealcommand(g_command);
    }
    return 0;
}
