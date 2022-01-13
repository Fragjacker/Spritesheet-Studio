#include "includes/opencv_image.h"
#include <opencv2/core/opengl.hpp>
// Global

Mat src;
void showCVImage() {
	Mat image;
	image = imread("testimage.png", IMREAD_UNCHANGED);
	namedWindow("Display Image", WINDOW_AUTOSIZE);
	imshow("Display Image", image);
	imwrite("testimage_opencv.png", image);
	waitKey(0);
}

// Loads a single image from a given path and adds it to the Imagelist obj.
void loadimage(Imagelist& imglist, const char* dirpath) {
	Mat img;
	img = imread(string(dirpath), IMREAD_UNCHANGED);
	// Handle the case of 16 bits per channel unsigned images.
	switch (img.depth())
	{
	case CV_16U:
		img.convertTo(img, CV_8UC4, 1 / 256.0);
		break;
	case CV_32F:
		img.convertTo(img, CV_8UC4, 1 / 16777216.0);
		break;
	default:
		break;
	}
	cvtColor(img, img, COLOR_BGR2BGRA);
	imglist.addimage(img);
};

// Stitch all images together that exist in the Imagelist obj.
Mat stitchimages(Imagelist& imglist, int rows, int cols) {
	Mat temp_img, new_img;
	list<Mat> worklist = imglist.getList();
	ImageCells* cells = new ImageCells(rows, cols, worklist.begin()->cols, worklist.begin()->rows);
	list<Mat>::iterator it = worklist.begin();
	for (int j = 0; j < cells->getrows(); ++j)
	{
		for (int i = 0; i < cells->getcols(); ++i)
		{
			if (it != worklist.end())
			{
				cells->setCell(i, j, *it);
				++it;
			}
			else break;
		}
	}
	imshow("cells.image", cells->getImage());
	imwrite("merged_image.png", cells->getImage());
	waitKey();
	return temp_img;
}