# ImageProcessor 과제 제출

## 구현 항목

- 알고리즘 -> grayscale, brightness, threshold
- 필터 파이프라인 -> maim.cpp : pipeFilterApply / CommandLineParser.cpp : PipelineParse
- 추상화 클래스 기반 -> FilterBase / GrayscaleFilter / BrightnessFilter / ThresholdFilter
- 스레드 병렬처리 -> FilterBase : apply

## 실행 명령어 (예시)

```powershell
# Grayscale
.\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Result\1_astronaut_grayscale.bmp --filter grayscale
```

```powershell
# Brightness
.\x64\Release\ImageProcessor.exe --input .\Resource\2_coffee.bmp --output .\Result\2_coffee_brightness.bmp --filter brightness:100
```

```powershell
# Threshold
.\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Result\3_chelsea_cat_threshold.bmp --filter threshold:100
```

```powershell
# Thread
.\x64\Release\ImageProcessor.exe --input .\Resource\4_text_page.bmp --output .\Result\4_text_page_brightness.bmp --filter brightness:-50 --thread 4
```

```powershell
# Pipeline Filter
.\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Result\3_chelsea_cat_pipeline.bmp --pipeline grayscale,brightness:50,threshold:128
```
