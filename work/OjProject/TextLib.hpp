#pragma once 
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "tools.hpp"
#include "oj_log.hpp"

typedef struct TextInfo
{
    std::string _id;
    std::string _name;
    std::string _path;
    std::string _star;
}test;

class TextLib
{
    public:
    //利用构造函数将配置文件中的试题，加载到_map中。
    TextLib()
    {
         LoadText("/home/LMZ/bitefile/OJProject/source/ifconfig_file.cfg");
    }
    //提供输出全部试题的接口
    bool GetAllText(std::vector<TextInfo>* v)
    {
        for(const auto e : _map)
        {
            v->push_back(e.second);
        }
        //提供出去的试题必须是根据id升序的
        std::sort(v->begin(),v->end(),[](const TextInfo& l,const TextInfo& r){
                    return std::atoi(l._id.c_str()) < std::atoi(r._id.c_str());
                });
        return true;
    }

    //获取一个试题的详细信息
    bool GetOneText(const std::string& id , std::string* des,std::string* header,TextInfo* Tex)
    {
        auto itr = _map.find(id);
        if(itr == _map.end())
        {
            LOG(ERROR,"Not Found Question :")<<id<<std::endl;
            return false;
        }

        *Tex = itr->second;
        int ret = FileOper::ReadDataFromFile(DescPath(itr->second._path),des);
        if(ret == -1)
        {
            LOG(ERROR,"Read desc failed")<<std::endl;
            return false;
        }
        ret = FileOper::ReadDataFromFile(HeaderPath(itr->second._path),header);
        if(ret == -1)
        {
            LOG(ERROR,"Read header failed")<<std::endl;
            return false;
        }
        return true;
    }

    bool SplicingCode(std::string user_code,const std::string& Que_id,std::string* Code)
    {
        auto itr = _map.find(Que_id);
        if(itr == _map.end())
        {
            LOG(ERROR,"CAN NOT FIND QUESTION ID IS")<<Que_id<<std::endl;
            return false;
        }

        std::string taili_code;
        int ret = FileOper::ReadDataFromFile(TailPath(itr->second._path),&taili_code);
       if(ret < 0)
       {
           LOG(ERROR,"Open tail.cpp failed");
           return false;
       }

       *Code = user_code + taili_code;
       return true;
    }

    private:
    bool LoadText(const std::string& config_FilePath)
    {
        std::ifstream file(config_FilePath.c_str());
        if(!file.is_open())
        {
            return false;
        }

        std::string Line;
        while(std::getline(file,Line))//从配置文件中读取一行，分别获取每个题目的信息
        {
            //分割字符串，将一个题目的每个信息分离出来
            std::vector<std::string> tmp;
            StringTool::Split(Line," ",&tmp);
            if(tmp.size()!=4)
                continue;
            TextInfo node;
            node._id = tmp[0];
            node._name = tmp[1];
            node._path = tmp[2];
            node._star = tmp[3];
            _map[node._id] = node;
        }
        file.close();
        return true;
    }

    std::string DescPath(const std::string& Path)
    {
        return Path + "desc.txt";
    }

    std::string HeaderPath(const std::string& Path)
    {
        return Path + "header.cpp";
    }
    std::string TailPath(const std::string& que_path)
    {
        return que_path + "tail.cpp"; 
    }
    private:
        std::unordered_map<std::string , TextInfo> _map;
};
