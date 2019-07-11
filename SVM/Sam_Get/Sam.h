#ifndef _SAM_H_
#define _SAM_h_

#include <iostream>
#include <fstream>
#include <algorithm>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "core/core.hpp"
#include <stdio.h>
#include <math.h>
#include <string>

using namespace std;
using namespace cv;

#define PSAM_B	1000			//障碍正样本数量
#define PSAM_G	1000			//绿色正样本数量
#define PSAM_R	1000			//红色正样本数量
#define NSAM	400			//负样本数量

#define SAM_W	64			//样本宽
#define SAM_H	64			//样本高

#define SAMSIZE Size(SAM_W, SAM_H)	//样本大小

#define I_W	640				//图像宽
#define I_H	480				//图像高

extern Rect SAM_Rec;		//框出样本的矩形框
extern int Get_Sam_Flag;	//是否可以进行HOG提取的标志，1为可以，0为不可以

extern Point pre_pt;
extern Point cur_pt;

extern int R_f, G_f, B_f, N_f, downflag;

extern int drawflag;

extern Mat src, srct, dst, temp;

extern void Mat_Init(void);
//时间：2016.8.19
//功能：矩阵初始化
//参数：暂无
//说明：暂无
extern void Sam_R_Get(char *capname, int rectw, int recth, int numstart, int numend, int flag);
//时间：2016.8.1
//功能：采集样本
//参数：视频文件指针，矩形框宽，矩形框高，储存开始编号，储存结束编号，样本标签
//说明：必须自己在内部释放视频指针，每执行一次函数仅能提取同一种样本
extern void on_mouse(int event, int x, int y, int flags, void* ustc);
//时间：2016.8.1
//功能：鼠标回调

#endif