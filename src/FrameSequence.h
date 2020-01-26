#pragma once

#include <boost/circular_buffer.hpp>
#include "NLMcommon.h"
#include "Frame.h"
#include "range/Every_Pixel_in_SearchWindow_of_Every_Frame.h"

namespace VNLM
{

    /*  Holds successive frames. One of the frames is the one
     *  we want to denoise (maybe the one in the middle - center frame).
     *  As a frame is added to the sequence
     *  All frames (images) should be of the same size.
     * */
    template <typename Pixel_Value_policy>
    class FrameSequence
    {
        using FrameArray = boost::circular_buffer<Frame<Pixel_Value_policy>>;
    
    public:
        FrameSequence(
                const unsigned int size_of_sequence )
            :   frames  (size_of_sequence)
            ,   center_frame_index  ( size_of_sequence / 2U )
        {}
    public:
        void
        push_back(
                const Frame<Pixel_Value_policy>& fr )
        {
            VNLM_ASSERT( frames.size() ? (fr.getFrameSize()==frames[0].getFrameSize()) : true )
            frames.push_back( fr );
        }
    public:
        const Frame<Pixel_Value_policy>&
        get_center_frame() const noexcept
        {
            VNLM_ASSERT( center_frame_index < frames.size() )
            return frames[center_frame_index];
        }
    public:
        void
        set_center_frame_index(
                const unsigned int _center_frame_index ) noexcept
        {
            center_frame_index = _center_frame_index;
        }
    public:
        unsigned int
        get_center_frame_index() const noexcept
        {
            return center_frame_index;
        }
    public:
        const Frame<Pixel_Value_policy>&
        get_frame(
                const unsigned int ix ) const noexcept
        {
            VNLM_ASSERT( ix < frames.size() )
            return frames[ix];
        }
    public:
        unsigned int
        get_sequence_size() const noexcept
        {
            return frames.size();
        }
    public:
        auto
        operator[](
                const Every_Pixel_in_SearchWindow_of_Every_Frame::iterator& it ) const noexcept
        {
            VNLM_ASSERT( it.getFrame() < frames.size() )
            VNLM_ASSERT( (it.getPixelCoord().y*frames[it.getFrame()].getFrameSize().size_x + it.getPixelCoord().x)
                        <
                        (frames[it.getFrame()].getFrameSize().size_x*frames[it.getFrame()].getFrameSize().size_y) )
            return frames   [ it.getFrame() ]
                            [ it.getPixelCoord() ];
        }
    public:
        FrameSize
        getFrameSize() const noexcept
        {
            VNLM_ASSERT( 0 < frames.size() )
            return frames[0].getFrameSize();
        }
    
    private:
        FrameArray      frames;
        unsigned int    center_frame_index;
    };

}

