#ifndef _S_C_H_
#define _S_C_H_

#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "core/core.hpp"
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <time.h>

using namespace std;
using namespace cv;

#define PSAM_B	0				//the number of the obstacles
#define PSAM_G	1000			//the number of the green sample
#define PSAM_R	1000			//the number of the red sample
#define NSAM	3000			//the number of the negative sample

#define SAM_W	64				//width of sample
#define SAM_H	64				//height of sample

#define I_W	640					//width of image
#define I_H	480					//height of image

#define WIN		Size(32,32)		//窗口大小		原定(32,32)
#define BLOCK	Size(16,16)		//块大小		原定(16,16)
#define STEP	Size(8,8)		//步长			原定(8,8)
#define CELL	Size(8,8)		//细胞大小		原定(8,8)
#define HBIN	9				//直方图分类数

extern Mat HogMat;					//提取特征的矩阵
extern Mat sampleFeatureMat;		//所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数
extern Mat sampleLabelMat;			//训练样本的类别向量，行数等于所有样本的个数，列数等于1；0-r  1-g  2-b  3-neg

extern HOGDescriptor hog;			//hog描述子
extern vector<float> descriptors;	//HOG描述子向量
extern int DescriptorDim;			//描述子维数
extern int extradim;				//额外维数
extern int supportvector;			//支持向量个数

extern unsigned long dwLast;
extern unsigned long dwCurrent;

extern int Hog_Dim_Cal(Size win, Size block, Size step, Size cell, int hbin, int offset);
//时间：2016.8.1
//功能：计算HOG特征维数
//参数：窗口大小，框大小，循环步长，元素大小，直方图分类数，额外维数
//说明：必须整除！
extern void Data_Mat_Init(int dim, int samn);
//时间：2016.8.1
//功能：初始化数据和标签矩阵
//参数：描述子维数，样本个数（各种种类）
//说明：暂无
extern void Hog_Init(void);
//时间：2016.8.2
//功能：初始HOG类模板
//参数：暂无，均由函数内部设定
//说明：暂无
extern void Hog_Get(int flag, int Hog_N, int offset, Size win, Size block, Size step, Size cell);
//时间：2016.8.1
//功能：提取HOG特征
//参数：标签，提取图片个数，已经完成的样本数，窗口大小，框大小，循环步长，元素大小
//说明：0-R  1-G  2-B  3-NEG，每一次执行该函数只能训练同一种样本，不能训练多种样本
extern int SVM_Train(char *filename, Mat sampleFeatureMat, Mat sampleLabelMat);
//时间：2016.8.2
//功能：训练SVM分类器
//参数：分类器保存的目录，数据矩阵，标签矩阵
//说明：暂无
extern void HogReadtxt(char *filename);
//时间：2016.8.2
//功能：写入说明文档
//参数：文件目录
//说明：暂无
extern void RGB_AverCal(Mat mat, vector<float>&v);
//时间：2016.8.18
//功能：计算图像的RGB通道均值
//参数：图像，返回向量
//说明：暂无

#endif