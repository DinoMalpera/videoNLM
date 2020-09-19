#pragma once

#include "../NLMutils.h"

namespace VNLM
{

    float
    norm_sqr(
        const float _ ) noexcept
    {
        return sqr(_);
    }
    
    template <typename T>
    struct NormL2
    {
        constexpr
        float
        operator()(
                const T v1,
                const T v2) const noexcept
        {
            return norm_sqr(v1-v2);
        }
    };

}
