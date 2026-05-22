#pragma once
#include "FilterBase.h"

namespace ip {
	class ThresholdFilter : public FilterBase
	{

	public:
		ThresholdFilter(int threshold) {
			m_threshold = threshold;
		}
		ThresholdFilter() {
			m_threshold = DEFAULT_THRESHOLD;
		}

		std::string GetFilterName() const override;
		std::string GetFilterParams() const override;

	protected:
		int m_threshold = 0;
		void processWork(ip::ImageBuffer& img, int startY, int endY) override;
	};
}