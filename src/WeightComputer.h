#pragma once

#include "NLMparams.h"
#include "range/Pixel_Range_Iterator.h"
#include "range/Every_Pixel_in_SearchWindow_of_Every_Frame.h"
#include "Policy_Contracts.h"

namespace VNLM
{

    // forward declarations
    template <ComputableColor>
    class FrameSequence;
    
    namespace WeightComputer
    {
    
        template<   ComputableColor Pixel_Value_policy,
                    typename PatchDistance_policy,
                    typename... Policies>
        static
        double
        computePatch(
                const Pixel_Range_Iterator&                                 ix,
                const Every_Pixel_in_SearchWindow_of_Every_Frame::iterator& ix_s,
                const FrameSequence<Pixel_Value_policy>&                    frameSequence,
                const NLMparams&                                            params) noexcept
        {
            return PatchDistance_policy()(
                        ix,
                        ix_s,
                        frameSequence,
                        params);
        }
        
        template<   ComputableColor Pixel_Value_policy,
                    typename DistributionFunction_policy,
                    typename... Policies>
        [[nodiscard]]
        static
        double
        weight(
                const Pixel_Range_Iterator&                                 ix,
                const Every_Pixel_in_SearchWindow_of_Every_Frame::iterator& ix_s,
                const FrameSequence<Pixel_Value_policy>&                    frameSequence,
                const NLMparams&                                            params) noexcept
        {
            return  DistributionFunction_policy()(
                        computePatch<Pixel_Value_policy,Policies...>(
                            ix,
                            ix_s,
                            frameSequence,
                            params),
                        params
                        );
        }
    }

}

