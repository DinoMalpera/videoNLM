#pragma once

#include "../NLMparams.h"
#include <cmath>
#include <algorithm>

namespace VNLM
{

    struct Step
    {
        double 
        operator()(
                const double x,
                const NLMparams& params ) const
        {
            if ( x < params.standard_deviation_of_noise )
            {
                return 1.0;
            }
            return 0.0;
        }
    };
    
    struct Gauss
    {
        double 
        operator()(
                const double x,
                const NLMparams& params ) const
        {
            return
                exp(
                    - std::max( x - 2 * params.standard_deviation_of_noise, 0.0 )
                    /
                    params.filtering_parameter );
        }
    };

}

