#pragma once
#include "ImageBuffer.h"
#include "Exceptions.h"
#include "define.h"
#include <thread>

namespace ip {
    struct FilterParamsInfo
    {
        int width = 0;
        int height = 0;
        int rowStride = 0;
        int channels = 0;
        size_t dataSize = 0;
        int threadCount = 0;
    };

    class FilterBase
    {

    public:
        virtual ~FilterBase() = default;

        bool apply(ip::ImageBuffer& img, int userThreadCount = 1) {
            m_params.width = img.width();
            m_params.height = img.height();
            m_params.rowStride = img.rowStride();
            m_params.channels = img.CHANNELS;
            m_params.dataSize = img.dataSize();
            m_params.threadCount = userThreadCount;

            unsigned int numThreads = userThreadCount;

            if (numThreads <= 0) {
                numThreads = std::thread::hardware_concurrency();
                if (numThreads == 0) {
                    numThreads = 1;
                }
            }

            if (numThreads > (unsigned int)img.height()) {
                numThreads = img.height();
            }

            std::vector<std::thread> threads;
            int rowsPerThread = img.height() / numThreads;

            for (unsigned int i = 0; i < numThreads; ++i) {
                int startY = i * rowsPerThread;
                int endY = (i == numThreads - 1) ? img.height() : (startY + rowsPerThread);

                // 파생 클래스가 구현한 알고리즘 함수 실행
                threads.push_back(std::thread(&FilterBase::processWork, this, std::ref(img), startY, endY));
            }

            for (auto& t : threads) 
            {
                t.join();
            }

            return true;
        }

		virtual std::string GetFilterName() const = 0;
        virtual std::string GetFilterParams() const = 0;
        virtual void processWork(ip::ImageBuffer& img, int startY, int endY) = 0;

    protected:
        FilterParamsInfo m_params;
    };

    
}
