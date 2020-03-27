#pragma once 
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>

class StringTool//写一个切割字符串的工具
{
    public:
        static void Split(const std::string& Str,const std::string split_char,std::vector<std::string>* out)
        {
            boost::split(*out,Str,boost::is_any_of(split_char),boost::token_compress_off);
        }
};
