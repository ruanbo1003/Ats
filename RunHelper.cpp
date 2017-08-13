/*
 * RunHelper.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ruanbo
 */

#include "RunHelper.hpp"
#include <sys/resource.h>

RunHelper::RunHelper()
{
    _ats = AtsPtr(new Ats());
}

RunHelper::~RunHelper()
{
}

bool RunHelper::system_init()
{
    {
        struct rlimit core_limit;
        core_limit.rlim_cur = RLIM_INFINITY;
        core_limit.rlim_max = RLIM_INFINITY;

        if(setrlimit(RLIMIT_CORE, &core_limit) != 0)
        {
            return false;
        }
    }


    {
        struct rlimit file_limit;
        file_limit.rlim_cur = RLIM_INFINITY;
        file_limit.rlim_max = RLIM_INFINITY;
        if(setrlimit(RLIMIT_FSIZE, &file_limit) != 0)
        {
            return false;
        }
    }

    return true;
}

bool RunHelper::init()
{
    if(system_init() == false)
    {
        LogError("system_init failed");
        return false;
    }

    Log("main thread id:%ld\n", gettid());

    if(_ats->init() == false)
    {
        LogError("Ats init failed");
        return false;
    }

    return true;
}

void RunHelper::run()
{
    _ats->run();

    std::string input = "";
    while(true)
    {
      std::cin >> input;
      if(input == "quit" || input == "QUIT")
      {
          break;
      }
      else
      {
          Log("unknow cmd:%s", input.data());
      }
//      sleep(1);
      usleep(10000);
    }

    _ats->stop();

    _ats->join();
}

