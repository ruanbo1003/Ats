/*
 * LogFile.cpp
 *
 *  Created on: 2017Äê8ÔÂ12ÈÕ
 *      Author: ruanbo
 */

#include <utils/AtsLog.hpp>
#include <stdarg.h>
#include "TimeOp.hpp"

AtsLog::AtsLog(const string& file, bool with_data, AtsLogLevel level)
{
	string filename = "";
	if(with_data)
	{
		filename = file + TimeOp::nowdate() + ".log";
	}
	else
	{
		filename = file + ".log";
	}

	_file = tr1::shared_ptr<FileOp>(new FileOp(file));
	_buff_size = 1024;
	_buff = new char[_buff_size];
	bzero(_buff, _buff_size);

	_log_level = level;
}

AtsLog::~AtsLog()
{
	delete[] _buff;
}

void AtsLog::write(const char* fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	int cnt = vsnprintf(_buff, _buff_size, fmt, argptr);
	va_end(argptr);

	if(_log_level == AtsLog_File)
	{
		_file->write(_buff, cnt);
	}

}














