#include <cstdio>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include "TextLib.hpp"
#include "httplib.h"
#include "oj_view.hpp"

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
    ser.listen("0.0.0.0",19999);
    return 0; 
}
