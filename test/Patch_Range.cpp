#include <gtest/gtest.h>
#include <src/range/Patch_Range.h>
#include "Test_Utils.h"

using namespace VNLM;

static
auto
test_params(
	const unsigned int x_c,
	const unsigned int y_c,
	const unsigned int x_max,
	const unsigned int y_max,
	const unsigned int radius )
{
	const FrameSize frameSize{ x_max, y_max };

	return std::tuple( x_c, y_c, x_max, y_max, radius, frameSize );
}

TEST( PatchRangeTest, Basics)
{
	const auto [ x_c, y_c, x_max, y_max, radius, frameSize ]
		= test_params( 5U, 3U, 10U, 8U, 3U );
	const Patch_Range patch_range(
		Pixel_Coord{ x_c, y_c },
		radius,
		frameSize );

	const auto b = patch_range.begin();
	const auto e = patch_range.end();

	ASSERT_EQ( b.getX(), x_c-radius );
	ASSERT_EQ( b.getY(), y_c-radius );
	ASSERT_EQ( e.getX(), x_c+radius+1 );
	ASSERT_EQ( e.getY(), y_c+radius+1 );

	ASSERT_EQ( b!=e, true );
	ASSERT_EQ( e!=e, false );
}

TEST( PatchRangeTest, Operators)
{
	const auto [ x_c, y_c, x_max, y_max, radius, frameSize ]
		= test_params( 5U, 3U, 10U, 8U, 3U );
	const Patch_Range patch_range(
		Pixel_Coord{ x_c, y_c },
		radius,
		frameSize );

	      auto p = patch_range.begin();
	const auto e = patch_range.end();

	p!=e;
	ASSERT_EQ( p.getX(), x_c-radius );
	ASSERT_EQ( p.getY(), y_c-radius );
	++p;
	ASSERT_EQ( p.getX(), x_c-radius+1 );
	ASSERT_EQ( p.getY(), y_c-radius );
}

TEST( PatchRangeTest, LoopBasic)
{
	const auto [ x_c, y_c, x_max, y_max, radius, frameSize ]
		= test_params( 5U, 3U, 10U, 8U, 3U );
	const Patch_Range patch_range(
		Pixel_Coord{ x_c, y_c },
		radius,
		frameSize );

	      auto p = patch_range.begin();
	const auto e = patch_range.end();
	
	const unsigned int j_end = (y_c+radius+1);
	const unsigned int i_end = (x_c+radius+1);

	unsigned int j = y_c-radius;
	unsigned int i = x_c-radius;
	for(             ; j<j_end; ++j)
	for( i=x_c-radius; i<i_end; ++i)
	{
		ASSERT_EQ( p.getX(), i );
		ASSERT_EQ( p.getY(), j );
		
		++p;
		
		ASSERT_TRUE( loop_end_condition( p, e, i, j, i_end, j_end ) );
	}
}

TEST( PatchRangeTest, LoopCrossingLeftUp)
{
	const auto [ x_c, y_c, x_max, y_max, radius, frameSize ]
		= test_params( 1U, 1U, 10U, 8U, 3U );
	const Patch_Range patch_range(
		Pixel_Coord{ x_c, y_c },
		radius,
		frameSize );

	      auto p = patch_range.begin();
	const auto e = patch_range.end();
	
	const unsigned int j_end = (y_c+radius+1);
	const unsigned int i_end = (x_c+radius+1);

	int j = (int)y_c-(int)radius;
	int i = (int)x_c-(int)radius;
	for(                       ; j<j_end; ++j)
	for( i=(int)x_c-(int)radius; i<i_end; ++i)
	{
		ASSERT_EQ( p.getX(), abs(i) );
		ASSERT_EQ( p.getY(), abs(j) );
		
		++p;
		
		ASSERT_TRUE( loop_end_condition( p, e, i, j, i_end, j_end ) );
	}
}

TEST( PatchRangeTest, LoopCrossingRightBottom)
{

	const auto [ x_c, y_c, x_max, y_max, radius, frameSize ]
		= test_params( 9U, 6U, 10U, 8U, 3U );
	const Patch_Range patch_range(
		Pixel_Coord{ x_c, y_c },
		radius,
		frameSize );

	      auto p = patch_range.begin();
	const auto e = patch_range.end();
	
	const unsigned int j_end = (y_c+radius+1);
	const unsigned int i_end = (x_c+radius+1);

	unsigned int j = y_c-radius;
	unsigned int i = x_c-radius;
	for(             ; j<j_end; ++j)
	for( i=x_c-radius; i<i_end; ++i)
	{
		ASSERT_EQ( p.getX(), ((i<x_max) ? i : (x_max-(i-x_max)-2)) );
		ASSERT_EQ( p.getY(), ((j<y_max) ? j : (y_max-(j-y_max)-2)) );
		
		++p;
		
		ASSERT_TRUE( loop_end_condition( p, e, i, j, i_end, j_end ) );
	}
}


