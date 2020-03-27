#pragma once 
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <cstdio>
#include <string>

class LogTime
{
    public:
        static int64_t GetTimeStamp()
        {
            struct timeval tv;
            gettimeofday(&tv,NULL);
            return tv.tv_sec;
        }

        static void GetTimeStamp(std::string* TimeStamp)
        {
            time_t Systime;
            time(&Systime);
            struct tm* st = localtime(&Systime);
            char buf[] = {'\0'};
            snprintf(buf,sizeof(buf)-1,"%04d-%02d-%02d %02d:%02d:%02d",st->tm_year+1900,st->tm_mon+1,st->tm_mday,st->tm_hour,st->tm_min,st->tm_sec);
            TimeStamp->assign(buf,strlen(buf));
        }
};

//日志等级
const char* Level[]={
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL",
    "DEBUG",
};

enum LogLevel
{
    INFO = 0,
    WARNING,
    ERROR,
    FATAL,
    DEGUB
};

//使用inline函数，可以让其在调用处展开，提高效率。
inline std::ostream& Log(LogLevel Lev,const char* file,int line,const std::string& logmsg)
{
    std::string Level_info = Level[Lev];
    std::string TimeStamp;
    //获取时间
    LogTime::GetTimeStamp(&TimeStamp);
    //输出日志信息：[时间 日志等级 文件:行号] 具体的日志信息
    std::cout<<"["<<TimeStamp<<" "<<Level_info<<" "<<file<<":"<<line<<"]"<<logmsg;
    return std::cout;
}

#define LOG(lev,msg) Log(lev,__FILE__,__LINE__,msg)
