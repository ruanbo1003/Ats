/*
 * CodeConvert.hpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ruanbo
 */

#ifndef CODECONVERT_HPP_
#define CODECONVERT_HPP_

#include <iconv.h>
#include "comm/comm.hpp"
#include "utils/Buffer.hpp"


// codecvt_utf8
class CodeConvert : public NonCopyable
{
private:
    iconv_t _cd;

    BufferPtr _buff;

public:
    CodeConvert(const std::string& from, const std::string& to);
    virtual ~CodeConvert();

public:
    int convert(char* inbuf, int inlen, char* outbuf, int outlen);

    char* convert2(char* inbuf, size_t inlen);
};

using CodeConvertPtr = tr1::shared_ptr<CodeConvert>;


#endif /* CODECONVERT_HPP_ */


