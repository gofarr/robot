#include "S_C.h"

Mat HogMat;					//��ȡ�����ľ���
Mat sampleFeatureMat;		//����ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��
Mat sampleLabelMat;			//ѵ����������������������������������ĸ�������������1��0-r  1-g  2-b  3-neg

HOGDescriptor hog;			//hog������
vector<float> descriptors;	//HOG����������
int DescriptorDim = 0;		//������ά��
int extradim = 0;		//����ά��

int supportvector = 0;			//֧����������

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
	hog.winSize = WIN;								//��ⴰ�ڴ�С
	hog.blockSize = BLOCK;							//���С��Ŀǰֻ֧��Size(16, 16)
	hog.blockStride = STEP;							//��Ļ�����������Сֻ֧���ǵ�Ԫ��cell_size��С�ı���
	hog.cellSize = CELL;							//��Ԫ��Ĵ�С��Ŀǰֻ֧��Size(8, 8)
	hog.nbins = HBIN;								//9 only
	hog.derivAperture = 1;							//
	hog.winSigma = -1.0;							//��˹�˲����ڵĲ���
	hog.histogramNormType = 0;
	hog.L2HysThreshold = 0.20000000000000001;		//����ֱ��ͼ��һ������L2-Hys�Ĺ�һ��������
	hog.gammaCorrection = true;						//�Ƿ�gammaУ��
	hog.nlevels = 64;								//��ⴰ�ڵ��������
}
void Hog_Get(int flag, int Hog_N, int offset, Size win, Size block, Size step, Size cell)
{
	char filename[100] = "";
	for (int i = 0; i < Hog_N; i++)		//���μ���ͼƬ����ȡHOG����
	{
		switch (flag)
		{
		case 0:		//r
			sprintf(filename, "../../../SAM/POS_R/Pos_R%d.jpg", i + 1);
			sampleLabelMat.at<float>(i + offset, 0) = 0;		//��ǩ��ֵ
			break;
		case 1:		//g
			sprintf(filename, "../../../SAM/POS_G/Pos_G%d.jpg", i + 1);
			//sampleLabelMat.at<float>(i + offset, 0) = 1;		//��ǩ��ֵ
			sampleLabelMat.at<float>(i + offset, 0) = 0;		//��ǩ��ֵ
			break;
		case 2:		//b
			sprintf(filename, "../../../SAM/POS_B/Pos_B%d.jpg", i + 1);
			sampleLabelMat.at<float>(i + offset, 0) = 2;		//��ǩ��ֵ
			break;
		case 3:		//neg
			sprintf(filename, "../../../SAM/NEG/NEG%d.jpg", i + 1);
			sampleLabelMat.at<float>(i + offset, 0) = 3;		//��ǩ��ֵ
			break;
		default:break;
		}

		Mat HogMattemp = imread(filename);	//��������ͼ
		resize(HogMattemp, HogMat, WIN, 0, 0, INTER_CUBIC);
		hog.compute(HogMat, descriptors, Size(16, 16));			//��ⴰ���ƶ�����  ԭ��(32,32)
		for (int j = 0; j < DescriptorDim - extradim; j++)
			sampleFeatureMat.at<float>(i + offset, j) = descriptors[j];		//��i��ͼƬ�����������еĵ�j

		/*vector<float>v;
		RGB_AverCal(HogMat, v);
		for (int pp = 0; pp < v.size(); pp++)
			sampleFeatureMat.at<float>(i + offset, DescriptorDim - extradim + pp) = v[pp];*/
		printf("��%d��HOG��ȡ���\n", i + 1);
	}
	return;
}
int SVM_Train(char *filename, Mat sampleFeatureMat, Mat sampleLabelMat)
{
	CvSVM svm;
	CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
	CvSVMParams param = CvSVMParams(CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.06, 1.0, 10.0, 0.5, 1.0, NULL, criteria);	//1.12  10.0
	//CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.09, 1.0, 10.0, 0.5, 1.0, NULL, criteria

	//cout << "��ʼѵ��SVM������" << endl;
	svm.train(sampleFeatureMat, sampleLabelMat, Mat(), Mat(), param);
	svm.save(filename);//��ѵ���õ�SVMģ�ͱ���Ϊxml�ļ�
	//cout << "ѵ�����" << endl;
	return svm.get_support_vector_count();
}
void HogReadtxt(char *filename)
{
	SYSTEMTIME Time = { 0 };
	GetLocalTime(&Time);			//��õ�ǰ����ʱ��

	ofstream fout(filename);		//�������ߴ��ļ�

	fout << "Hog :" << "\n";

	fout << "Some notes about this classifier:" << "\n\n";
	fout << "Time :";
	fout << Time.wYear << "." << Time.wMonth << "." << Time.wDay << "  " << Time.wHour << ":" << Time.wMinute << ":" << Time.wSecond << "\n\n";
	fout << "Red Sample :" << PSAM_R << "\n";
	fout << "Green Sample :" << PSAM_G << "\n";
	fout << "Barrie Sample :" << PSAM_B << "\n";
	fout << "Negative Sample :" << NSAM << "\n\n";

	fout << "The dimension of the descriptor : " << DescriptorDim << "\n";		//������ά��
	fout << "The number of the supportvector :" << supportvector << "\n\n";		//֧����������

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