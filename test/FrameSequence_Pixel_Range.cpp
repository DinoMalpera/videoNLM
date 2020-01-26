#include <gtest/gtest.h>
#include <src/range/FrameSequence_Pixel_Range.h>
#include "Test_Utils.h"

using namespace VNLM;

static
auto
test_params(
		const unsigned int	x_max,
		const unsigned int	y_max,
		const unsigned int	sequence_length,
		const unsigned int	search_window_radius,
		const Pixel_Coord	window_center )
{
	const FrameSize frameSize{ x_max, y_max };
	const FrameSequence_Pixel_Range fpr{ frameSize, sequence_length, search_window_radius, window_center };

	return std::tuple( x_max, y_max, fpr );
}

TEST( FrameSequencePixelRangeTest, Basics)
{
	const auto [ x_max, y_max, frameSequence_pixel_range ] = test_params( 10U, 8U, 1, 9999U, {0,0} );
	
	const auto b = frameSequence_pixel_range.begin();
	const auto e = frameSequence_pixel_range.end();
	
	const auto beginFrame = b.getFrame();
	ASSERT_EQ( beginFrame, 0 );
	const auto endFrame = e.getFrame();
	ASSERT_EQ( endFrame, 1 );
	
	const auto b_it = b.getPixelCoord();
	const auto e_it = e.getPixelCoord();

	ASSERT_EQ( b_it.x, 0 );
	ASSERT_EQ( b_it.y, 0 );
	ASSERT_EQ( e_it.x, x_max );
	ASSERT_EQ( e_it.y, y_max );

	ASSERT_EQ( b!=e, true );
	ASSERT_EQ( e!=e, false );
}

TEST( FrameSequencePixelRangeTest, Loop)
{
	const auto [ x_max, y_max, frameSequence_pixel_range ] = test_params( 10U, 8U, 2, 9999U, {0,0} );

	const auto b = frameSequence_pixel_range.begin();
	const auto e = frameSequence_pixel_range.end();

	const auto beginFrame = b.getFrame();
	ASSERT_EQ( beginFrame, 0 );
	const auto endFrame = e.getFrame();
	ASSERT_EQ( endFrame, 2 );
	
	auto p = b;
	unsigned int j = 0;
	unsigned int i = 0;
	for(    ; j<y_max; ++j)
	for( i=0; i<x_max; ++i)
	{
		const auto p_it = p.getPixelCoord();
		const unsigned int ti = p_it.x;
		const unsigned int tj = p_it.y;
		ASSERT_EQ( ti, i );
		ASSERT_EQ( tj, j );

		++p;
		
		ASSERT_TRUE( p!=e );
	}
	
	j = 0;
	for(    ; j<y_max; ++j)
	for( i=0; i<x_max; ++i)
	{
		const auto p_it = p.getPixelCoord();
		const unsigned int ti = p_it.x;
		const unsigned int tj = p_it.y;
		ASSERT_EQ( ti, i );
		ASSERT_EQ( tj, j );

		++p;

		ASSERT_TRUE( loop_end_condition( p, e, i, j, x_max, y_max ) );
	}
}

TEST( FrameSequencePixelRangeTest, LoopWithSearchWindow)
{
	const auto search_window_size = 3U;
	const Pixel_Coord window_center{ 5U, 4U };
	const auto [ x_max, y_max, frameSequence_pixel_range ] = test_params(
		10U, 8U, 2, search_window_size, window_center );

	const auto b = frameSequence_pixel_range.begin();
	const auto e = frameSequence_pixel_range.end();

	const auto beginFrame = b.getFrame();
	ASSERT_EQ( beginFrame, 0 );
	const auto endFrame = e.getFrame();
	ASSERT_EQ( endFrame, 2 );
	
	const unsigned int j_end = window_center.y+search_window_size+1;
	const unsigned int i_end = window_center.x+search_window_size+1;
	
	auto p = b;
	unsigned int j = 0;
	unsigned int i = 0;
	for( j=window_center.y-search_window_size; j<j_end; ++j)
	for( i=window_center.x-search_window_size; i<i_end; ++i)
	{
		const auto p_it = p.getPixelCoord();
		const unsigned int ti = p_it.x;
		const unsigned int tj = p_it.y;
		ASSERT_EQ( ti, i );
		ASSERT_EQ( tj, j );

		++p;
		
		ASSERT_TRUE( p!=e );
	}
	
	for( j=window_center.y-search_window_size; j<j_end; ++j)
	for( i=window_center.x-search_window_size; i<i_end; ++i)
	{
		const auto p_it = p.getPixelCoord();
		const unsigned int ti = p_it.x;
		const unsigned int tj = p_it.y;
		ASSERT_EQ( ti, i );
		ASSERT_EQ( tj, j );

		++p;
		
		ASSERT_TRUE( loop_end_condition( p, e, i, j, i_end, j_end ) );
	}
}

TEST( FrameSequencePixelRangeTest, LoopWithSearchWindowAtBorder)
{
	const auto search_window_size = 3U;
	const Pixel_Coord window_center{ 8U, 0U };
	const auto [ x_max, y_max, frameSequence_pixel_range ] = test_params(
		10U, 8U, 2, search_window_size, window_center );

	const auto b = frameSequence_pixel_range.begin();
	const auto e = frameSequence_pixel_range.end();

	const auto beginFrame = b.getFrame();
	ASSERT_EQ( beginFrame, 0 );
	const auto endFrame = e.getFrame();
	ASSERT_EQ( endFrame, 2 );
	
	const unsigned int j_end = window_center.y+search_window_size+1;
	
	auto p = b;
	unsigned int j = 0;
	unsigned int i = 0;
	for( j=0;                                  j<j_end; ++j)
	for( i=window_center.x-search_window_size; i<x_max; ++i)
	{
		const auto p_it = p.getPixelCoord();
		const unsigned int ti = p_it.x;
		const unsigned int tj = p_it.y;
		ASSERT_EQ( ti, i );
		ASSERT_EQ( tj, j );

		++p;
		
		ASSERT_TRUE( p!=e );
	}
	
	for( j=0;                                  j<j_end; ++j)
	for( i=window_center.x-search_window_size; i<x_max; ++i)
	{
		const auto p_it = p.getPixelCoord();
		const unsigned int ti = p_it.x;
		const unsigned int tj = p_it.y;
		ASSERT_EQ( ti, i );
		ASSERT_EQ( tj, j );

		++p;
		
		ASSERT_TRUE( loop_end_condition( p, e, i, j, x_max, j_end ) );
	}
}

