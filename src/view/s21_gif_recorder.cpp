/**
 * @file s21_gif_recorder.cpp
 * @brief Gif recorder implementation.
 */

#include "s21_gif_recorder.h"

#include "include/gif.h"

namespace s21 {

GifRecorder::GifRecorder() : gifWriter(new GifWriter), width(0), height(0) {}

GifRecorder::~GifRecorder() { delete static_cast<GifWriter *>(gifWriter); }

void GifRecorder::CreateGif(std::string outputFilename, int w, int h, int fps,
                            int duration) {
  width = w;
  height = h;
  frameDelay = (fps / (fps * duration * 1.0f)) * 100.0f;
  GifBegin(static_cast<GifWriter *>(gifWriter), outputFilename.data(), width,
           height, frameDelay);
}

void GifRecorder::AddImage(QImage image) {
  std::vector<uint8_t> frame = ConvertQImage(image);
  GifWriteFrame(static_cast<GifWriter *>(gifWriter), frame.data(), width,
                height, frameDelay);
}

void GifRecorder::CompleteGIF() { GifEnd(static_cast<GifWriter *>(gifWriter)); }

std::vector<uint8_t> GifRecorder::ConvertQImage(const QImage &image) const {
  QImage convertedImage = image
                              .scaled(width, height, Qt::IgnoreAspectRatio,
                                      Qt::SmoothTransformation)
                              .convertToFormat(QImage::Format_RGBA8888);
  std::vector<uint8_t> frame(convertedImage.sizeInBytes());
  std::memcpy(frame.data(), convertedImage.bits(),
              convertedImage.sizeInBytes());
  return frame;
}

}  // namespace s21
