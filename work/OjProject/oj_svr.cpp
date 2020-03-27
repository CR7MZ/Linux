#include <cstdio>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include "TextLib.hpp"
#include "httplib.h"
#include "oj_view.hpp"
#include "oj_log.hpp"

using namespace std;

int main()
{
    using namespace httplib;
    Server ser;
    TextLib Tex;
    ser.Get("/all_questions",[&Tex](const Request& req,Response& resp){
                vector<TextInfo> tes;
                Tex.GetAllText(&tes);
                //char buf[10240] = {'\0'};
                //cout<< tes.size()<<endl;
                //if(tes.size()!= 0)
                //{
                //snprintf(buf,sizeof(buf)-1,"<html>%s.%s %s</html>",tes[0]._id.c_str(),tes[0]._name.c_str(),tes[0]._star.c_str());
                //}
                //string html;
                //html.assign(buf,strlen(buf));
                std::string html;
                OJview::ExpandAllQuestionsHtml(&html,tes);
                resp.set_content(html,"text/html;charset=UTF-8");
            });

    //处理用户对单个题目信息的请求。
    ser.Get(R"(/question/(\d+))",[&Tex](const Request& req,Response& resp){
                std::string desc;
                std::string header;
                //从请求中获取请求的题目id。
                LOG(INFO,"req.matches")<<req.matches[0]<<":"<<req.matches[1]<<std::endl;
                //在题目地址路径下去加载单个题目详细信息
                struct TextInfo tex;
                Tex.GetOneText(req.matches[1].str(),&desc,&header,&tex);
                //进行组织渲染，返回给浏览器
                std::string html;
                OJview::ExpandOneQuestionsHtml(tex,desc,header,&html); 
                resp.set_content(html,"text/html;charset=UTF-8");
            });
    LOG(INFO,"listen in 0.0.0.0:19999")<<std::endl;
    LOG(INFO,"Server ready")<<std::endl;
    ser.listen("0.0.0.0",19999);
    return 0; 
}
