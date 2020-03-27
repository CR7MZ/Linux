#pragma once 
#include <iostream>
#include <ctemplate/template.h>
#include <vector>
#include <string>
#include "TextLib.hpp"

class OJview
{
    public:
        //渲染html页面，并返回给调用者
        static void ExpandAllQuestionsHtml(std::string* html,std::vector<TextInfo>& Tex)
        {
            //1.获取数据字典，将拿到的数据按顺序保存在内存中。
            ctemplate::TemplateDictionary dict("all_questions");

            for(const auto& e : Tex)
            {
                ctemplate::TemplateDictionary* section_dict = dict.AddSectionDictionary("question");
                section_dict->SetValue("id",e._id);
                section_dict->SetValue("id",e._id);
                section_dict->SetValue("name",e._name);
                section_dict->SetValue("star",e._star);
            }
            //2.获取模板类指针，加载预定义的html页面到内存当中
            ctemplate::Template* t1 = ctemplate::Template::GetTemplate("./template/all_questions.html",ctemplate::DO_NOT_STRIP);
            //3.渲染 拿着模板类的指针，将数据字典当中的数据更新到html页面的内存中
            t1->Expand(html,&dict);
        }
};

