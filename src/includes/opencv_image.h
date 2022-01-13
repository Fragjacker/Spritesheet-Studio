#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

class Imagelist;
class Imagecells;

class Imagelist {
private:
	list<Mat> imglist;
public:
	void addimage(Mat& mat) {
		imglist.push_front(mat);
	};

	int listsize() {
		return int(imglist.size());
	};

	list<Mat> getList() {
		return imglist;
	}
};

class ImageCells {
private:
	int  m_width;
	int  m_height;
	int  m_cols;
	int  m_rows;
	Mat image;
public:
	ImageCells(int rows, int cols, int width, int height) {
		image = Mat::zeros(rows * height, cols * width, CV_8UC4);
		m_width = width;
		m_height = height;
		m_cols = cols;
		m_rows = rows;
	}
	virtual ~ImageCells() {};

	int getrows() { return m_rows; }
	int getcols() { return m_cols; }

	void setCell(int col, int row, Mat& img) {
		if (img.cols == m_width && img.rows == m_height) {
			Mat roi = image(Rect(col * m_width, row * m_height, m_width, m_height));
			img.copyTo(roi);
		}
	}

	Mat getCell(int col, int row) {
		Mat roi = image(Rect(col * m_width, row * m_height, m_width, m_height));
		return roi.clone();
	}

	void setImage(Mat img) {
		if (img.cols >= image.cols && img.rows >= image.rows)
			img.copyTo(image);
	}

	Mat getImage() {
		return image;
	}
};

void showCVImage();
void loadimage(Imagelist& imglist, const char* dirpath);
Mat stitchimages(Imagelist& imglist, int rows, int cols);