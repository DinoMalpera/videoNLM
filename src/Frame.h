#pragma once

#include "NLMcommon.h"
#include "Pixel_Coord.h"
#include "FrameSize.h"

namespace VNLM
{

	/*	Frame container. It holds (but not owns) data
	 * 	for a single image (single frame). Depending on
	 * 	the pixel_value_type policy colors can be expressed
	 * 	in different color spaces.
	 * */
	template <typename pixel_value_type>
	class Frame
	{
	private:
		using PixelArray		= pixel_value_type*;
	
	// interface
	
	public:
		Frame(
				const PixelArray	data,
				const FrameSize&	frameSize,
				const unsigned int	frame
		)
			:	data		(data)
			,	frameSize	(&frameSize)
			,	frame		(frame)
		{
			VNLM_ASSERT( nullptr == data )
		}
	public:
		pixel_value_type&
		operator[](
				const Pixel_Coord ix ) noexcept
		{
			const auto pos = compute_index(ix);
			VNLM_ASSERT( pos < (frameSize->size_x*frameSize->size_y) )
			return data[ pos ];
		}
	public:
		pixel_value_type
		operator[](
				const Pixel_Coord ix ) const noexcept
		{
			const auto pos = compute_index(ix);
			VNLM_ASSERT( pos < (frameSize->size_x*frameSize->size_y) )
			return data[ pos ];
		}
	public:
		const FrameSize&
		getFrameSize() const noexcept
		{
			return *frameSize;
		}
		
	// methods
	
	private:
		constexpr
		size_t
		compute_index(
				const Pixel_Coord& pt ) const noexcept
		{
			VNLM_ASSERT( pt.x < frameSize->size_x )
			return pt.y * frameSize->size_x + pt.x;
		}
		
	// members
	
	private:
				PixelArray		data;
		const	FrameSize*		frameSize;
				unsigned int	frame;
	};

}

