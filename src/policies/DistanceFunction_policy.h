#pragma once

#include "../NLMutils.h"

namespace VNLM
{

	float
	norm_sqr(
		const float _ )
	{
		return sqr(_);
	}
	
	template <typename T>
	struct NormL2
	{
		static
		constexpr
		float
		compute(
				const T v1,
				const T v2) noexcept
		{
			return norm_sqr(v1-v2);
		}
	};

}
