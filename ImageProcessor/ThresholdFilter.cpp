#include "ThresholdFilter.h"

namespace ip {
	std::string ThresholdFilter::GetFilterName() const
	{
		return "ThresoldFilter";
	}

	std::string ThresholdFilter::GetFilterParams() const
	{
		return "Width=" + std::to_string(m_params.width) +
			", Height=" + std::to_string(m_params.height) +
			", RowStride=" + std::to_string(m_params.rowStride) +
			", DataSize=" + std::to_string(m_params.dataSize) +
			", Channels=" + std::to_string(m_params.channels) +
			", Thread=" + std::to_string(m_params.threadCount);
	}

	void ThresholdFilter::processWork(ImageBuffer &img, int startY, int endY)
	{
		int width = img.width();
		int height = img.height();
		int rowStride = img.rowStride();
		size_t dataSize = img.dataSize();
		int channels = img.CHANNELS;
		int threshold = m_threshold;

		uint8_t* firstPtr = img.data();
		

		// 빈 이미지인 경우
		if (!firstPtr || dataSize == 0)
			throw BmpParseError("Empty Image");

		// rowStride 가 데이터 크기를 초과하는 경우
		if (static_cast<size_t>(height * rowStride) > dataSize)
			throw BmpParseError("RowStride out of Image");

		for (int y = startY; y < endY; ++y) {
			std::uint8_t* rowPtr = img.rowPtr(y);

			for (int x = 0; x < width; ++x) {
				int pixelOffset = x * channels;
				std::uint8_t* pixel = &rowPtr[pixelOffset];

				// grayscale
				std::uint8_t gray = 
					static_cast<uint8_t>(
						0.114f * pixel[0] + 
						0.587f * pixel[1] + 
						0.299f * pixel[2]); // b g r

				// threshold
				std::uint8_t binary = gray >= threshold ? 255 : 0;

				// Excute
				pixel[0] = binary; // b
				pixel[1] = binary; // g
				pixel[2] = binary; // r
			}
		}
	}
}