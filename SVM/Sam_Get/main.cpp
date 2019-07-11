#include "Sam.h"

int main(void)
{
	namedWindow("src");
	namedWindow("dst");
	namedWindow("temp");

	Mat_Init();

	cout << "The label of this picture:\n";
	cout << "0--Red\n" << "1--Green\n" << "2--Barrier\n" << "3--Neg\n";
	//d¼ü×÷·Ï	b¼üÍË³ö

	//cout << "Red start!\n";
	//Sam_R_Get("../../../video/RedSam3.avi", SAM_W, SAM_H, 993, PSAM_R, 0);		//PSAM_R
	//cout << "Red over!\n";

	cout << "Green start!\n";
	Sam_R_Get("../../../video/GreSam3.avi", SAM_W, SAM_H, 667, PSAM_G, 1);	//PSAM_G
	cout << "Green over!\n";

	//cout << "Barrier start!\n";
	//Sam_R_Get("../../../video/YYF_4.avi", SAM_W, SAM_H, 1, PSAM_B, 2);
	//cout << "Barrier over!\n";

	//cout << "Neg start!\n";
	//Sam_R_Get("../../../video/YYF_4.avi", SAM_W, SAM_H, 1, NSAM, 3);
	//cout << "Neg over!\n";

	destroyAllWindows();

	return 0;
}