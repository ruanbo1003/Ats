/*
 * FileOp.hpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ruanbo
 */

#ifndef FILEOP_HPP_
#define FILEOP_HPP_

#include "comm/comm.hpp"
#include <fstream>

class FileOp : public NonCopyable
{
private:
    string _file_name;
    ofstream _f;

public:
    FileOp(const string& file_name);
    virtual ~FileOp();

public:
    bool is_valid();

    void write(const string& data);
    void write(const char* data, int len=0);
};


using FileOpPtr = tr1::shared_ptr<FileOp>;

#endif /* FILEOP_HPP_ */


