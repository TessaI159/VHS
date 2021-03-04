#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include <opencv2/opencv.hpp>

#include <string>

void processVideo(std::string filename, int centroids);
void processVideo(std::string filename);
void processFrame(cv::Mat *frame, int centroids, int pixels);

#endif
