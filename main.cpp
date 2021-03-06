#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace std;
using namespace cv;
/*********************  
计时函数 PrintMs 
************************/
void PrintMs(const char *text = " ")
{
	static long long last = 0;      //
	long long cur = getTickCount();
	if (last == 0)
	{
		last = cur;
		return;
	}
	long long ms = 0;
	ms = ((double)(cur - last) / getTickFrequency()) * 1000;
	if (*text != 0)
	{
		printf("%s =  %dms\n", text, ms);
	}
	last = getTickCount();

}

int main(int argc, char *argv)
{
	Mat mat(3000, 4000, CV_8UC3);
	//mat.create(3000, 4000, CV_8UC3); // 另外一种创建的方法
	int  mat_size = mat.elemSize();
	int  size = mat.rows * mat.cols *mat_size;
	PrintMs();
	//遍历连续的mat空间
	for (int i = 0; i < size; i += mat_size)
	{
		mat.data[i] = 255; //blue
		mat.data[i + 1] = 0; //green
		mat.data[i + 2] = 0; //red

	}

	PrintMs("mat.data ms");

	// 直接地址遍历不一定连续的空间
	for (int row = 0; row < mat.rows; row++)
	{
		for (int col = 0; col < mat.cols; col++)
		{
			(&mat.data[row*mat.step])[col*mat_size] = 0; //B
			(&mat.data[row*mat.step])[col*mat_size+1] = 0;
			(&mat.data[row*mat.step])[col*mat_size+2] = 255;

		}
	}
	PrintMs("mat.step ms");

	// 通过mat.ptr 接口遍历mat
	for (int row = 0; row < mat.rows; row++)
	{
		for (int col = 0; col < mat.cols; col++)
		{
			Vec3b *c = mat.ptr<Vec3b>(row, col);
			c->val[0] = 0;
			c->val[1] = 255;
			c->val[2] = 0;


		}
	}
	PrintMs("mat.ptr ms");



	try
	{
		// 通过mat.at 接口遍历mat
		for (int row = 0; row < mat.rows; row++)
		{
			for (int col = 0; col < mat.cols; col++)
			{
				Vec3b &m = mat.at<Vec3b>(row, col);
				m[0] = 100;
				m[1] = 100;
				m[2] = 100;


			}
		}
		PrintMs("mat.at ms");

	}
	catch (Exception &ex)
	{
		cout << ex.what() << endl;
	}

	//迭代器遍历方法
	PrintMs( );
	auto it = mat.begin<Vec3b>();
	auto it_end = mat.end<Vec3b>();
	for (; it != it_end; it++)
	{
		(*it).val[0] = 0;
		(*it).val[1] = 222;
		(*it).val[2] = 0;
	}

	PrintMs("mat.begin ms");
	namedWindow(" mat");  //创建一个窗口，
	imshow("mat", mat);   // 显示这个窗口
	waitKey(0);

	return 0;
}
