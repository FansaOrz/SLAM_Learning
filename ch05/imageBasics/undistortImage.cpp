/*
本程序实现去畸变部分的代码。
尽管我们可以调用OpenCV的去畸变，但自己实现一遍有助于理解。
*/
#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

string image_file = "/home/fansa/Src/SLAM_Learning/ch5/imageBasics/distorted.png";

int main(int argc, char **argv){
    // 畸变参数
    double k1 = -0.28340811, k2 = 0.07395907, p1 = 0.00019359, p2 = 1.76187114e-05;
    // 内参数
    double fx = 458.654, fy = 457.296, cx = 367.215, cy = 248.375;
    // 图像是灰度图，CV_8UC1
    /*
        imread flags参数含义
        IMREAD_UNCHANGED            = -1, 
        IMREAD_GRAYSCALE            = 0,  
        IMREAD_COLOR                = 1,  
        IMREAD_ANYDEPTH             = 2,  
        IMREAD_ANYCOLOR             = 4,  
        IMREAD_LOAD_GDAL            = 8,  
        IMREAD_REDUCED_GRAYSCALE_2  = 16, 
        IMREAD_REDUCED_COLOR_2      = 17, 
        IMREAD_REDUCED_GRAYSCALE_4  = 32, 
        IMREAD_REDUCED_COLOR_4      = 33, 
        IMREAD_REDUCED_GRAYSCALE_8  = 64, 
        IMREAD_REDUCED_COLOR_8      = 65, 
        IMREAD_IGNORE_ORIENTATION   = 128 
    */  
    cv::Mat image = cv::imread(image_file, 0);
    int rows = image.rows, cols = image.cols;
    // 灰度图，所以使用CV_8UC1就可以
    cv::Mat image_undistort = cv::Mat(rows, cols, CV_8UC1);

    // 计算去畸变后的图像
    for (size_t v = 0; v < rows; v++){
        for (size_t u = 0; u < cols; u++){
            /*
            对于新的图片中每一个像素点(u, v)，计算出其对应于原图片中的像素地址，然后赋值
            */

            // 按照公式，计算点(u,v)对应到畸变图像中的坐标(u_distorted, v_distorted)
            double x = (u - cx) / fx, y = (v - cy) / fy;
            double r = sqrt(x * x + y * y);
            double x_distorted = x * (1 + k1 * r * r + k2 * r * r * r * r) + 2 * p1 * x * y + p2 * (r * r + 2 * x * x);
            double y_distorted = y * (1 + k1 * r * r + k2 * r * r * r * r) + p1 * (r * r + 2 * y * y) + 2 * p2 * x * y;
            double u_distorted = fx * x_distorted + cx;
            double v_distorted = fy * y_distorted + cy;
            
            // 赋值 (最近邻插值)
            if (u_distorted >= 0 && v_distorted >=0 && u_distorted < cols && v_distorted < rows){
                image_undistort.at<uchar>(v, u) = image.at<uchar>((int) v_distorted, (int) u_distorted);
            }
            else{
                image_undistort.at<uchar>(v, u) = 0;
            }
        }
    }

    // 画图去畸变后图像
    cv::imshow("distorted", image);
    cv::imshow("undistorted", image_undistort);
    cv::waitKey();
    return 0;
}