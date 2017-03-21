/*
 * RunHelper.hpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ruanbo
 */

#ifndef RUNHELPER_HPP_
#define RUNHELPER_HPP_

#include <AtsMain/Ats.hpp>
#include "comm/comm.hpp"

class RunHelper : public NonCopyable
{
private:
    AtsPtr _ats;

public:
    RunHelper();
    virtual ~RunHelper();

public:
    bool system_init();

    bool init();

    void run();
};

using RunHelperPtr = tr1::shared_ptr<RunHelper>;



#endif /* RUNHELPER_HPP_ */
