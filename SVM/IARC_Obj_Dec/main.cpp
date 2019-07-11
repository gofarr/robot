#include "IARC_Obj_Dec.h"
#include <windows.h>

int framecount = 0;
int main(void)
{
	extradim = 0;

	unsigned long dwLast = 1;
	unsigned long dwCurrent = 1;

	//SVM_Load(&mySVM, "../../../SVMXML/SVM_Data6/SVM_Data6.xml");		//加载SVM
	SVM_Load(&mySVM, "../../../SVMXML/SVM_Data9/SVM_Data9.xml");		//加载SVM
	Hog_Init();
	Vec_Init();

	char filename[100] = "";
	//先用图片测试
	VideoCapture capture("../../../video/YYF_100.avi");

	if (!capture.isOpened())
		cout << "fail to open!" << endl;

	long frametotal = capture.get(CV_CAP_PROP_FRAME_COUNT);		//总帧数

	cout << "整个视频共" << frametotal << "帧" << endl;

	Mat srcimg;

	while (1)
	{
		if (!capture.read(srcimg))
		{
			break;
		}
		cout << "**********start!**********\n";
		cout << "framecount = " <<framecount<<"\n";
		dwLast = GetTickCount();
		int height = 80;
		//if (framecount == 30 || framecount == 32)
			Multi_Scale_Obj_Dec(height, srcimg, myHOG, { 0, 0 }, WIN, Size(16, 16), 1.1, 2, 1, 1);	//原来Size(32,32)
		//Multi_Scale_Obj_Dec2(height, srcimg, myHOG, { 0, 0 }, WIN, Size(16, 16), 1.1, 2, 1, 1);
		dwCurrent = GetTickCount();
		printf("时间： %d ms\n", dwCurrent - dwLast);
		cout << "***********end!***********\n";
		framecount++;
		//cvWaitKey(50);
		cvWaitKey(0);
	}
	//for (int i = 1; i <= 20; i++)
	//{
	//	printf("开始：i = %d\n", i);
	//	sprintf(filename, "../../../ScreenShot/%d.jpg", i);
	//	Text_Win = imread(filename);	//载入待检测图片
	//	dwLast = GetTickCount();
	//	Multi_Scale_Obj_Dec(80, Text_Win, myHOG, { 0, 0 }, WIN, Size(16, 16), 1.1);	//原来Size(32,32)
	//	//Multi_Scale_Obj_Dec2(80,Text_Win, myHOG, { 0, 0 }, WIN, Size(32, 32), 1.1);
	//	dwCurrent = GetTickCount();
	//	printf("时间： %d ms\n", dwCurrent - dwLast);
	//	waitKey(500);
	//}

	destroyAllWindows();

	return 0;
}