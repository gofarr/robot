#include "S_C.h"

int main(void)
{
	extradim = 0;
	DescriptorDim = Hog_Dim_Cal(WIN, BLOCK, STEP, CELL, HBIN, extradim);
	cout << "The dimension of the descriptor = " << DescriptorDim << endl;
	
	Data_Mat_Init(DescriptorDim, PSAM_B + PSAM_G + PSAM_R + NSAM);		//初始化数据和标签矩阵

	Hog_Init();

	cout << "Start taking red sample!" << endl;
	dwLast = GetTickCount();
	Hog_Get(0, PSAM_R, 0, WIN, BLOCK, STEP, CELL);
	dwCurrent = GetTickCount();
	cout << "Finish taking red sample!" << endl;
	cout << "Time: " << dwCurrent - dwLast << " ms" << endl;

	cout << "Start taking green sample!" << endl;
	Hog_Get(1, PSAM_G, PSAM_R, WIN, BLOCK, STEP, CELL);
	cout << "Finish taking green sample!" << endl;

	cout << "Start taking barrier sample!" << endl;
	Hog_Get(2, PSAM_B, PSAM_R + PSAM_G, WIN, BLOCK, STEP, CELL);
	cout << "Finish taking barrier sample!" << endl;

	cout << "Start taking negative sample!" << endl;
	Hog_Get(3, NSAM, PSAM_R + PSAM_G + PSAM_B, WIN, BLOCK, STEP, CELL);
	cout << "Finish taking negative sample!" << endl;

	char filename[100] = "../../../SVMXML/SVM_Data9/SVM_Data9.xml";

	cout << "Start training!" << endl;
	dwLast = GetTickCount();
	supportvector = SVM_Train(filename, sampleFeatureMat, sampleLabelMat);
	dwCurrent = GetTickCount();
	cout << "Finish training!" << endl;

	strcpy(filename, "../../../SVMXML/SVM_Data9/ReadMe.txt");

	printf("开始写入说明文档！\n");
	HogReadtxt(filename);
	printf("写入说明文档结束！\n");
	
	//char filename[100] = "";
	//char filename1[100] = "";
	//
	//vector<float>v1;
	//for (int i = 0; i < 600; i++)
	//{
	//	sprintf(filename, "../../../SAM/POS_R/Pos_R%d.jpg", i + 1);
	//	sprintf(filename1, "../../../SAM/POS_G/Pos_G%d.jpg", i + 1);
	//	Mat img = imread(filename);
	//	Mat img1 = imread(filename1);
	//	
	//	RGB_AverCal(img1, v1);
	//	cout << "===============================\n";
	//	cout << "Red: \t" << "R: " << v[0] << " G: " << v[1] << " B: " << v[2] << endl;
	//	//cout << "PPP" << endl;
	//	cout << "Green: \t" << "R: " << v1[0] << " G: " << v1[1] << " B: " << v1[2] << endl;
	//	cout << "===============================\n";
	//	imshow("img", img);
	//	imshow("img1", img1);
	//	waitKey(0);
	//}
	
	return 0;
}