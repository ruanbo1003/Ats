/*
 * BufferTest.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ruanbo
 */


#include "comm/comm.hpp"
#include "utils/Buffer.hpp"


void buffer_tests()
{
    BufferPtr bp = BufferPtr(new Buffer());

    Log("bp size():%ld", bp->size());

    bp->append();

    Log("bp size():%ld", bp->size());
}



