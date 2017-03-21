/*
 * Buffer.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ruanbo
 */

#include <utils/Buffer.hpp>

Buffer::Buffer()
{
    _size = DEFAULT_BUFF_SIZE;
    _buff = tr1::shared_ptr<char>(new char[_size], deleter);
}

Buffer::~Buffer()
{
}

void Buffer::deleter(char* p)
{
    delete [] p;
    p = NULL;
}

size_t Buffer::size()const
{
    return _size;
}

char* Buffer::data()const
{
    return _buff.get();
}

char* Buffer::pdata()
{
    return _buff.get();
}

void Buffer::append()
{
    size_t new_size = _size * 2;
    char* new_buff = new char[new_size];

    _buff.reset(new_buff, deleter);
    _size = new_size;
}

void Buffer::tozero()
{
    bzero(_buff.get(), _size);
}







