// This file was originally taken from https://github.com/juho-p/simple-dds-image-reader then modifier by FragJacker.
#include "includes/opencv_image.h"
#include "includes/dds_img_reader.h"
#include <opencv2/core/opengl.hpp>
// Global
int last_col = 0, last_row = 0;

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

/// <summary>
/// Reconstructs the alpha channel from a 8.8.8.8 DDS image since OpenCV is weird with alpha channels.
/// </summary>
/// <param name="img"></param>
void reconstructAlphaChannel(cv::Mat& img, alpha_mode mode)
{
	Mat channels[4];
	split(img, channels);
	Mat alpha = channels[3];
	cvtColor(img, img, COLOR_BGRA2RGB);
	switch (mode)
	{
	case DDSRGB:
	{
		Mat newchan[] = { channels[2], channels[1], channels[0], alpha };
		merge(newchan, 4, img);
	}
	break;
	case DDSDXT:
	{
		Mat newchan[] = { channels[0], channels[1], channels[2], alpha };
		merge(newchan, 4, img);
	}
	break;
	default:
		break;
	}
}

// Loads a single image from a given path and adds it to the Imagelist obj.
int loadimage(Imagelist& imglist, string dirpath, string extension) {
	Mat img;
	// Handle the case of 16 bits per channel unsigned images.
	if (extension.compare("dds") == 0)
	{
		Image dds_img;
		dds_img = read_dds(readFile(dirpath.c_str()));
		switch (dds_img.bpp)
		{
		case 0:
			img = Mat(dds_img.height, dds_img.width, CV_8UC4, dds_img.data.data());
			reconstructAlphaChannel(img, DDSDXT);
			break;
		case 24:
			img = Mat(dds_img.height, dds_img.width, CV_8UC3, dds_img.data.data());
			cvtColor(img, img, COLOR_BGR2RGBA);
			break;
		case 32:
		{
			img = Mat(dds_img.height, dds_img.width, CV_8UC4, dds_img.data.data());
			reconstructAlphaChannel(img, DDSRGB);
		}
		break;
		default:
			break;
		}
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
	// Bad image dimension handling
	if (last_col == 0 && last_row == 0)
	{
		last_col = img.cols;
		last_row = img.rows;
	}
	if (img.cols == last_col && img.rows == last_row)
	{
		imglist.addimage(img);
		return 0;
	}
	else
	{
		return 1;
	}
}

/// <summary>
/// Resets the global variables back to zero once we finished reading all selected images, getting ready for the next batch.
/// </summary>
void resetColsAndRows()
{
	last_col = 0;
	last_row = 0;
}

// Stitch all images together that exist in the Imagelist obj.
Mat* stitchimages(Imagelist& imglist, int rows, int cols) {
	Mat* result;
	list<Mat> worklist = imglist.getList();
	worklist.reverse();
	ImageCells cells = ImageCells(rows, cols, worklist.begin()->cols, worklist.begin()->rows);
	list<Mat>::iterator it = worklist.begin();
	for (int j = 0; j < cells.getrows(); ++j)
	{
		for (int i = 0; i < cells.getcols(); ++i)
		{
			if (it != worklist.end())
			{
				cells.setCell(i, j, *it);
				++it;
			}
			else break;
		}
	}
	//imshow("cells.image", temp_img);
	//vector<int> compression_params;
	//compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	//compression_params.push_back(9);
	//imwrite("spritesheet.png", temp_img, compression_params);
	//waitKey();
	result = new Mat(*cells.getImage());
	return result;
}