#include <gtest/gtest.h>
#include <src/range/Frame_Pixel_Range.h>
#include "Test_Utils.h"

using namespace VNLM;

static
auto
test_params(
        const unsigned int x_max,
        const unsigned int y_max )
{
    const FrameSize frameSize{ x_max, y_max };

    return std::tuple( x_max, y_max, frameSize );
}

TEST( FramePixelRangeTest, Basics)
{
    const auto [ x_max, y_max, frameSize ] = test_params( 5U, 3U );
    const Frame_Pixel_Range frame_pixel_range( frameSize );
    
    const auto b = frame_pixel_range.begin();
    const auto e = frame_pixel_range.end();

    ASSERT_EQ( b.getX(), 0 );
    ASSERT_EQ( b.getY(), 0 );
    ASSERT_EQ( e.getX(), 0 );
    ASSERT_EQ( e.getY(), y_max );

    ASSERT_EQ( b!=e, true );
    ASSERT_EQ( e!=e, false );
}

TEST( FramePixelRangeTest, Operators)
{
    const auto [ x_max, y_max, frameSize ] = test_params( 5U, 3U );
    const Frame_Pixel_Range frame_pixel_range( frameSize );

    const auto b = frame_pixel_range.begin();
    const auto e = frame_pixel_range.end();

    auto p = b;

    p!=e;
    ASSERT_EQ( p.getX(), 0 );
    ASSERT_EQ( p.getY(), 0 );
    ++p;
    ASSERT_EQ( p.getX(), 1 );
    ASSERT_EQ( p.getY(), 0 );
}
 
TEST( FramePixelRangeTest, Loop)
{
    const auto [ x_max, y_max, frameSize ] = test_params( 5U, 3U );
    const Frame_Pixel_Range frame_pixel_range( frameSize );

    const auto b = frame_pixel_range.begin();
    const auto e = frame_pixel_range.end();

    auto p = b;
    unsigned int j = 0;
    unsigned int i = 0;
    for(    ; j<y_max; ++j)
    for( i=0; i<x_max; ++i)
    {
        const unsigned int ti = p.getX();
        const unsigned int tj = p.getY();
        ASSERT_EQ( ti, i );
        ASSERT_EQ( tj, j );
        
        ++p;
        
        ASSERT_TRUE( loop_end_condition( p, e, i, j, x_max, y_max ) );
    }
}

