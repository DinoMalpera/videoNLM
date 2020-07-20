#pragma once

#include "../NLMcommon.h"
#include "../Pixel_Coord.h"
#include "Pixel_Range_Iterator.h"
#include "../FrameSize.h"
#include "../NonCopyable.h"

namespace VNLM
{

    /*  Iterates over every pixel in the patch.
    *
    *   Handles cases when the patch crosses frame border.
    */
    class Every_Pixel_in_a_Patch : private NonCopyable
    {
    private:
        class iterator
        {
        public:
            constexpr
            explicit
            iterator(
                            Pixel_Range_Iterator     pt,
                    const   Every_Pixel_in_a_Patch&  swr_data
            ) noexcept
                :   it          (pt)
                ,   swr_data    (swr_data)
            {
            }
        public:
            constexpr
            const iterator&
            operator*() const noexcept
            {
                return *this;
            }
        public:
            constexpr
            iterator&
            operator++() noexcept
            {
                ++it;
                
                return *this;
            }
        public:
            constexpr
            bool
            operator!=(
                    const iterator& _end ) const noexcept
            {
                return it!=_end.it;
            }
        public:
            unsigned int
            getX() const noexcept
            {
                const unsigned int ret = remap_local_to_global_and_wrap(
                    it.getX(),
                    swr_data.radius,
                    swr_data.center_point.x,
                    swr_data.frameSize.size_x );
                VNLM_ASSERT( ret < swr_data.frameSize.size_x )
                return ret;
            }
        public:
            unsigned int
            getY() const noexcept
            {
                const unsigned int ret = remap_local_to_global_and_wrap(
                    it.getY(),
                    swr_data.radius,
                    swr_data.center_point.y,
                    swr_data.frameSize.size_y );
                VNLM_ASSERT( ret < swr_data.frameSize.size_y )
                return ret;
            }
        public:
            Pixel_Coord
            getPixelCoord() const noexcept
            {
                return { getX(), getY() };
            }
            
        private:
            /*  Checks whether the point is outside the frame border.
            *   Any border; upper or lower. 
            */
            static
            bool 
            is_border_case(
                    const unsigned int _p,
                    const unsigned int frame_max ) noexcept
            {
                return _p >= frame_max;
            }
        private:
            /*  Checks whether the point is below the lower frame border specifically.
            */
            static 
            bool 
            is_lower_border(
                    const unsigned int _p,
                    const unsigned int frame_max_2 ) noexcept
            {
                return _p > frame_max_2;
            }
        private:
            /*  Takes care of the case when the point is outside the frame border.
            */
            static
            unsigned int
            remap_local_to_global_and_wrap(
                    const unsigned int p,
                    const unsigned int radius,
                    const unsigned int center_point,
                    const unsigned int frame_max ) noexcept
            {
                const unsigned int _p = center_point - radius + p;
                if ( is_border_case( _p, frame_max ) )
                {
                    if ( const unsigned int frame_max_2 = 2 * frame_max;
                         is_lower_border( _p, frame_max_2 ) )
                    {
                        return -_p;
                    }
                    else
                    {
                        return frame_max_2 - _p - 2;
                    }
                }
    
                return _p;
            }
            
        private:
                    Pixel_Range_Iterator    it;
            const   Every_Pixel_in_a_Patch& swr_data;
        };
    public:
        constexpr
        explicit
        Every_Pixel_in_a_Patch(
                const   Pixel_Coord     center_point,
                const   unsigned int    radius,
                const   FrameSize&      frameSize
        ) noexcept
            :   center_point    (center_point)
            ,   radius          (radius)
            ,   frameSize       (frameSize)
        {
            VNLM_ASSERT( radius < frameSize.size_x )
            VNLM_ASSERT( radius < frameSize.size_y )
        }
    public:
        constexpr
        iterator
        begin() const noexcept
        {
            const unsigned int range = 2 * radius + 1;
            return iterator{    Pixel_Range_Iterator{ 0, 0, range },
                                *this };
        }
    public:
        constexpr
        iterator
        end() const noexcept
        {
            const unsigned int range = 2 * radius + 1;
            return iterator{    Pixel_Range_Iterator{ range, range, range },
                                *this };
        }
    
    private:
        const   Pixel_Coord     center_point;
        const   unsigned int    radius;
        const   FrameSize&      frameSize;
    };

}

