// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <opencv2/opencv.hpp>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
using namespace cv;
void showCVImage() {
    Mat image;
    image = imread("testimage.png", IMREAD_UNCHANGED);
    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", image);
    imwrite("testimage_opencv.png", image);
    waitKey(0);
}
Mat src;
int main(int argc, char* argv[]) {
    showCVImage();
}