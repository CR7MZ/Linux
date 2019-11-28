#include<iostream>
using namespace std;
#include<string>
void Func()
{
    string s1;
    string s2;
    while(cin>>s1>>s2)
    {
        if(s1.size()>s2.size())
            swap(s1,s2);
        string tmp;
        int min=0;
        for(int i=0;i<s1.size();i++)
        {
            for(int j=i;j<s1.size();j++)
            {
               tmp=s1.substr(i,j-i+1);
               if(s2.find(tmp)==std::string::npos)
                   break;
               if(tmp.size()>min)
                   min=tmp.size();
            }
        }
        cout<<min<<endl;
    }
}
int main()
{
    Func();
    return 0;
}
