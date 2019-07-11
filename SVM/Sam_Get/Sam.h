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

#define PSAM_B	1000			//�ϰ�����������
#define PSAM_G	1000			//��ɫ����������
#define PSAM_R	1000			//��ɫ����������
#define NSAM	400			//����������

#define SAM_W	64			//������
#define SAM_H	64			//������

#define SAMSIZE Size(SAM_W, SAM_H)	//������С

#define I_W	640				//ͼ���
#define I_H	480				//ͼ���

extern Rect SAM_Rec;		//��������ľ��ο�
extern int Get_Sam_Flag;	//�Ƿ���Խ���HOG��ȡ�ı�־��1Ϊ���ԣ�0Ϊ������

extern Point pre_pt;
extern Point cur_pt;

extern int R_f, G_f, B_f, N_f, downflag;

extern int drawflag;

extern Mat src, srct, dst, temp;

extern void Mat_Init(void);
//ʱ�䣺2016.8.19
//���ܣ������ʼ��
//����������
//˵��������
extern void Sam_R_Get(char *capname, int rectw, int recth, int numstart, int numend, int flag);
//ʱ�䣺2016.8.1
//���ܣ��ɼ�����
//��������Ƶ�ļ�ָ�룬���ο�����ο�ߣ����濪ʼ��ţ����������ţ�������ǩ
//˵���������Լ����ڲ��ͷ���Ƶָ�룬ÿִ��һ�κ���������ȡͬһ������
extern void on_mouse(int event, int x, int y, int flags, void* ustc);
//ʱ�䣺2016.8.1
//���ܣ����ص�

#endif