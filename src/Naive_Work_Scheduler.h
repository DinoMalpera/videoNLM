#pragma once

#include "FrameSize.h"

namespace VNLM
{

	class Naive_Work_Scheduler
	{
	public:
	    Naive_Work_Scheduler(
		    const FrameSize&    _frameSize,
		    const unsigned int  _numThreads
	    )
		:   frameSize       (_frameSize)
		,   rows_per_thread (_frameSize.size_y / _numThreads)
		,   last_row        (_numThreads - 1U)
	    {
	    }
	public:
            /*  Given a thread index, computes the range of frame pixels
            *   that are assigned to that thread.
            */
	    auto
	    get_range(
		    const unsigned int thr ) const
	    {
		const   Every_Pixel_In_a_Frame fpr(frameSize);
		        Pixel_Range_Iterator b =    fpr.begin() + ((rows_per_thread * thr)*frameSize.size_x);
		const   Pixel_Range_Iterator e =    (thr == last_row)
		                                    ?
		                                    fpr.end()
		                                    :
		                                    (fpr.begin() + ((rows_per_thread * (thr+1))*frameSize.size_x));
		
		return std::tuple( b, e );
	    }
	private:
	    const FrameSize&    frameSize;
	    const unsigned int  rows_per_thread;
	    const unsigned int  last_row;
	};

}

