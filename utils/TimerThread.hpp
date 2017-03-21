/*
 * TimerThread.hpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ruanbo
 */

#ifndef TIMERTHREAD_HPP_
#define TIMERTHREAD_HPP_

#include "comm/comm.hpp"

class TimerThread : public NonCopyable
{
public:
    TimerThread();
    virtual ~TimerThread();
};

#endif /* TIMERTHREAD_HPP_ */
