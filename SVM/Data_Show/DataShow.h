#ifndef _DATASHOW_H_
#define _DATASHOW_H_

#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <stdlib.h>
#include <string.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

#define PSAM_B	4157				//the number of the obstacles
#define PSAM_G	4559			//the number of the green sample
#define PSAM_R	4055			//the number of the red sample
#define NSAM	786			//the number of the negative sample

#define HIST_HDIMS 256
#define SAM_W 64
#define SAM_H 64

#define CHB 0x01	//00000001
#define CHG	0x02	//00000010
#define CHR	0x04	//00000100
#define CHH	0x08	//00001000
#define CHS	0x10	//00010000
#define	CHV	0x20	//00100000

typedef struct Hist_Statistic		//直方图统计结构体
{
	//统计时需要将所有图片resize到64*64下
	int Is_Num_Count[6];			//B-G-R-H-S-V的256个数值中不为零的个数
	float Is_Num_Mean[6];			//B-G-R-H-S-V通道的数据分布的均值
	float Is_Num_Stddev[6];			//B-G-R-H-S-V通道的数据分布的标准差
	int Node_Thres[2][6];			//B-G-R-H-S-V选择的12个节点
	int Bet_Node_Count[6];			//在节点区间的且不为零的个数
	float Node_Proportion[6];		//在节点之间的数据所占整幅图的比例
	float Two_Dim_Entropy[6];		//B-G-R-H-S-V六个通道的二维熵
}Hist_Statistic;

extern Mat histshow, hist;

extern void Hist_Init(void);
extern void Hist_Draw(Mat hist, float maxval, int yoffset, Scalar s);
//time: 2016.8.14
//func: draw the histogram of the ROI
//para: histogram
//tips: none
extern void HistGet(Mat img);
//统计直方图
extern void Letter_Show(char *buf, Point point, Mat& img, Scalar s);
//time: 2016.8.14
//func: show some words on the image
//para: words, image
//tips: none

extern void Hist_Statistic_Init(Hist_Statistic* statistic);
//time: 2016.8.22
//func: 初始化直方图统计结构体
//para: 直返图结构体指针
//tips: 暂无
extern int Hist_Ch_Num(Mat img);
//time: 2016.8.22
//func: 统计某一个单通道图像直方图不为零个数
//para: 单通道图像
//tips: 必须单通道
extern void Hist_Is_Num(Mat img, Hist_Statistic *Hist_Statistic, uchar flag);
//time: 2016.8.22
//func: 统计直方图的整个区间不为零的个数0-255
//para: 原图，直方图统计结构体，二进制标志位
//tips: flag请使用宏定义，thanks
extern void Hist_Node_Set(Hist_Statistic *Hist_Statistic, int start[], int end[], uchar flag);
//time: 2016.8.22
//func: 设定统计区间
//para: 直方图结构体，开始数值，结束数值
//tips: 暂无
extern void Any_Ch_Node_Greater(Mat img, Hist_Statistic *Hist_Statistic, uchar flag);
//time: 2016.8.22
//func: 节点之间不为零数据的统计
//para: 图像（多通道），直方图统计结构体，二进制标志位
//tips: 暂无
extern int Sig_Ch_Node_Greater(Mat img, int start, int end);
//time: 2016.8.22
//func: 单通道直方图加点之间数据统计
//para: 单通道图像，起始节点，终止节点
//tips: 暂无
extern void Hist_Variance_Cal(Mat img, Hist_Statistic *Hist_Statistic, uchar flag);
//time: 2016.8.23
//func: 统计各个通道的方差
//para: 统计结构体，二进制标志位
//tips: 暂无
extern float Hist_Ch_Variance_Cal(Mat img);
//time: 2016.8.24
//func: 单通道方差统计
//para: 图像
//tips: 必须单通道
extern float Ch_Entropy_Cal(Mat img);
//time: 2016.8.24
//func: 单通道图像二维熵计算
//para: 单通道图像
//tips: 必须单通道
extern void Entropy_Cal(Mat img, Hist_Statistic *Hist_Statistic, uchar flag);
//time: 2016.8.24
//func: 图像二维熵计算
//para: 图像
//tips: 暂无
extern uchar Eight_NP_Cal(Mat img, int x, int y);
//time: 2016.8.24
//func: 图像八邻域均值计算
//para: 图像，x，y
//tips: 暂无
extern void Color_Balance(Mat &src);
//图像色彩的均衡化
extern Mat HSI_Thres(Mat img);
//基于HSI的阈值分割

extern int Pic_Not_0_Cal(Mat img);
//计算图片中不为零的个数

extern int Pic_Not_0_CirCal(Mat img, int r, Point center);
//计算图片中的圆域内不为零的像素点个数

#endif
