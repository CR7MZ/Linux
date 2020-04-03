#pragma once 
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>

#include "oj_log.hpp"
#include "tools.hpp"

enum ErrorNo
{
    OK = 0,
    COMPILE_ERROR,
    RUN_ERROR,
    PRAM_ERROR,
    INTERNAL_ERROR
};

class Compiler
{
    public:
        static void CompileAndRun(Json::Value Req,Json::Value* Resp)
        {
            //判断代码是否为空
            if(Req["code"].empty())
            {
                (*Resp)["errorno"] = PRAM_ERROR;
                (*Resp)["reason"] = "Pram error";
                LOG(ERROR,"Request Commande is Empty")<<std::endl;
            }
            //将用户提交的代码写入到文件中去
            std::string code = Req["code"].asString();
            //文件名称进行约定 tmp_时间戳.cpp
            std::string tmp_filename =WriteTmpFile(code);
            if(tmp_filename == "")
            {
                (*Resp)["errorno"] = INTERNAL_ERROR;
                (*Resp)["reason"] = "create file failed";
                LOG(ERROR,"Write source failed");
                return ;
            }

            //编译
            if(!compile(tmp_filename))
            {
                (*Resp)["errorno"] = COMPILE_ERROR;
                std::string reason;
                FileOper::ReadDataFromFile(ErrorPath(tmp_filename),&reason);
                (*Resp)["reason"] = reason;
                LOG(ERROR,"compile error");
                return ;
            }

            //运行
            int sig = Run(ExePath(tmp_filename));
            if(sig != 0)
            {
                (*Resp)["errorno"] = RUN_ERROR;
                (*Resp)["reason"] = "Pragma exit by sig"+std::to_string(sig);
                LOG(ERROR,"Run error")<<std::endl;
                return ;
            }

            //构造响应
            (*Resp)["errorno"] = OK;
            (*Resp)["reason"] = "compile and run OK";
            
            std::string stdout_reason;
            FileOper::ReadDataFromFile(stdoutPath(tmp_filename),&stdout_reason);
            (*Resp)["stdout"] = stdout_reason;

            std::string stderr_reason;
            FileOper::ReadDataFromFile(stderrPath(tmp_filename),&stderr_reason);
            (*Resp)["stderr"] = stderr_reason;

            //Clean(tmp_filename);
            return ;

        }
private:
        static std::string WriteTmpFile(const std::string& code)
        {
            std::string tmp_filename = "/tmp_" + std::to_string(LogTime::GetTimeStamp());

            int ret = FileOper::WriteDataToFile(SrcPath(tmp_filename),code);
            if(ret < 0)
            {
                LOG(ERROR,"Write code to source failed");
                return "";
            }
            return tmp_filename;
        }
        
        static std::string SrcPath(const std::string& filename)
        {
            return "./tmp_files"+filename+".cpp";
        }

        static std::string ErrorPath(const std::string& filename)
        {
            return "./tmp_files"+filename+".err";
        }
        static std::string ExePath(const std::string& filename)
        {
            return "./tmp_files"+filename+".executable";
        }
        static std::string stdoutPath(const std::string& filename)
        {
            return "./tmp_files"+filename+".stdout";
        }
        static std::string stderrPath(const std::string& filename)
        {
            return "./tmp_files"+filename+".stderr";
        }

        static bool compile(const std::string& filename)
        {
            //1.构造编译命令--》g++ src -o [exec] -std=c++11
           const int commandcount = 20;
           char buf[commandcount][50] = {{0}};
           char* Command[commandcount] = {0};

           for(int i = 0; i < commandcount;i++)
           {
               Command[i] = buf[i];
           }
           snprintf(Command[0],49,"%s","g++");
           snprintf(Command[1],49,"%s",SrcPath(filename).c_str());
           snprintf(Command[2],49,"%s","-o");
           snprintf(Command[3],49,"%s",ExePath(filename).c_str());
           snprintf(Command[4],49,"%s","-std=c++11");
           snprintf(Command[5],49,"%s","-D");
           snprintf(Command[6],49,"%s","CompileOnline");
           Command[7] = NULL;

            //2.创建子进程
            int pid = fork();
            if(pid < 0)
            {
                LOG(ERROR,"fork failed");
                return false;
            }
            else if(pid == 0)
            {
                //创建一个文件来保存，编译错误信息。
                int fd = open(ErrorPath(filename).c_str(),O_CREAT | O_RDWR,0664);
                if(fd < 0)
                {
                    LOG(ERROR,"open err file failed")<<ErrorPath(filename)<<std::endl;
                    exit(1);
                }

                dup2(fd,2);

                execvp(Command[0],Command);
                perror("execvp");
                LOG(ERROR,"execvp failed")<<std::endl;
                exit(0);
            }
            else
            {
                waitpid(pid,NULL,0);     
            }
            //3.验证是否产生可执行程序
            struct stat st;
            int ret = stat(ExePath(filename).c_str(),&st);
            if(ret < 0 )
            {
                LOG(ERROR,"compile ERROR  Exe name is ")<<ExePath(filename)<<std::endl;
                return false;
            }
            return true;
        }
        
        static int Run(const std::string& filename)
        {
            //创建子进程
            pid_t pid = fork();
            if(pid < 0 )
            {
                LOG(ERROR,"fork failed")<<std::endl;
                return -1;
            }
            else if(pid == 0)
            {
                alarm(1);
                struct rlimit rl;
                rl.rlim_cur = 1024*30000;
                rl.rlim_max = RLIM_INFINITY;
                setrlimit(RLIMIT_AS,&rl);

                int stdout_fd = open(stdoutPath(filename).c_str(),O_CREAT | O_RDWR,0664);
                if(stdout_fd < 0)
                {
                    LOG(ERROR,"open stdout failed ")<<stdoutPath(filename)<<std::endl;
                    return -1;
                }

                dup2(stdout_fd,1);

                int stderr_fd = open(stderrPath(filename).c_str(),O_CREAT | O_RDWR,0664);
                if(stderr_fd < 0)
                {
                    LOG(ERROR,"open stderr failed ")<<stderrPath(filename)<<std::endl;
                    return -1;
                }

                dup2(stderr_fd,2);

                execl(ExePath(filename).c_str(),ExePath(filename).c_str(),NULL);
                exit(1);
            }
            int status = -1;
            waitpid(pid,&status,0);
            return status&0x7f;
        }
        static void Clean(std::string filename)
        {
            unlink(SrcPath(filename).c_str());
            unlink(ExePath(filename).c_str());
            unlink(ErrorPath(filename).c_str());
            unlink(stdoutPath(filename).c_str());
            unlink(stderrPath(filename).c_str());

        }
};
