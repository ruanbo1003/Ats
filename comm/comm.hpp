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

#include <iostream>

#include "Noncopyable.hpp"
#include "global.hpp"
#include "Log.hpp"

using namespace std;


void stdcopy(char* dest, const char* src, size_t len);


#endif /* COMM_COMM_HPP_ */
