#include "includes/opencv_image.h"
#include "includes/dds_img_reader.h"
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

std::vector<BYTE> readFile(const char* filename)
{
	// open the file:
	std::ifstream file(filename, std::ios::binary);

	// Stop eating new lines in binary mode!!!
	file.unsetf(std::ios::skipws);

	// get its size:
	std::streampos fileSize;

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// reserve capacity
	std::vector<BYTE> vec;
	vec.reserve(fileSize);

	// read the data:
	vec.insert(vec.begin(),
		std::istream_iterator<BYTE>(file),
		std::istream_iterator<BYTE>());

	return vec;
}

// Loads a single image from a given path and adds it to the Imagelist obj.
void loadimage(Imagelist& imglist, string dirpath, string extension) {
	Mat img;
	// Handle the case of 16 bits per channel unsigned images.
	if (extension.compare("dds") == 0)
	{
		Image dds_img;
		dds_img = read_dds(readFile(dirpath.c_str()));
		img = Mat(dds_img.height, dds_img.width, CV_8UC3, dds_img.data.data());
		cvtColor(img, img, COLOR_BGR2RGBA);
	}
	if (extension.compare("png") == 0)
	{
		img = imread(dirpath, IMREAD_UNCHANGED);
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
	}
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
	delete cells;
	return temp_img;
}