#pragma once
// 实际上#pragma once 与#ifndef  块作用是一样的,都是为了防止同一个文件被#include多次,只是机理有些不同.
// 	对于这两种方法,单独使用其中任何一种就可以,在编译上不会有什么问题.

#ifndef OPENCV_CODE               //为了防止重复预编译，定义的
#define  OPENCV_CODE
#include <opencv2/opencv.hpp>

//  查找最长的轮廓

/*int longcontour(cv::Mat canny_input, std::vector<std::vector<cv::Point>> contours_output,
std::vector<cv::Vec4i> Hierarchy);

//提取长方形轮廓的四个顶点按照上（左右）下（左右）顺序
std::vector<cv::Point2f>  vertex_recog(std::vector<cv::Point> contour, cv::Mat Image);*/

//条形码预处理
//cv::Rect recognise_bar_code(cv::Mat thresholdImage);

//条形码识别
std::string recognise(cv::Mat resultImage);

//指针识别
double pointer_recognise(cv::Mat meterImage);

//string to int
//int string_to_int(std::string s);

#endif#pragma once
#pragma once
