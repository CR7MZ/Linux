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

        static void ExpandOneQuestionsHtml(const TextInfo& Tex,std::string& dec,std::string& header,std::string* html)
        {
           ctemplate::TemplateDictionary dict("question");
           dict.SetValue("id",Tex._id);
           dict.SetValue("name",Tex._name);
           dict.SetValue("star",Tex._star);
           dict.SetValue("desc",dec);
           dict.SetValue("header",header);

           ctemplate::Template* tpl = ctemplate::Template::GetTemplate("./template/question.html",ctemplate::DO_NOT_STRIP);

           tpl->Expand(html,&dict);
        }
        static void ExpandReason(const std::string& errorno,const std::string& reason,const std::string& stdout_reason,std::string* html)
        {
           ctemplate::TemplateDictionary dict("reason");
           dict.SetValue("errorno",errorno);
           dict.SetValue("reason",reason);
           dict.SetValue("stdout_reason",stdout_reason);

           ctemplate::Template* tpl = ctemplate::Template::GetTemplate("./template/reason.html",ctemplate::DO_NOT_STRIP);
           tpl->Expand(html,&dict);
        }        
};

