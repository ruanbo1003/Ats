/*
 * comm.cpp
 *
 *      Author: ruanbo
 */

#include <comm/comm.hpp>

void stdcopy(char* dest, const char* src, size_t len)
{
    std::copy(src, src+len, dest);
}





