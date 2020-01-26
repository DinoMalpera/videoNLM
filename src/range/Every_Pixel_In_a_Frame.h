#pragma once

#include "../FrameSize.h"
#include "../NonCopyable.h"
#include "Pixel_Range_Iterator.h"

namespace VNLM
{

	class Every_Pixel_In_a_Frame : private NonCopyable
	{
	public:
		Every_Pixel_In_a_Frame(
				const FrameSize& _frameSize
		)
			:	frameSize	(_frameSize)
		{
		}
	public:
		constexpr
		Pixel_Range_Iterator
		begin() const noexcept
		{
			return { 0U, 0U, frameSize.size_x };
		}
	public:
		constexpr
		Pixel_Range_Iterator
		end() const noexcept
		{
			return {	0U,
						frameSize.size_y,
						frameSize.size_x };
		}
	
	private:
		const FrameSize&	frameSize;
	};

}
