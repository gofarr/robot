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
#include "core/core.hpp"
#include <iostream>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

#define PSAM_B	0				//the number of the obstacles
#define PSAM_G	3893			//the number of the green sample
#define PSAM_R	3320			//the number of the red sample
#define NSAM	2604			//the number of the negative sample

#define Xoffleft	50
#define Xoffright	248
#define Yoffleft	50
#define Yoffright	90

extern Mat DataShow;			//显示曲线
extern Mat src;					//图像
extern int reddata[3][PSAM_R];
extern int greendata[3][PSAM_G];
extern int negdata[3][NSAM];

extern MatND hist;
extern Mat histshow;

#define HIST_HDIMS 256
#define SAM_W 64
#define SAM_H 64

#define CHB 0x01	//00000001
#define CHG	0x02	//00000010
#define CHR	0x04	//00000100
#define CHH	0X08	//00001000
#define CHS	0X10	//00010000
#define	CHV	0X20	//00100000

typedef struct Hist_Statistics		//直方图统计结构体
{
	//统计时需要将所有图片resize到64*64下
	int Is_Num_Count[6];			//B-G-R-H-S-V的256个数值中不为零的个数
	float Is_Num_Mean[6];			//B-G-R-H-S-V通道的数据分布的均值
	float Is_Num_Stddev[6];			//B-G-R-H-S-V通道的数据分布的标准差
	int Node_Thres[2][6];			//B-G-R-H-S-V选择的12个节点
	int Bet_Node_Count[6];			//在节点区间的且不为零的个数
	float Node_Proportion[6];		//在节点之间的数据所占整幅图的比例
	float Two_Dim_Entropy[6];		//B-G-R-H-S-V六个通道的二维熵
}Hist_Statistics;

extern void Coor_RecDraw(int xoffleft, int yoffleft, int xoffright, int yoffright);
//画出坐标系矩形
//x轴刨除像素数、y轴刨除像素数
extern void CoorNumDraw(int xbin, int ybin, int xstart, int ystart, int xstep, int ystep);
//写出坐标
//x轴区间数，y轴区间数，x起始数，y起始数，x步长，y步长
extern void Grid_Draw(int xbin, int ybin, int xstart, int ystart, int xstep, int ystep);
//画网格
//同上
extern void DataShow_Init(int width, int height);
//画曲线图像的初始化
extern void ChaH_Ave_Cal(int start, int end, int flag);
//计算若干图片H通道的平均值
//起始标号，终止标号，0-red  1-green  2-neg
//每一次只能画一组曲线
extern void DataDraw(int flag, int channel);



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
extern float Hist_Wei_Cal(Mat hist, int binstart, int binend);
//计算某一段直方图所占比例

extern void Hist_Statistics_Init(Hist_Statistics* statistics);
//time: 2016.8.22
//func: 初始化直方图统计结构体
//para: 直返图结构体指针
//tips: 暂无
extern int Hist_Ch_Num(Mat img);
//time: 2016.8.22
//func: 统计某一个单通道图像直方图不为零个数
//para: 单通道图像
//tips: 必须单通道
extern void Hist_Is_Num(Mat img, Hist_Statistics *hist_statistics, uchar flag);
//time: 2016.8.22
//func: 统计直方图的整个区间不为零的个数0-255
//para: 原图，直方图统计结构体，二进制标志位
//tips: flag请使用宏定义，thanks
extern void Hist_Node_Set(Hist_Statistics *hist_statistics, int start[], int end[], uchar flag);
//time: 2016.8.22
//func: 设定统计区间
//para: 直方图结构体，开始数值，结束数值
//tips: 暂无
extern void Hist_Node_Cal(Mat img, Hist_Statistics *hist_statistics, uchar flag);
//time: 2016.8.22
//func: 节点之间不为零数据的统计
//para: 图像（多通道），直方图统计结构体，二进制标志位
//tips: 暂无
extern int Hist_Ch_Node_Num(Mat img, int start, int end);
//time: 2016.8.22
//func: 单通道直方图加点之间数据统计
//para: 单通道图像，起始节点，终止节点
//tips: 暂无
extern void Hist_Variance_Cal(Mat img, Hist_Statistics *hist_statistics, uchar flag);
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
extern void Entropy_Cal(Mat img, Hist_Statistics *hist_statistics, uchar flag);
//time: 2016.8.24
//func: 图像二维熵计算
//para: 图像
//tips: 暂无
extern uchar Eight_NP_Cal(Mat img, int x, int y);
//time: 2016.8.24
//func: 图像八邻域均值计算
//para: 图像，x，y
//tips: 暂无


extern int Sig_Ch_Node_Greater(Mat img, int start, int end, int threshold);
//时间：2016.8.22
//功能：单通道图像节点之间不为零数据统计
//参数：直方图结构体，开始数值，结束数值，阈值
//说明：暂无
extern void Any_Ch_Node_Greater(Mat img, Hist_Statistics *hist_statistic, int threshold, uchar flag);
//时间：2016.8.22
//功能：多通道图像节点之间不为零数据统计
//参数：原图，统计结构体，阈值，二进制标志位
//说明：暂无
extern float Sig_Ch_Variance(Mat img);
//时间：2016.8.22
//功能：统计单通道图像的方差
//参数：单通道图像
//说明：暂无
extern void Any_Ch_Variance(Mat img, Hist_Statistics *hist_statistic, uchar flag);
//时间：2016.8.23
//功能：统计各个通道的方差
//参数：统计结构体，二进制标志位
//说明：暂无
#endif