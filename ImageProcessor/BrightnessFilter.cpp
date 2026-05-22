#include "BrightnessFilter.h"
#include <algorithm>

namespace ip {
	std::string BirghtnessFilter::GetFilterName() const
	{
		return "BrightnessFilter";
	}

	std::string BirghtnessFilter::GetFilterParams() const
	{

		return "Width=" + std::to_string(m_params.width) +
			", Height=" + std::to_string(m_params.height) +
			", RowStride=" + std::to_string(m_params.rowStride) +
			", DataSize=" + std::to_string(m_params.dataSize) +
			", Channels=" + std::to_string(m_params.channels) +
			", Thread=" + std::to_string(m_params.threadCount);
	}

	void BirghtnessFilter::processWork(ImageBuffer& img, int startY, int endY)
	{
		int width = img.width();
		int height = img.height();
		int rowStride = img.rowStride();
		size_t dataSize = img.dataSize();
		int channels = img.CHANNELS;
		int brightness = m_brightness;

		uint8_t* firstPtr = img.data(); // img.rowPtr(0);
		

		// 빈 이미지인 경우
		if (!firstPtr || dataSize == 0)
			throw BmpParseError("Empty Image");

		// rowStride 가 데이터 크기를 초과하는 경우
		if (static_cast<size_t>(height * rowStride) > dataSize)
			throw BmpParseError("RowStride out of image");

		// 밝기 조정 범위
		if (brightness < -255 || brightness > 255)
			throw std::string("Brightness Value : -255 ~ 255");

		for (int y = startY; y < endY; ++y) {

			std::uint8_t* rowPtr = img.rowPtr(y);

			for (int x = 0; x < width; ++x) {
				int pixelOffset = x * channels;

				std::uint8_t* pixel = &rowPtr[pixelOffset];

				// Brightness
				int newB = pixel[0] + brightness; // b
				int newG = pixel[1] + brightness; // g
				int newR = pixel[2] + brightness; // r

				// Execute
				pixel[0] = static_cast<std::uint8_t>(std::clamp(newB, 0, 255)); // B
				pixel[1] = static_cast<std::uint8_t>(std::clamp(newG, 0, 255)); // G
				pixel[2] = static_cast<std::uint8_t>(std::clamp(newR, 0, 255)); // R
			}
		}
	}
}