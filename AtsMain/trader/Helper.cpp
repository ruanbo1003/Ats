/*
 * Helper.cpp
 *
 *  Created on: Mar 15, 2017
 *      Author: ruanbo
 */


#include "AtsTrader.hpp"



const char* AtsTrader::onCC(char* pSrc, size_t len)
{
    if(len <= 0)
    {
        len = strlen(pSrc) + 1;
    }

    return _cc->convert2(pSrc, len);
}


