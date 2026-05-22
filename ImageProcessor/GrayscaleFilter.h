#pragma once

#include "FilterBase.h"

namespace ip {
	class GrayscaleFilter : public FilterBase
	{

	public:
		GrayscaleFilter() = default;

		std::string GetFilterName() const override;
		std::string GetFilterParams() const override;

	protected:
		void processWork(ip::ImageBuffer& img, int startY, int endY) override;
	};
}