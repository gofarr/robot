/*
	The second way of Object-Detecting in this game:
	This algorithm is based on machine learning, which we call it SVM(Support Vector Machine).

	This file should contain the following sections:
	1.Definition of some struct.
	2.Some const parameters.
	3....
*/
#ifndef _PARA_H_
#define _PARA_H_

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

#define MAX_NUM 10
#define NUM_COF 46

/**********************some parameters about data fitting************************/

typedef enum Robust
{
	OFF,			//��ʹ��³���Լ��
	LAR,			//LAR³���Լ��
	BIS				//Bisquare³���Լ��
}Robust;

typedef struct Data_Fit
{
	double coefficient[7];			//coefficients of formula
	bool C_PLUS_S;					//center and scale
	double SSE;						//���ƽ����
	double ReSquare;				//�����ϵ��
	double DFE;						//���ɶ�
	double AdjReSquare;				//�������ɶȵĸ����ϵ��
	double RMSE;					//���������
	Robust robust;
}Data_Fit;

#define MIN_EXP_2_CSOFF	0
#define MIN_EXP_2_CSON	1
#define MIN_EXP_4_CSOFF	2	//okay
#define MIN_EXP_4_CSON	3
#define MIN_FOU_4_CSOFF	4
#define MIN_FOU_4_CSON	5
#define MIN_FOU_6_CSOFF	6
#define MIN_FOU_6_CSON	7
#define MIN_POL_3_CSOFF_ROBUSTOFF	8	//okayokay
#define MIN_POL_3_CSOFF_ROBUSTLAR	9
#define MIN_POL_3_CSOFF_ROBUSTBIS	10
#define MIN_POL_3_CSON_ROBUSTOFF	11
#define MIN_POL_3_CSON_ROBUSTLAR	12
#define MIN_POL_3_CSON_ROBUSTBIS	13
#define MIN_POL_4_CSOFF_ROBUSTOFF	14
#define MIN_POL_4_CSON_ROBUSTOFF	15
#define MIN_POL_4_CSON_ROBUSTLAR	16
#define MIN_POL_4_CSOFF_ROBUSTBIS	17
#define MIN_POL_4_CSON_ROBUSTBIS	18
#define MIN_POW_2_CSOFF	19	//okay
#define MIN_POW_3_CSOFF	20
#define MIN_RAT_7_CSOFF	21

#define MAX_EXP_2_CSOFF	22
#define MAX_EXP_2_CSON	23
#define MAX_EXP_4_CSOFF	24	//okay
#define MAX_EXP_4_CSON	25
#define MAX_FOU_4_CSOFF	26
#define MAX_FOU_4_CSON	27
#define MAX_FOU_6_CSOFF	28
#define MAX_FOU_6_CSON	29
#define MAX_POL_3_CSOFF_ROBUSTOFF	30
#define MAX_POL_3_CSON_ROBUSTOFF	31
#define MAX_POL_3_CSOFF_ROBUSTLAR	32
#define MAX_POL_3_CSON_ROBUSTLAR	33
#define MAX_POL_3_CSOFF_ROBUSTBIS	34
#define MAX_POL_3_CSON_ROBUSTBIS	35
#define MAX_POL_4_CSOFF_ROBUSTOFF	36
#define MAX_POL_4_CSON_ROBUSTOFF	37
#define MAX_POL_4_CSON_ROBUSTLAR	38
#define MAX_POL_4_CSOFF_ROBUSTBIS	39
#define MAX_POL_4_CSON_ROBUSTBIS	40
#define MAX_POW_2_CSOFF	41
#define MAX_POW_3_CSOFF	42
#define MAX_RAT_3_CSOFF	43
#define MAX_RAT_4_CSON	44
#define MAX_RAT_5_CSOFF	45

#define MIN_SELF	46
#define MAX_SELF	47

const Data_Fit DATA_FIT[NUM_COF] = {
	{ { 181.6, -0.008281 }, false, 242.71, 0.9443, 7, 0.9363, 5.8883, OFF },												//0
	{ { 48.26, -0.4536 }, true, 242.71, 0.9443, 7, 0.9363, 5.8883, OFF },													//1
	{ { 237.8, -0.01122, 0.3463, 0.0164 }, false, 79.3568, 0.9818, 5, 0.9709, 3.9839, OFF },								//2
	{ { 39.48, -0.6146, 4.775, 0.8982 }, true, 79.3568, 0.9818, 5, 0.9709, 3.9839, OFF },									//3
	{ { 2.613e8, -2.613e8, 2.911e5, -5.134e-6 }, false, 74.9325, 0.9828, 5, 0.9725, 3.8712, OFF },							//4
	{ { 6.627e6, -6.627e6, 1.217e4, -0.001766 }, true, 74.9325, 0.9828, 5, 0.9725, 3.8712, OFF },							//5
	{ { 57.37, -9.86, 28.53, -8.637, -2.663, 0.02455 }, false, 28.4547, 0.9935, 5, 0.9826, 3.0798, OFF },					//6
	{ { 57.37, -13.23, -27.13, -2.646, 8.643, 1.345 }, true, 28.4547, 0.9935, 5, 0.9826, 3.0798, OFF },						//7
	{ { 0.003444, -1.495, 194.9 }, false, 74.9325, 0.9828, 6, 0.9771, 3.5339, OFF },										//8
	{ { 0.002865, -1.31, 182.5 }, false, 95.8707, 0.9780, 6, 0.9707, 3.9773, LAR },											//9
	{ { 0.003338, -1.462, 192.9 }, false, 90.5312, 0.9792, 6, 0.9723, 3.8844, BIS },										//10
	{ { 10.33, -21.5, 43.93 }, true, 74.9325, 0.9828, 6, 0.9771, 3.5339, OFF },												//11
	{ { 8.594, -21.57, 46.17 }, true, 95.8707, 0.9780, 6, 0.9707, 3.9973, LAR },											//12
	{ { 10.01, -21.58, 44.46 }, true, 90.5321, 0.9792, 6, 0.9723, 3.8844, BIS },											//13
	{ { -4.314e-6, 0.005515, -1.806, 209.3 }, false, 73.2345, 0.9832, 5, 0.9731, 3.8271, OFF },								//14
	{ { -0.7089, 10.33, -20.38, 43.93 }, true, 73.2345, 0.9832, 5, 0.9731, 3.8271, OFF },									//15
	{ { 0.7702, 8.031, -23.21, 47.37 }, true, 46.8701, 0.9892, 5, 0.9828, 3.0617, LAR },									//16
	{ { -4.225e-6, 0.005456, -1.794, 208.8 }, false, 86.8155, 0.9801, 5, 0.9681, 4.1669, BIS },								//17
	{ { -0.6942, 10.28, -20.43, 44.03 }, true, 86.8155, 0.9801, 5, 0.9681, 4.1669, BIS },									//18
	{ { 1.202e4, -1.096 }, false, 136.38, 0.9687, 7, 0.9642, 4.4140, OFF },													//19
	{ { 1.439e4, -1.151, 2.759 }, false, 136.01, 0.9688, 6, 0.9584, 4.7612, OFF },											//20
	{ { 0.003574, -1.548, 200.7, -14.81, 159.1, 1.606, 23.31 }, false, 74.7871, 0.9828, 2, 0.9313, 6.1150, OFF },			//21

	{ { 289.7, -0.006921 }, false, 219.17, 0.9746, 7, 0.9709, 6.4495, OFF },												//22
	{ { 95.73, -0.3791 }, true, 219.17, 0.9746, 7, 0.9709, 6.4495, OFF },													//23
	{ { 1910, -0.04951, 214.1, -0.005256 }, false, 11.8434, 0.9990, 5, 0.9983, 1.5391, OFF },								//24
	{ { 0.6932, -2.712, 92.35, -0.2879 }, true, 11.8434, 0.9990, 5, 0.9983, 1.5391, OFF },									//25
	{ { 1.375e8, -1.375e8, 2.547e5, -7.65e-6 }, false, 173.06, 0.9849, 5, 0.9758, 5.8832, OFF },							//26
	{ { 1.343e8, -1.343e8, 8.538e4, -0.0004239 }, true, 173.06, 0.9849, 5, 0.9758, 5.8832, OFF },							//27
	{ { 6.963e12, -9.284e12, -4.074e10, 2.321e12, 2.037e10, 2.305e-5 }, false, 12.5484, 0.9989, 3, 0.9971, 2.0452, OFF },	//28
	{ { 5.439e8, -7.252e8, -5.397e6, 1.813e8, 2.697e6, 0.01343 }, true, 12.5484, 0.9989, 3, 0.9971, 2.0452, OFF },			//29
	{ { 0.004023, -1.948, 300.2 }, false, 173.06, 0.9849, 6, 0.9799, 5.3706, OFF },											//30
	{ { 12.07, -36.2, 91.49 }, true, 173.06, 0.9849, 6, 0.9799, 5.3706, OFF },												//31
	{ { 0.002812, -1.5, 261.9 }, false, 86.5295, 0.9924, 6, 0.9899, 3.7976, LAR },											//32
	{ { 8.437, -32.86, 93.88 }, true, 86.5295, 0.9924, 6, 0.9899, 3.7976, LAR },											//33
	{ { 0.003823, -1.874, 293.8 }, false, 230.0, 0.9799, 6, 0.9732, 6.1913, BIS },											//34
	{ { 11.47, -35.64, 91.77 }, true, 230.0, 0.9799, 6, 0.9732, 6.1913, BIS },												//35
	{ { -3.977e-5, 0.02311, -4.185, 433.1 }, false, 28.7316, 0.9975, 5, 0.9960, 2.3971, OFF },								//36
	{ { -6.535, 12.07, -25.91, 91.49 }, true, 28.7316, 0.9975, 5, 0.9960, 2.3971, OFF },									//37
	{ { -5.777, 13.12, -27.73, 90.5 }, true, 39.8943, 0.9965, 5, 0.9944, 2.8247, LAR },										//38
	{ { -3.954e-5, 0.02301, -4.8, 432.4 }, false, 33.1868, 0.9971, 5, 0.9954, 2.5763, BIS },								//39
	{ { -6.497, 12.09, -25.98, 91.45 }, true, 33.1868, 0.9971, 5, 0.9954, 2.5763, BIS },									//40
	{ { 1.085e4, -0.941 }, false, 30.8838, 0.9973, 7, 0.9969, 2.1005, OFF },												//41
	{ { 1.021e4, -0.9246, -1.928 }, false, 30.8123, 0.9973, 6, 0.9964, 2.2661, OFF },										//42
	{ { 3.614, 1.436e4, 3.334 }, false, 31.6367, 0.9972, 6, 0.9963, 2.2963, OFF },											//43
	{ { -5.79e5, 1.735e6, -4.386e6, -4.974e4 }, true, 171.07, 0.9849, 5, 0.9758, 5.8834 },									//44
	{ { -0.0005358, 0.03557, 59.28, 3507, -34.01 }, false, 12.9893, 0.9989, 4, 0.9977, 1.8020 }								//45
};

/**********************some parameters about data fitting************************/

#define PSAM_B	0				//�ϰ�����������
#define PSAM_G	1000			//��ɫ����������
#define PSAM_R	1000			//��ɫ����������
#define NSAM	2600			//����������

#define SAM_W	64				//������
#define SAM_H	64				//������

#define I_W	640					//ͼ���
#define I_H	480					//ͼ���

#define WIN		Size(32,32)		//���ڴ�Сԭ��(64,64)
#define BLOCK	Size(16,16)		//���С
#define STEP	Size(8,8)		//����	ԭ��(8,8)
#define CELL	Size(8,8)		//ϸ����С
#define HBIN	9				//ֱ��ͼ������

extern CvSVM mySVM;

extern Mat Text_Win;			//�����ͼƬ

extern HOGDescriptor myHOG;

extern vector<Rect>foundr;
extern vector<Rect>foundg;
extern vector<Rect>Obj_Out;

extern int extradim;				//����ά��

#endif