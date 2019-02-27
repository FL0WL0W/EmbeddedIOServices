#include "Interpolation.h"

#ifdef INTERPOLATION_H
namespace Interpolation
{
	InterpolationResponse Interpolate(const float value, const float maxValue, const float minValue, const uint8_t resolution)
	{
		InterpolationResponse response = { 0, 0, 0 };
		if (resolution > 1)
		{
			const uint8_t resolutionMinus1 = resolution - 1;
			response.Multiplier = resolutionMinus1 * (value - minValue) / (maxValue - minValue);
			if(response.Multiplier < 0)
			{
				response.Multiplier = 0;
			}
			response.IndexL = static_cast<uint8_t>(response.Multiplier);
			response.Multiplier -= response.IndexL;
			response.IndexH = response.IndexL + 1;
			if (response.IndexL > resolutionMinus1)
			{
				response.IndexL = response.IndexH = resolutionMinus1;
			}
			else if (response.IndexH > resolutionMinus1)
			{
				response.IndexH = resolutionMinus1;
			}
		}

		return response;
	}
}
#endif
