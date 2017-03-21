/*
 * CodeConvert.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ruanbo
 */

#include <utils/CodeConvert.hpp>

CodeConvert::CodeConvert(const std::string& from, const std::string& to)
{
    _cd = iconv_open(to.data(), from.data());
    _buff = BufferPtr(new Buffer());
}

CodeConvert::~CodeConvert()
{
    if(_cd)
    {
        iconv_close(_cd);
    }
    _cd = NULL;
}


int CodeConvert::convert(char* inbuf, int inlen, char* outbuf, int outlen)
{
    if(!_cd)
    {
        return 0;
    }

    char **pin = &inbuf;
    char **pout = &outbuf;

    return iconv(_cd, pin, (size_t*)&inlen, pout, (size_t*)&outlen);
}


char* CodeConvert::convert2(char* inbuf, size_t inlen)
{
    if(!_cd)
    {
        LogError("convert2 is not valid(!_cd)");
        return NULL;
    }

    char **pin = &inbuf;

    while((size_t)(inlen * 4) > _buff->size())
    {
        _buff->append();
    }

    _buff->tozero();
    char *pout = _buff->pdata();
    size_t outlen = _buff->size();

    Log("before iconv inlen:%ld, outlen:%ld", inlen, outlen);

    int ret = iconv(_cd, pin, (size_t*)&inlen, (char**)&pout, (size_t*)&outlen);
    if(ret < 0)
    {
        Log("convert failed inlen:%ld, outlen:%ld, ret:%d, errno:%d", inlen, outlen, ret, errno);
//        return NULL;
    }
    else
    {
        Log("convert OK inlen:%ld, outlen:%ld, ret:%d, errno:%d", inlen, outlen, ret, errno);
    }

    pin = NULL;
    pout = NULL;

    return _buff->data();
}








