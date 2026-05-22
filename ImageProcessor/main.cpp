/**
 * @file main.cpp
 * @brief ImageProcessor 진입점 — 지원자가 작성해야 할 파일입니다.
 *
 * BMP 입출력과 커맨드라인 파싱은 제공된 코드가 처리합니다.
 * 본 과제에서 작성해야 할 것은 단 하나입니다:
 *
 *     ▶ 이미지 처리 필터 2개 이상 구현 + main 의 TODO 위치에 연결
 *
 * 또한 일관된 컨벤션과 예외 처리, 메모리 안정성도 함께 평가됩니다.
 */

#include "BmpParser.h"
#include "CommandLineParser.h"
#include "ImageBuffer.h"
#include "Exceptions.h"

#include <iostream>

 // TODO: 본인이 구현한 필터 헤더를 include 하세요.
 // #include "GrayscaleFilter.h"
 // #include "ThresholdFilter.h"
 // ...
#include "GrayscaleFilter.h"
#include "BrightnessFilter.h"
#include "ThresholdFilter.h"
#include "FilterBase.h"
#include "FilterPipeline.h"
#include "Log.h"
#include "define.h"

void singleFilterApply(ip::ProgramOptions options, ip::ImageBuffer& image, int threadCount);
void pipeFilterApply(ip::ProgramOptions options, ip::ImageBuffer& image, int threadCount);

int main(int argc, char* argv[]) {
    try {
        // ── CLI 인자 파싱 (제공된 코드) ─────────────────────────
        const ip::ProgramOptions options = ip::CommandLineParser::parse(argc, argv);

        // ── BMP 로드 (제공된 코드) ──────────────────────────────
        ip::ImageBuffer image = ip::BmpParser::loadFromFile(options.inputPath);
        std::cout << "Loaded: " << image.width() << " x " << image.height() << "\n";

        // ───────────────────────────────────────────────────────
        // TODO: options.filterName 에 따라 적절한 필터를 생성하고
        //       image 에 적용하세요.
        //
        //   예시 코드 (참고용):
        //
        //     if (options.filterName == "grayscale") {
        //         GrayscaleFilter filter;
        //         filter.apply(image);
        //     }
        //     else if (options.filterName == "threshold:128") {
        //         ThresholdFilter filter(128);
        //         filter.apply(image);
        //     }
        //     else {
        //         throw ip::FilterError("Unknown filter: " + options.filterName);
        //     }
        //
        //   ※ 가산점 항목:
        //     - 추상 클래스(FilterBase) 기반 다형성 설계
        //     - 필터 파이프라인 체인 (CLI 옵션 확장 필요)
        //     - 멀티쓰레드 처리
        //     - 로그 파일 출력 (CLI 옵션 확장 필요)
        // ───────────────────────────────────────────────────────

        // ↓ 여기에 필터 적용 코드를 작성하세요.

        // 스레드 수 확인
        int threadCount = DEFAULT_THREAD_COUNT;
        if (!options.ThreadCount.empty())
        {
            threadCount = std::stoi(options.ThreadCount);
            if (threadCount <= 0) {
                threadCount = DEFAULT_THREAD_COUNT;
            }
        }
		if (options.pipeline.empty()) 
        {
            // 단일 필터
            singleFilterApply(options, image, threadCount);
        }
        else { 
            // 파이프 필터
            pipeFilterApply(options, image, threadCount);
        }

        
        // ── BMP 저장 (제공된 코드) ──────────────────────────────
        ip::BmpParser::saveToFile(options.outputPath, image);
        std::cout << "Saved:  " << options.outputPath << "\n";
        return 0;
    }
    catch (const ip::ArgumentError& e) {
        std::cerr << e.what() << "\n\n";
        ip::CommandLineParser::printUsage(argc > 0 ? argv[0] : "ImageProcessor");
        return 4;
    }
    catch (const ip::BmpParseError& e) {
        std::cerr << e.what() << std::endl;
        return 2;
    }
    catch (const ip::FilterError& e) {
        std::cerr << e.what() << std::endl;
        return 3;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
}

void singleFilterApply(ip::ProgramOptions options , ip::ImageBuffer& image, int threadCount) {
    ip::FilterBase* filter = nullptr;
    bool success;

    // 실행
    if (options.filterName.find("grayscale") == 0) {
        filter = new ip::GrayscaleFilter();
    }
    else if (options.filterName.find("brightness") == 0) {
        if (options.value.empty()) {
            filter = new ip::BirghtnessFilter();
        }
        else {
            int value = std::stoi(options.value);
            filter = new ip::BirghtnessFilter(value);
        }
    }
    else if (options.filterName.find("threshold") == 0) {
        if (options.value.empty()) {
            filter = new ip::ThresholdFilter();
        }
        else {
            int value = std::stoi(options.value);
            filter = new ip::ThresholdFilter(value);
        }
    }
    success = filter->apply(image, threadCount);

    // 결과
    if (filter) {
        ip::FilterParamsInfo Params;

        if (success)
        {
            ip::Log::log("Applied Filter: " + filter->GetFilterName(), true, filter->GetFilterParams());
        }
        else
        {
            ip::Log::log("Applied Filter: " + filter->GetFilterName(), false, filter->GetFilterParams());
        }
    }

    delete filter;
    filter = nullptr;
}

void pipeFilterApply(ip::ProgramOptions options, ip::ImageBuffer& image, int threadCount) {
    ip::FilterBase* filter = nullptr;
    bool success = false;

    if (options.filterFromPipeline.size() != options.valueFromPipeline.size()) {
        throw std::string("PipelineParseError");
    }

    std::string logStr;

    for (size_t i = 0; i < options.filterFromPipeline.size(); ++i) {
        const std::string& filterName = options.filterFromPipeline[i];
        const std::string& valueStr = options.valueFromPipeline[i];

        delete filter;

        int value = 0;

        if (filterName == "grayscale") {
            filter = new ip::GrayscaleFilter();
        }
        else if (filterName == "brightness") {
            if (valueStr.empty()) {
                filter = new ip::BirghtnessFilter();
            }
            else {
                value = std::stoi(valueStr);
                filter = new ip::BirghtnessFilter(value);
            }
        }
        else if (filterName == "threshold") {         
            if (valueStr.empty()) {
                filter = new ip::ThresholdFilter();
            }
            else {
                value = std::stoi(valueStr);
                filter = new ip::ThresholdFilter(value);
            }
        }
        else {
            throw std::string("Unkown filter Name: " + filterName);
        }

        success = filter->apply(image, threadCount);
        if (!success) {
            break;
        }

        if (!logStr.empty()) {
            logStr += ", ";
        }
        logStr += filterName;
    }

    if (success)
    {
        ip::Log::log("Applied Filters: " + logStr, true);
    }
    else
    {
        ip::Log::log("Applied Filter: " + logStr, false);
    }

    delete filter;
    filter = nullptr;
}