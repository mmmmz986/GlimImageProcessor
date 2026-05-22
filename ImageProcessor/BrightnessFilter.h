#pragma once

#include "FilterBase.h"

namespace ip {
	class BirghtnessFilter : public FilterBase
	{

	public:
		BirghtnessFilter(int birghtness) {
			m_brightness = birghtness;
		}
		BirghtnessFilter() {
			m_brightness = DEFAULT_BRIGHTNESS;
		}

		std::string GetFilterName() const override;
		std::string GetFilterParams() const override;

	protected:
		int m_brightness = 0;
		void processWork(ip::ImageBuffer& img, int startY, int endY) override;
	};
}