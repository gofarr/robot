#include "IARC_Obj_Dec.h"
#include <windows.h>

int framecount = 0;
int main(void)
{
	extradim = 0;

	unsigned long dwLast = 1;
	unsigned long dwCurrent = 1;

	//SVM_Load(&mySVM, "../../../SVMXML/SVM_Data6/SVM_Data6.xml");		//����SVM
	SVM_Load(&mySVM, "../../../SVMXML/SVM_Data9/SVM_Data9.xml");		//����SVM
	Hog_Init();
	Vec_Init();

	char filename[100] = "";
	//����ͼƬ����
	VideoCapture capture("../../../video/YYF_100.avi");

	if (!capture.isOpened())
		cout << "fail to open!" << endl;

	long frametotal = capture.get(CV_CAP_PROP_FRAME_COUNT);		//��֡��

	cout << "������Ƶ��" << frametotal << "֡" << endl;

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
			Multi_Scale_Obj_Dec(height, srcimg, myHOG, { 0, 0 }, WIN, Size(16, 16), 1.1, 2, 1, 1);	//ԭ��Size(32,32)
		//Multi_Scale_Obj_Dec2(height, srcimg, myHOG, { 0, 0 }, WIN, Size(16, 16), 1.1, 2, 1, 1);
		dwCurrent = GetTickCount();
		printf("ʱ�䣺 %d ms\n", dwCurrent - dwLast);
		cout << "***********end!***********\n";
		framecount++;
		//cvWaitKey(50);
		cvWaitKey(0);
	}
	//for (int i = 1; i <= 20; i++)
	//{
	//	printf("��ʼ��i = %d\n", i);
	//	sprintf(filename, "../../../ScreenShot/%d.jpg", i);
	//	Text_Win = imread(filename);	//��������ͼƬ
	//	dwLast = GetTickCount();
	//	Multi_Scale_Obj_Dec(80, Text_Win, myHOG, { 0, 0 }, WIN, Size(16, 16), 1.1);	//ԭ��Size(32,32)
	//	//Multi_Scale_Obj_Dec2(80,Text_Win, myHOG, { 0, 0 }, WIN, Size(32, 32), 1.1);
	//	dwCurrent = GetTickCount();
	//	printf("ʱ�䣺 %d ms\n", dwCurrent - dwLast);
	//	waitKey(500);
	//}

	destroyAllWindows();

	return 0;
}