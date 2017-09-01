/*
 * comm.hpp
 *
 *      Author: ruanbo
 */

#ifndef COMM_COMM_HPP_
#define COMM_COMM_HPP_


#include <tr1/memory>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <set>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/syscall.h>

#include <iostream>

#include "Noncopyable.hpp"
#include "global.hpp"
#include "Log.hpp"

using namespace std;


void stdcopy(char* dest, const char* src, size_t len);

#define gettid() syscall(__NR_gettid)


//1000万亿
#define DoubleMaxDef 1000000000000000
#define DoubleCheck(v) {if((v) > DoubleMaxDef){v=0;}}



#endif /* COMM_COMM_HPP_ */
