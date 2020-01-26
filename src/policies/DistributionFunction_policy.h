#pragma once

#include "../NLMparams.h"
#include <cmath>
#include <algorithm>

namespace VNLM
{

	struct Step
	{
		static
		double 
		compute(
				const double x,
				const NLMparams& params )
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
		static
		double 
		compute(
				const double x,
				const NLMparams& params )
		{
			return
				exp(
					- std::max( x - 2 * params.standard_deviation_of_noise, 0.0 )
					/
					params.filtering_parameter );
		}
	};

}

