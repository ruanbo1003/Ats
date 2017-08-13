/*
 * LogFile.hpp
 *
 *  Created on: 2017Äê8ÔÂ12ÈÕ
 *      Author: ruanbo
 */

#ifndef UTILS_ATSLOG_HPP_
#define UTILS_ATSLOG_HPP_

#include "FileOp.hpp"

enum AtsLogLevel
{
	AtsLog_Null = 0,

	AtsLog_Print = 1,
	AtsLog_File = 2,

	AtsLog_End = 9
};

class AtsLog : public NonCopyable
{
private:
	FileOpPtr _file;
	char *_buff;
	size_t _buff_size;

	char _log_level;

public:
	AtsLog(const string& file, bool with_data=false, AtsLogLevel level=AtsLog_Print);
	virtual ~AtsLog();

public:
	void write(const char* fmt, ...);

};

using AtsLogPtr = tr1::shared_ptr<AtsLog>;

#endif /* UTILS_ATSLOG_HPP_ */
