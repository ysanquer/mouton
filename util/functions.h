#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <algorithm>
#include <functional>
#include <cctype>
#include <string>

inline void str_trim_ws(std::string &str)
{
    // erase leading ws
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int,int>(std::isspace))));
    // erase trailing ws
    str.erase(std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int,int>(std::isspace))).base(), str.end());
}

#endif
