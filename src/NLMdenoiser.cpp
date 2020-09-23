#include "NLMdenoiser.h"
#include "range/Every_Pixel_In_a_Frame.h"
#include "WeightComputer.h"
#include "policies/DistributionFunction_policy.h"
#include "policies/PatchDistance_policy.h"
#include "policies/DistanceFunction_policy.h"
#include "Pixel_Value.h"

#include <thread>
#include <algorithm>
#include <vector>

#include "Naive_Work_Scheduler.h"

using namespace VNLM;

namespace {

bool
verify_radius(
        const unsigned int radius,
        const FrameSize&   frameSize )
{
    if ( 0U == radius )
    {
        return false;
    }

    if ( radius >= frameSize.size_x )
    {
        return false;
    }

    if ( radius >= frameSize.size_y )
    {
        return false;
    }

    return true;
}

bool
verify_params(
        const NLMparams& params,
        const FrameSize  frameSize )
{
    if ( false == verify_radius( params.patch_radius, frameSize ) )
    {
        return false;
    }

    if ( false == verify_radius( params.search_window_radius, frameSize ) )
    {
        return false;
    }

    if ( 0.0 > params.standard_deviation_of_noise )
    {
        return false;
    }

    return true;
}

template <ComputableColor Pixel_Value_policy>
void
do_denoise(
        const   FrameSequence<Pixel_Value_policy>&  frameSequence,
        Frame<Pixel_Value_policy>&                  result,
        Pixel_Range_Iterator                        b,
        const Pixel_Range_Iterator                  e,
        const NLMparams&                            params) noexcept
{
    const unsigned int sequence_size = frameSequence.get_sequence_size();
    const FrameSize&   frameSize     = frameSequence.getFrameSize();
    
    for( auto ix = b; ix!=e; ++ix )
    {
        // denoise the pixel ix

        auto totalWeight        = 0.0;
        Pixel_Value_policy acc  = (Pixel_Value_policy) 0.0;
        // iterate through all the pixels within the search window of every frame
        for( auto ix_s : Every_Pixel_in_SearchWindow_of_Every_Frame( frameSize, sequence_size, params.search_window_radius, ix.getPixelCoord() ) )
        {
            const double weight =
                WeightComputer::weight<
                    Pixel_Value_policy,
                    Gauss,
                    Element2Element<
                        Pixel_Value_policy,
                        NormL2<Pixel_Value_policy>>
                    >
                (
                    ix,
                    ix_s,
                    frameSequence,
                    params );
                    
            acc += frameSequence[ix_s] * weight;
            totalWeight += weight;
        }

        result[ix.getPixelCoord()] = acc / totalWeight;
    }
}

}

template <ComputableColor Pixel_Value_policy>
bool
NLMdenoiser::verify(
        const   FrameSequence<Pixel_Value_policy>&  frameSequence,
        const   NLMparams&                          params ) const
{
    if ( false == frameSequence.verify() )
    {
        return false;
    }

    if ( false == verify_params( params, frameSequence.getFrameSize() ) )
    {
        return false;
    }

    return true;
}

template <ComputableColor Pixel_Value_policy>
void
NLMdenoiser::Denoise(
        const   FrameSequence<Pixel_Value_policy>&  frameSequence,
        Frame<Pixel_Value_policy>&                  result,
        const   NLMparams&                          params )
{
    if ( false == verify( frameSequence, params ) )
    {
        return;
    }

    denoise( frameSequence, result, params );
}

template <ComputableColor Pixel_Value_policy>
void
NLMdenoiser::denoise(
        const   FrameSequence<Pixel_Value_policy>&  frameSequence,
        Frame<Pixel_Value_policy>&                  result,
        const NLMparams&                            params )
{
    const FrameSize& frameSize = frameSequence.getFrameSize();
    const unsigned int numThreads = std::thread::hardware_concurrency();
    VNLM_ASSERT( 0U != numThreads )
    
    std::vector<std::jthread> threads;
    
    const Naive_Work_Scheduler  work_scheduler( frameSize, numThreads );

    // assign work to threads

    for( unsigned int thr=0; thr<numThreads; ++thr )
    {
        auto [ b, e ] = work_scheduler.get_range( thr );
        
        threads.emplace_back(
            std::jthread(    do_denoise<Pixel_Value_policy>,
                            std::cref( frameSequence ),
                            std::ref(  result ),
                            b,
                            e,
                            params ));
    }
}

// explicit template initializations for ColorSpace parameter

// grayscale

template
bool
NLMdenoiser::verify<Color_Space_Grayscale>(
        const FrameSequence<Color_Space_Grayscale>&,
        const NLMparams& ) const;
template
void
NLMdenoiser::Denoise<Color_Space_Grayscale>(
        const FrameSequence<Color_Space_Grayscale>&,
        Frame<Color_Space_Grayscale>&,
        const NLMparams& );
template
void
do_denoise<Color_Space_Grayscale>(
        const FrameSequence<Color_Space_Grayscale>&,
        Frame<Color_Space_Grayscale>&,
        Pixel_Range_Iterator,
        Pixel_Range_Iterator,
        const NLMparams& );
template
void
NLMdenoiser::denoise<Color_Space_Grayscale>(
        const FrameSequence<Color_Space_Grayscale>&,
        Frame<Color_Space_Grayscale>&,
        const NLMparams& );
        
// RGB
        
template
bool
NLMdenoiser::verify<Color_Space_RGB>(
        const FrameSequence<Color_Space_RGB>&,
        const NLMparams& ) const;
template
void
NLMdenoiser::Denoise<Color_Space_RGB>(
        const FrameSequence<Color_Space_RGB>&,
        Frame<Color_Space_RGB>&,
        const NLMparams& );
template
void
do_denoise<Color_Space_RGB>(
        const FrameSequence<Color_Space_RGB>&,
        Frame<Color_Space_RGB>&,
        Pixel_Range_Iterator,
        Pixel_Range_Iterator,
        const NLMparams& );
template
void
NLMdenoiser::denoise<Color_Space_RGB>(
        const FrameSequence<Color_Space_RGB>&,
        Frame<Color_Space_RGB>&,
        const NLMparams& );
        
        
