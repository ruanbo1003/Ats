/*
 * FileOp.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ruanbo
 */

#include "FileOp.hpp"
#include <sys/stat.h>

FileOp::FileOp(const string& file_name)
{
    _file_name = file_name;
    _opened = false;

//    _f = ofstream(file_name.c_str());
    create_dirctory(file_name);

    _f.open(file_name.c_str(), ios::in | ios::out );  // | ios::app
    if(_f.is_open() == false)
    {
        Log("ofstream open file:%s failed, errorno:%d", file_name.data(), errno);
    }
    else
    {
        Log("ofstream open file:%s ok", file_name.data());
        _opened = true;
    }
}

FileOp::~FileOp()
{
	_f.flush();
    _f.close();
}

bool FileOp::is_dirctory(const string& dir)
{
    struct stat st;
    if(stat(dir.data(), &st) != 0)
    {
        return false;
    }

    if(S_ISDIR(st.st_mode))
    {
        return true;
    }
    else
    {
        return false;
    }
}
void FileOp::create_dirctory(const string& dir)
{
    size_t len = dir.length();
    if(len <= 1)
    {
        return;
    }

    for(size_t i = 1; i<len; ++i)
    {
        if(dir[i] == '/')
        {
            string parent_dir(dir, 0, i);

            if(is_dirctory(parent_dir) == false)
            {
                mkdir(parent_dir.data(), 0755);
            }
        }
    }
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

bool FileOp::opened()const
{
    return _opened;
}









