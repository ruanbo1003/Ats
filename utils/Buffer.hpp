/*
 * Buffer.hpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ruanbo
 */

#ifndef BUFFER_HPP_
#define BUFFER_HPP_

#include "comm/comm.hpp"

class Buffer
{
#define DEFAULT_BUFF_SIZE 512
private:
    tr1::shared_ptr<char> _buff;
    size_t _size;


public:
    Buffer();
    virtual ~Buffer();

private:
    static void deleter(char* p);

public:
    size_t size()const;
    char* data()const;

    char* pdata();

    void append();
    void tozero();
};


using BufferPtr = tr1::shared_ptr<Buffer>;

#endif /* BUFFER_HPP_ */
