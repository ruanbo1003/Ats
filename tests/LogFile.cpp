/*
 * LogFile.cpp
 *
 *  Created on: 2017Äê8ÔÂ12ÈÕ
 *      Author: ruanbo
 */

#include "comm/comm.hpp"
#include "utils/AtsLog.hpp"

void log_file_test()
{
	string file = "/home/ruanbo/Desktop/test.log";
	AtsLogPtr log = tr1::shared_ptr<AtsLog>(new AtsLog(file, false, AtsLog_File));

	log->write("HelloWorld\n");

	log->write("hello %d\n", 1);
}



