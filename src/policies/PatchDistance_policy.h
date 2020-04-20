#pragma once

#include "../Frame.h"
#include "src/range/Every_Pixel_in_a_Patch.h"
#include "../FrameSequence.h"
#include "../NLMparams.h"
#include "DistanceFunction_policy.h"

namespace VNLM
{

    template <typename Pixel_Value_policy, typename DistanceFunction>
    struct Element2Element
    {
    public:
        double
        operator()(
                const Pixel_Range_Iterator&                 ix,
                const Every_Pixel_in_SearchWindow_of_Every_Frame::iterator&  ix_s,
                const FrameSequence<Pixel_Value_policy>&    frameSequence,
                const NLMparams&                            params ) const
        {
                    double                      acc             = 0.0;
            const   Frame<Pixel_Value_policy>&  central_frame   = frameSequence.get_center_frame();
            const   Frame<Pixel_Value_policy>&  target_frame    = frameSequence.get_frame( ix_s.getFrame() );
            const   unsigned int                search_radius   = params.patch_radius;
            const   FrameSize&                  frameSize       = frameSequence.getFrameSize();
        
            Every_Pixel_in_a_Patch central_frame_iterator(
                    ix.getPixelCoord(),
                    search_radius,
                    frameSize );
            Every_Pixel_in_a_Patch all_frames_iterator(
                    ix_s.getPixelCoord(),
                    search_radius,
                    frameSize );
        
            auto it_central = central_frame_iterator.begin();
            auto it_all = all_frames_iterator.begin();
        
            for( ;  it_central != central_frame_iterator.end(); ++it_central, ++it_all )
            {
                VNLM_ASSERT( it_all != all_frames_iterator.end() )  // it_central and it_all should be in perfect sync
            
                acc += DistanceFunction()(
                            central_frame[ it_central.getPixelCoord() ],
                            target_frame [ it_all.getPixelCoord() ] );
            }
            VNLM_ASSERT( false == (it_all != all_frames_iterator.end()) )   // it_central and it_all should be in perfect sync
        
            const unsigned int cntr = sqr(2 * search_radius + 1);
            acc /= cntr;
        
            return acc;
        }
    };

}

