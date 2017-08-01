/*
 *
 *      Author: ruanbo
 */

#ifndef UTIL_UTILS_HPP_
#define UTIL_UTILS_HPP_

#include "comm/comm.hpp"

namespace utils {

void split(const string& str, const string& delim, std::vector<string>& ret);

void trim_left(string& src, char c);
void trim_right(string& src, char c);

string get_path();

}  // namespace utils


#endif /* UTIL_UTILS_HPP_ */
