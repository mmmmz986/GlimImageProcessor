#include "GrayscaleFilter.h"

namespace ip {
	std::string GrayscaleFilter::GetFilterName() const
	{
		return "GrayscaleFilter";
	}

	std::string GrayscaleFilter::GetFilterParams() const 
	{
		return "Width=" + std::to_string(m_params.width) +
			", Height=" + std::to_string(m_params.height) +
			", RowStride=" + std::to_string(m_params.rowStride) +
			", DataSize=" + std::to_string(m_params.dataSize) +
			", Channels=" + std::to_string(m_params.channels) +
			", Thread=" + std::to_string(m_params.threadCount);
	}

	void GrayscaleFilter::processWork(ImageBuffer& img, int startY, int endY) {
		int width = img.width();
		int height = img.height();
		int rowStride = img.rowStride();
		int channels = img.CHANNELS;
		size_t dataSize = img.dataSize();

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

				// Grayscale
				std::uint8_t gray = 
					static_cast<std::uint8_t>(
						0.114f * pixel[0] +
						0.587f * pixel[1] +
						0.299f * pixel[2]); // b g r

				// Excute
				pixel[0] = gray; // b
				pixel[1] = gray; // g
				pixel[2] = gray; // r
			}
		}
	}
}