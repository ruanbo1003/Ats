/*
 * utils.cpp
 *
 *      Author: ruanbo
 */

#include "utils.hpp"

namespace utils {

void split(const string& str, const string& delim, std::vector<string>& ret)
{
    size_t last = 0;
    size_t index = str.find_first_of(delim, last);

    while(index != std::string::npos)
    {
        ret.push_back(str.substr(last,index-last));
        last=index+1;
        index=str.find_first_of(delim,last);
    }

    if (index-last>0)
    {
        ret.push_back(str.substr(last,index-last));
    }
}

void trim_left(string& src, char c)
{
    src.erase(0, src.find_first_not_of(c));
}

void trim_right(string& src, char c)
{
    src.erase(src.find_last_not_of(c) + 1);
}

string get_path()
{
    char cwd[1024];

    if (getcwd(cwd,sizeof(cwd)) == NULL)
    {
        return NULL;
    }

    return cwd;
}

}  // namespace utils
