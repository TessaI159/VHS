#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include "centroid.h"
#include "pixel.h"
#include "color.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <map>
#include <fstream>

// the main function from this file, and the only one any outside function
// will need to call.
// Takes the filename of the video to be processed and the number of centroids to use
// First find the optimal ratio with the best speed/accuracy trade-off
// If the number of centroids is < =1 or > MAX_CENTROIDS (constants.h) calls
// findelbow using the ratio found above in findBestRatio
void processVideo(std::string filename, int centroids);

// The main logic loop that runs for processVideo after processVideo has
// figured out the best ratio and number of centroids
// Calls processFrame once per frame of video
void processVideoLoop(std::string filename, double ratio, int centroids,
		      std::ofstream &outputCache);

// Process a single frame of video in the form of a cv::Mat pointer
void processFrame(cv::Mat *frame, int centroids, std::ofstream &outputCache,
		  cv::VideoWriter &videoWriter);

// Why is it called find elbow, you may ask
// https://www.geeksforgeeks.org/elbow-method-for-optimal-value-of-k-in-kmeans/
// Find the optimal number of centroids using the elbow method
int findElbow(std::string filename, double ratio);

// The main logic of findElbow after findElbow sets up the necessary
// Values. Calls findElbowFrame once per frame.
// Only process percent of frames passed to it as a double
// Typically only a very, very small percent. For a 3300 frame video,
// Which is 2:20 at 23fps, I check 0.5% of the frames
// the std::map has keys representing the number of elbows, and values representing
// the number of frames in which that elbow was the dominant. Uses the elbow
// key with the highest value and the number of centroids
std::map<int, int> findElbowLoop(std::string filename, double percent, double ratio);

// Finds the elbow of a single frame and returns it to findElbowLoop
int findElbowFrame(cv::Mat* frame);

// Calls extractColor to run on percent% of frames in video filename
// Calls compareAccuracy maxRatio - 1 (defined inside the function for now)
// Compares the accuracy of maxRatio and every ratio down to minRatio
// To see which gives the best accuracy/speed tradeoff.
// Currently only outputs so I can create an algorithm to decide
// the best accuracy/speed tradeoff
double findBestRatio(std::string filename, double percent);

// Creates a vector of all the dominant colors from percent% of frames in the video filename
// Resizes each frame using ratio.
// Calls extractColorFrame once per frame
// colorVector is cleared and then created as an out return variable to the caller
void extractColor(std::string filename, double percent, double ratio,
		  double &totalProcessTime_o, double &averageProcessTime_o,
		  std::vector<Color> &colorVector);

// Main logic loop of extractColor. Synonymous to findElbowLoop for findElbow.
// Tests percent% of frames at ratio and returns a vector through an out variable
// Containing all the colors found
int extractColorLoop(std::string filename, double percent, double ratio,
		     std::vector<Color> &colorVector);

// Find the 3 dominant colors in an image and returns them in a vector
// Uses 3 because that is the most common number of centroids that will be
// picked by findElbow
// I don't mind returning this vector by value instead of
// reference because it only contains 3 objects
std::vector<Color> extractColorFrame(cv::Mat *frame, int currentFrame);

// Finds and returns the average difference in color using deltaE00Difference (color.cpp)
// largestDistance_o is changed to be the largest absolute difference
// between two colors
void compareAccuracy(std::vector<Color> largerColorVector,
		     std::vector<Color> smallerColorVector, double &largestDistance_o);

// Calls drawFrame and writeFrame in that order.
void createFrame(int width, int height, std::vector<Pixel> &pixelVector,
		 std::vector<int> &ratios, cv::VideoWriter &videoWriter, cv::Mat &createdFrame);

// Draws three rectangles into a frame and passes it to write frame
void drawFrame(int width, int height, std::vector<Pixel> &pixelVector,
	       std::vector<int> &ratios, cv::Mat &frame);

//Writes the frame to the video file
void writeFrame(cv::VideoWriter &videoWriter, cv::Mat &frame);

// Sorts intVector in descending order. Each time a swap is done on
// intVector an equivalent swap is done on pixelVector
void reorder(std::vector<Pixel> &pixelVector, std::vector<int> &intVector);
bool descending(std::vector<int> &intVector);

// Plays both the original and the created video
void playVideos(std::string original, std::string color);

// Ensures the videos are relatively the same length i.e. within tolerance
// frames of each other
bool checkVideos(std::string original, std::string color, double tolerance);

bool checkCache(std::string filename);

#endif
