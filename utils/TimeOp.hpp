/*
 * Timer.hpp
 *
 *  Created on: Mar 10, 2017
 *      Author: ruanbo
 */

#ifndef TIMEOP_HPP_
#define TIMEOP_HPP_

#include "comm/comm.hpp"
#include <chrono>

enum TimerEventType{
    TET_Null = 0,
    TET_Second = 0x01,
    TET_Minute = 0x02,
    TET_Hour = 0x04,
};

class TimeOp : public NonCopyable
{
private:
    //定时器变量
    long int _last_ms;
    long int _last_s;
    std::tm _last_tm;

public:
    TimeOp();
    virtual ~TimeOp();

public:
    void onUpdate();

public:
    TimerEventType update();

    std::pair<string, string> date_time();

    string date();

    time_t nowt()const;

    string time2date(time_t t);
};

using TimeOpPtr = tr1::shared_ptr<TimeOp>;


#endif /* TIMEOP_HPP_ */
