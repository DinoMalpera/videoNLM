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

using namespace VNLM;

static
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

static
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

    if ( 0.0 >= params.filtering_parameter )
    {
        return false;
    }

    return true;
}

template <typename Pixel_Value_policy>
bool
NLMdenoiser::verify(
        const   FrameSequence<Pixel_Value_policy>&  frameSequence,
        const   NLMparams&                          params )
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

template <typename Pixel_Value_policy>
void
NLMdenoiser::Denoise(
        const   FrameSequence<Pixel_Value_policy>&  frameSequence,
        Frame<Pixel_Value_policy>&                  result,
        const NLMparams&                            params )
{
    if ( false == verify( frameSequence, params ) )
    {
        return;
    }

    denoise( frameSequence, result, params );
}

template <typename Pixel_Value_policy>
void
do_denoise(
        const   FrameSequence<Pixel_Value_policy>&  frameSequence,
        Frame<Pixel_Value_policy>&                  result,
        Pixel_Range_Iterator                        b,
        const Pixel_Range_Iterator                  e,
        const NLMparams&                            params)
{
    const unsigned int sequence_size = frameSequence.get_sequence_size();
    const FrameSize& frameSize = frameSequence.getFrameSize();
    
    for( auto ix = b; ix!=e; ++ix )
    {
        auto totalWeight    = 0.0;
        Pixel_Value_policy acc  = (Pixel_Value_policy)0.0;
        for( auto ix_s : Every_Pixel_in_SearchWindow_of_Every_Frame( frameSize, sequence_size, params.search_window_radius, ix.getPixelCoord() ) )
        {
            const double weight = WeightComputer::weight<
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

static
void
join_vector_of_threads(
        std::vector<std::thread>& threads )
{
    std::for_each(
        threads.begin(),
        threads.end(),
        [](auto& thr)
        {
            thr.join();
        } );
}

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

template <typename Pixel_Value_policy>
void
NLMdenoiser::denoise(
        const   FrameSequence<Pixel_Value_policy>&  frameSequence,
        Frame<Pixel_Value_policy>&                  result,
        const NLMparams&                            params )
{
    const FrameSize& frameSize = frameSequence.getFrameSize();
    const unsigned int numThreads = std::thread::hardware_concurrency();
    VNLM_ASSERT( 0U != numThreads )
    
    std::vector<std::thread> threads;
    
    const Naive_Work_Scheduler  work_scheduler( frameSize, numThreads );

    for( unsigned int thr=0; thr<numThreads; ++thr )
    {
        auto [ b, e ] = work_scheduler.get_range( thr );
        
        threads.emplace_back(
            std::thread(    do_denoise<Pixel_Value_policy>,
                            std::cref( frameSequence ),
                            std::ref(  result ),
                            b,
                            e,
                            params ));
    }
    
    join_vector_of_threads( threads );
}

// explicit template initializations for ColorSpace parameter

// grayscale

template
bool
NLMdenoiser::verify<Color_Space_Grayscale>(
        const FrameSequence<Color_Space_Grayscale>&,
        const NLMparams& );
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
        const NLMparams& );
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
        
        
