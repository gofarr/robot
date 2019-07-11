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

extern Mat DataShow;			//��ʾ����
extern Mat src;					//ͼ��
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

typedef struct Hist_Statistics		//ֱ��ͼͳ�ƽṹ��
{
	//ͳ��ʱ��Ҫ������ͼƬresize��64*64��
	int Is_Num_Count[6];			//B-G-R-H-S-V��256����ֵ�в�Ϊ��ĸ���
	float Is_Num_Mean[6];			//B-G-R-H-S-Vͨ�������ݷֲ��ľ�ֵ
	float Is_Num_Stddev[6];			//B-G-R-H-S-Vͨ�������ݷֲ��ı�׼��
	int Node_Thres[2][6];			//B-G-R-H-S-Vѡ���12���ڵ�
	int Bet_Node_Count[6];			//�ڽڵ�������Ҳ�Ϊ��ĸ���
	float Node_Proportion[6];		//�ڽڵ�֮���������ռ����ͼ�ı���
	float Two_Dim_Entropy[6];		//B-G-R-H-S-V����ͨ���Ķ�ά��
}Hist_Statistics;

extern void Coor_RecDraw(int xoffleft, int yoffleft, int xoffright, int yoffright);
//��������ϵ����
//x���ٳ���������y���ٳ�������
extern void CoorNumDraw(int xbin, int ybin, int xstart, int ystart, int xstep, int ystep);
//д������
//x����������y����������x��ʼ����y��ʼ����x������y����
extern void Grid_Draw(int xbin, int ybin, int xstart, int ystart, int xstep, int ystep);
//������
//ͬ��
extern void DataShow_Init(int width, int height);
//������ͼ��ĳ�ʼ��
extern void ChaH_Ave_Cal(int start, int end, int flag);
//��������ͼƬHͨ����ƽ��ֵ
//��ʼ��ţ���ֹ��ţ�0-red  1-green  2-neg
//ÿһ��ֻ�ܻ�һ������
extern void DataDraw(int flag, int channel);



extern void Hist_Init(void);
extern void Hist_Draw(Mat hist, float maxval, int yoffset, Scalar s);
//time: 2016.8.14
//func: draw the histogram of the ROI
//para: histogram
//tips: none
extern void HistGet(Mat img);
//ͳ��ֱ��ͼ
extern void Letter_Show(char *buf, Point point, Mat& img, Scalar s);
//time: 2016.8.14
//func: show some words on the image
//para: words, image
//tips: none
extern float Hist_Wei_Cal(Mat hist, int binstart, int binend);
//����ĳһ��ֱ��ͼ��ռ����

extern void Hist_Statistics_Init(Hist_Statistics* statistics);
//time: 2016.8.22
//func: ��ʼ��ֱ��ͼͳ�ƽṹ��
//para: ֱ��ͼ�ṹ��ָ��
//tips: ����
extern int Hist_Ch_Num(Mat img);
//time: 2016.8.22
//func: ͳ��ĳһ����ͨ��ͼ��ֱ��ͼ��Ϊ�����
//para: ��ͨ��ͼ��
//tips: ���뵥ͨ��
extern void Hist_Is_Num(Mat img, Hist_Statistics *hist_statistics, uchar flag);
//time: 2016.8.22
//func: ͳ��ֱ��ͼ���������䲻Ϊ��ĸ���0-255
//para: ԭͼ��ֱ��ͼͳ�ƽṹ�壬�����Ʊ�־λ
//tips: flag��ʹ�ú궨�壬thanks
extern void Hist_Node_Set(Hist_Statistics *hist_statistics, int start[], int end[], uchar flag);
//time: 2016.8.22
//func: �趨ͳ������
//para: ֱ��ͼ�ṹ�壬��ʼ��ֵ��������ֵ
//tips: ����
extern void Hist_Node_Cal(Mat img, Hist_Statistics *hist_statistics, uchar flag);
//time: 2016.8.22
//func: �ڵ�֮�䲻Ϊ�����ݵ�ͳ��
//para: ͼ�񣨶�ͨ������ֱ��ͼͳ�ƽṹ�壬�����Ʊ�־λ
//tips: ����
extern int Hist_Ch_Node_Num(Mat img, int start, int end);
//time: 2016.8.22
//func: ��ͨ��ֱ��ͼ�ӵ�֮������ͳ��
//para: ��ͨ��ͼ����ʼ�ڵ㣬��ֹ�ڵ�
//tips: ����
extern void Hist_Variance_Cal(Mat img, Hist_Statistics *hist_statistics, uchar flag);
//time: 2016.8.23
//func: ͳ�Ƹ���ͨ���ķ���
//para: ͳ�ƽṹ�壬�����Ʊ�־λ
//tips: ����
extern float Hist_Ch_Variance_Cal(Mat img);
//time: 2016.8.24
//func: ��ͨ������ͳ��
//para: ͼ��
//tips: ���뵥ͨ��
extern float Ch_Entropy_Cal(Mat img);
//time: 2016.8.24
//func: ��ͨ��ͼ���ά�ؼ���
//para: ��ͨ��ͼ��
//tips: ���뵥ͨ��
extern void Entropy_Cal(Mat img, Hist_Statistics *hist_statistics, uchar flag);
//time: 2016.8.24
//func: ͼ���ά�ؼ���
//para: ͼ��
//tips: ����
extern uchar Eight_NP_Cal(Mat img, int x, int y);
//time: 2016.8.24
//func: ͼ��������ֵ����
//para: ͼ��x��y
//tips: ����


extern int Sig_Ch_Node_Greater(Mat img, int start, int end, int threshold);
//ʱ�䣺2016.8.22
//���ܣ���ͨ��ͼ��ڵ�֮�䲻Ϊ������ͳ��
//������ֱ��ͼ�ṹ�壬��ʼ��ֵ��������ֵ����ֵ
//˵��������
extern void Any_Ch_Node_Greater(Mat img, Hist_Statistics *hist_statistic, int threshold, uchar flag);
//ʱ�䣺2016.8.22
//���ܣ���ͨ��ͼ��ڵ�֮�䲻Ϊ������ͳ��
//������ԭͼ��ͳ�ƽṹ�壬��ֵ�������Ʊ�־λ
//˵��������
extern float Sig_Ch_Variance(Mat img);
//ʱ�䣺2016.8.22
//���ܣ�ͳ�Ƶ�ͨ��ͼ��ķ���
//��������ͨ��ͼ��
//˵��������
extern void Any_Ch_Variance(Mat img, Hist_Statistics *hist_statistic, uchar flag);
//ʱ�䣺2016.8.23
//���ܣ�ͳ�Ƹ���ͨ���ķ���
//������ͳ�ƽṹ�壬�����Ʊ�־λ
//˵��������
#endif