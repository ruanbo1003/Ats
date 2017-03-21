/*
 * FileOp.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ruanbo
 */

#include "FileOp.hpp"

FileOp::FileOp(const string& file_name)
{
    _file_name = file_name;

    _f = ofstream(file_name.c_str());
//    _f.open(file_name.c_str(), ios::in|ios::out);
}

FileOp::~FileOp()
{
    _f.close();
}

bool FileOp::is_valid()
{
    return _f.is_open();
}

void FileOp::write(const string& data)
{
    if(is_valid() == false)
    {
        return;
    }

    _f << data;
}

void FileOp::write(const char* data, int len)
{
    if(is_valid() == false)
    {
//        Log("write 2..is_valid failed");
        return;
    }

    _f << data;
}
