

#include <sstream> 
#include <opencv2/opencv.hpp>
#include <iostream>
#include <zbar.h>

using namespace cv;
using namespace std;
using namespace zbar;



string recognise(Mat resultImage)
{

	//imshow("条形码图像", resultImage);
	string information;
	if (!resultImage.empty())
	{
		ImageScanner scanner;
		scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
		Mat imageGray;
		cvtColor(resultImage, imageGray, CV_RGB2GRAY);

		int width = imageGray.cols;   // extract dimensions
		int height = imageGray.rows;
		uchar *raw = (uchar *)imageGray.data;
		Image imageZbar(width, height, "Y800", raw, width*height);

		scanner.scan(imageZbar); //扫描条码    
		Image::SymbolIterator symbol = imageZbar.symbol_begin();
		if (imageZbar.symbol_begin() == imageZbar.symbol_end())
		{
			cout << "查询条码失败，请检查图片！" << endl;
			information = "N";
		}
		for (; symbol != imageZbar.symbol_end(); ++symbol)
		{
			//cout << "类型：" << endl << symbol->get_type_name() << endl << endl;
			//cout << "条码：" << endl << symbol->get_data() << endl << endl;
			information = symbol->get_data();
		}

		//cout << "条码：" << endl << information << endl << endl;
		//imshow("Source Image", resultImage);
		//waitKey();
		imageZbar.set_data(NULL, 0);
		return information;
	}
}