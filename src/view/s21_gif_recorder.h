/**
 * @file s21_gif_recorder.h
 * @brief Gif recorder header file.
 */

#ifndef S21_GIF_RECORDER_H
#define S21_GIF_RECORDER_H

#include <QImage>
#include <string>
#include <vector>
#include <iostream>

namespace s21 {

/**
 * @brief Class that implements the logic of the GIF recorder.
 **/
class GifRecorder
{
public:
    GifRecorder();
    ~GifRecorder();

    /**
   * @brief Creates .gif file with given filename and parameters.
   * The exact number of frames equal to fps * duration must be transmitted.
   * There is no checks for the number of frames within a class.
   * @param outputFilename Filename of recording gif.
   * @param w Width of gif.
   * @param h Height of gif.
   * @param fps Number of frames per second in a recorded gif.
   * @param duration Duration of gif.
   **/
    void CreateGif(std::string outputFilename, int w, int h, int fps, int duration);

    /**
   * @brief Adds image in created gif
   * @param image Image that will be added.
   **/
    void AddImage(QImage image);

    /**
   * @brief Completes recording of gif.
   **/
    void CompleteGIF();
private:
    void* gifWriter; ///< A pointer to an internal structure gifwriter in the library.
    int width; ///< Width of gif.
    int height; ///< Height of gif.
    int frameDelay; ///< Duration of each gif frame.

    /**
   * @brief Private method to convert a QImage to a byte array.
   * @param image Image that will be added.
   * @return Byte array represented as vector<uint8_t>.
   **/
    std::vector<uint8_t> ConvertQImage(const QImage& image) const;
};

} //namespace s21

#endif // S21_GIF_RECORDER_H
