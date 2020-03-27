#pragma once 
#include <iostream>
#include <vector>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "oj_log.hpp"

class StringTool//写一个切割字符串的工具
{
    public:
        static void Split(const std::string& Str,const std::string split_char,std::vector<std::string>* out)
        {
            boost::split(*out,Str,boost::is_any_of(split_char),boost::token_compress_off);
        }
};

//实现文件操作的类
class FileOper
{
    public:
        static int ReadDataFromFile(const std::string& filename,std::string* content)
        {
            std::ifstream file(filename.c_str());
            if(!file.is_open())
            {
                LOG(ERROR,"Open File Faile:")<<filename<<std::endl;
                return -1;
            }
            
            std::string line;
            while(std::getline(file,line))
            {
                *content += line + "\n";
            }
            file.close();
            return 0;
        }
};
