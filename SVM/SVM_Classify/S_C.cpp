#include "S_C.h"

Mat HogMat;					//提取特征的矩阵
Mat sampleFeatureMat;		//所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数
Mat sampleLabelMat;			//训练样本的类别向量，行数等于所有样本的个数，列数等于1；0-r  1-g  2-b  3-neg

HOGDescriptor hog;			//hog描述子
vector<float> descriptors;	//HOG描述子向量
int DescriptorDim = 0;		//描述子维数
int extradim = 0;		//额外维数

int supportvector = 0;			//支持向量个数

unsigned long dwLast = 1;
unsigned long dwCurrent = 1;

int Hog_Dim_Cal(Size win, Size block, Size step, Size cell, int hbin, int offset)
{
	//hog.getDescriptorSize();
	if (hbin == 0)
	{
		printf("hbin error!\n");
		return 0;
	}
	if (((win.width-block.width) % step.width != 0) || ((win.height-block.height) % step.height != 0))
	{
		printf("win-step error!\n");
		return 0;
	}
	if ((block.width % cell.width != 0) || (block.height % cell.height != 0))
	{
		printf("block-cell error!\n");
		return 0;
	}
	return offset + (block.height / cell.height)*(block.width / cell.width)*((win.width - block.width) / step.width + 1)*((win.height - block.height) / step.height + 1)*(hbin);
}
void Data_Mat_Init(int dim, int samn)
{
	sampleFeatureMat = Mat::zeros(samn, dim, CV_32FC1);
	sampleLabelMat = Mat::zeros(samn, 1, CV_32FC1);
}
void Hog_Init(void)
{
	hog.winSize = WIN;								//检测窗口大小
	hog.blockSize = BLOCK;							//块大小，目前只支持Size(16, 16)
	hog.blockStride = STEP;							//块的滑动步长，大小只支持是单元格cell_size大小的倍数
	hog.cellSize = CELL;							//单元格的大小，目前只支持Size(8, 8)
	hog.nbins = HBIN;								//9 only
	hog.derivAperture = 1;							//
	hog.winSigma = -1.0;							//高斯滤波窗口的参数
	hog.histogramNormType = 0;
	hog.L2HysThreshold = 0.20000000000000001;		//块内直方图归一化类型L2-Hys的归一化收缩率
	hog.gammaCorrection = true;						//是否gamma校正
	hog.nlevels = 64;								//检测窗口的最大数量
}
void Hog_Get(int flag, int Hog_N, int offset, Size win, Size block, Size step, Size cell)
{
	char filename[100] = "";
	for (int i = 0; i < Hog_N; i++)		//依次加载图片，提取HOG特征
	{
		switch (flag)
		{
		case 0:		//r
			sprintf(filename, "../../../SAM/POS_R/Pos_R%d.jpg", i + 1);
			sampleLabelMat.at<float>(i + offset, 0) = 0;		//标签赋值
			break;
		case 1:		//g
			sprintf(filename, "../../../SAM/POS_G/Pos_G%d.jpg", i + 1);
			//sampleLabelMat.at<float>(i + offset, 0) = 1;		//标签赋值
			sampleLabelMat.at<float>(i + offset, 0) = 0;		//标签赋值
			break;
		case 2:		//b
			sprintf(filename, "../../../SAM/POS_B/Pos_B%d.jpg", i + 1);
			sampleLabelMat.at<float>(i + offset, 0) = 2;		//标签赋值
			break;
		case 3:		//neg
			sprintf(filename, "../../../SAM/NEG/NEG%d.jpg", i + 1);
			sampleLabelMat.at<float>(i + offset, 0) = 3;		//标签赋值
			break;
		default:break;
		}

		Mat HogMattemp = imread(filename);	//加载样本图
		resize(HogMattemp, HogMat, WIN, 0, 0, INTER_CUBIC);
		hog.compute(HogMat, descriptors, Size(16, 16));			//检测窗口移动步长  原定(32,32)
		for (int j = 0; j < DescriptorDim - extradim; j++)
			sampleFeatureMat.at<float>(i + offset, j) = descriptors[j];		//第i张图片的特征向量中的第j

		/*vector<float>v;
		RGB_AverCal(HogMat, v);
		for (int pp = 0; pp < v.size(); pp++)
			sampleFeatureMat.at<float>(i + offset, DescriptorDim - extradim + pp) = v[pp];*/
		printf("第%d个HOG提取完成\n", i + 1);
	}
	return;
}
int SVM_Train(char *filename, Mat sampleFeatureMat, Mat sampleLabelMat)
{
	CvSVM svm;
	CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
	CvSVMParams param = CvSVMParams(CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.06, 1.0, 10.0, 0.5, 1.0, NULL, criteria);	//1.12  10.0
	//CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.09, 1.0, 10.0, 0.5, 1.0, NULL, criteria

	//cout << "开始训练SVM分类器" << endl;
	svm.train(sampleFeatureMat, sampleLabelMat, Mat(), Mat(), param);
	svm.save(filename);//将训练好的SVM模型保存为xml文件
	//cout << "训练完成" << endl;
	return svm.get_support_vector_count();
}
void HogReadtxt(char *filename)
{
	SYSTEMTIME Time = { 0 };
	GetLocalTime(&Time);			//获得当前本地时间

	ofstream fout(filename);		//创建或者打开文件

	fout << "Hog :" << "\n";

	fout << "Some notes about this classifier:" << "\n\n";
	fout << "Time :";
	fout << Time.wYear << "." << Time.wMonth << "." << Time.wDay << "  " << Time.wHour << ":" << Time.wMinute << ":" << Time.wSecond << "\n\n";
	fout << "Red Sample :" << PSAM_R << "\n";
	fout << "Green Sample :" << PSAM_G << "\n";
	fout << "Barrie Sample :" << PSAM_B << "\n";
	fout << "Negative Sample :" << NSAM << "\n\n";

	fout << "The dimension of the descriptor : " << DescriptorDim << "\n";		//描述子维数
	fout << "The number of the supportvector :" << supportvector << "\n\n";		//支持向量个数

	fout << "The time that we get this classifier :" << dwCurrent - dwLast << " ms" << "\n\n";

	fout << "gamma correction" << "\t" << hog.gammaCorrection << "\n\n";

	fout << flush;
	fout.close();
	return;
}
void RGB_AverCal(Mat mat, vector<float>&v)
{
	vector<Mat>sp;
	split(mat, sp);
	v.clear();
	//printf("spsize = %d\n", sp.size());
	for (int i = 0; i < sp.size(); i++)
	{
		float temp = 0.;
		for (int a = 0; a < mat.cols; a++)
		for (int b = 0; b < mat.rows; b++)
		{
			temp += (float)(sp[i].at<uchar>(a, b)*1.0);
		}
		temp = temp / (float)(mat.cols*mat.rows*1.0);
		v.push_back(temp);
	}
}