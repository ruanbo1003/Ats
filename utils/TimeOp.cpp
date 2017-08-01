/*
 * Timer.cpp
 *
 *  Created on: Mar 10, 2017
 *      Author: ruanbo
 */

#include <utils/TimeOp.hpp>
#include <sstream>
#include <iomanip>

TimeOp::TimeOp()
{
    auto nowt = chrono::system_clock::now();
    _last_ms = chrono::duration_cast<chrono::milliseconds>(nowt.time_since_epoch()).count();
    _last_s = chrono::system_clock::to_time_t(nowt);
    _last_tm = *std::localtime(&_last_s);

    bzero(_datebuf, sizeof(_datebuf));
    bzero(_timebuf, sizeof(_timebuf));
}

TimeOp::~TimeOp()
{

}


void TimeOp::onUpdate()
{
    auto nowt = chrono::system_clock::now();
    _last_ms = chrono::duration_cast<chrono::milliseconds>(nowt.time_since_epoch()).count();
    _last_s = chrono::system_clock::to_time_t(nowt);
    _last_tm = *std::localtime(&_last_s);
}

TimerEventType TimeOp::update()
{
    auto nowt = chrono::system_clock::now();
    long int cur_ms = chrono::duration_cast<chrono::milliseconds>(nowt.time_since_epoch()).count();
    long int cur_s = chrono::system_clock::to_time_t(nowt);
    std::tm cur_tm = *std::localtime(&cur_s);

    TimerEventType ret = TET_Null;

    bool need_update = false;
    if(cur_tm.tm_sec != _last_tm.tm_sec)
    {
        need_update = true;
        ret = TimerEventType(ret | TET_Second);
    }
    if(cur_tm.tm_min != _last_tm.tm_min)
    {
        need_update = true;
        ret = TimerEventType(ret | TET_Minute);
    }
    if(cur_tm.tm_hour != _last_tm.tm_hour)
    {
        need_update = true;
        ret = TimerEventType(ret | TET_Hour);
    }

    if(need_update == true)
    {
        _last_tm = cur_tm;
    }

    if(cur_s - _last_s > 1)
    {
        Log("定时间隔超过1秒。");
    }

    _last_s = cur_s;
    _last_ms = cur_ms;

    return ret;
}

std::pair<string, string> TimeOp::date_time()
{
    onUpdate();

    //GCC5 才支持 std::put_time
//    string fmt_date = "";
//    string fmt_time = "";
//
//    std::stringstream sst;
//    sst << std::put_time(&_last_tm ,"%Y:%m:%d");
//    fmt_date = sst.str();
//
//    sst.clear();
//    sst.str("");
//    sst << std::put_time(&_last_tm, "%H:%M:%S");
//    fmt_time = sst.str();
//
//    return make_pair(fmt_date, fmt_time);

    bzero(_datebuf, sizeof(_datebuf));
    strftime(_datebuf, sizeof(_datebuf), "%Y:%m:%d", &_last_tm);
    bzero(_timebuf, sizeof(_timebuf));
    strftime(_timebuf, sizeof(_timebuf), "%Y:%m:%d", &_last_tm);

    return make_pair(_datebuf, _timebuf);
}

string TimeOp::date()
{
    onUpdate();

//    string fmt_date = "";
//    std::stringstream sst;
//    sst << std::put_time(&_last_tm ,"%Y:%m:%d");
//    fmt_date = sst.str();
//    return fmt_date;

    bzero(_datebuf, sizeof(_datebuf));
    strftime(_datebuf, sizeof(_datebuf), "%Y:%m:%d", &_last_tm);

    return _datebuf;
}

time_t TimeOp::nowt()const
{
    return (time_t)_last_s;
}

string TimeOp::time2date(time_t t)
{
//    std::tm cur_tm = *std::localtime(&t);
//    std::stringstream sst;
//    sst << std::put_time(&cur_tm ,"%Y:%m:%d");
//    return sst.str();

    std::tm cur_tm = *std::localtime(&t);
    bzero(_timebuf, sizeof(_timebuf));
    strftime(_timebuf, sizeof(_timebuf), "%Y:%m:%d", &cur_tm);

    return _timebuf;
}




