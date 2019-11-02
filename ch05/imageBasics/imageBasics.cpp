#include <iostream>
/*
c++11时间相关库(chrono)
chrono 库主要包含了三种类型：
    时间间隔 Duration
    时钟 Clocks
    时间点 Time point
*/
#include <chrono>

using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char **argv){
    // 读取argv指定的图像
    cv::Mat image;
    image = cv::imread(argv[1]);

    if (image.data == nullptr){
        cerr << "文件" << argv[1] << "不存在\n";
        return 0;
    }
    cout << "col is: " << image.cols << "row is " << image.rows << 
        "channel is " << image.channels() << endl;
    cv::imshow("image", image);
    cv::waitKey(0);
    // 遍历图像, 请注意以下遍历方式亦可使用于随机像素访问
    // 使用 std::chrono 来给算法计时
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    for (size_t y = 0; y < image.rows; y++){
        // 用cv::Mat::ptr获得图像的行指针
        unsigned char *row_ptr = image.ptr<unsigned char>(y);
        for (size_t x = 0; x < image.cols; x++){
            // data_ptr 指向待访问的像素数据
            // 生成的同样是一个指针，所以需要取地址
            unsigned char *data_ptr = &row_ptr[x * image.channels()];
            // 输出该像素的每个通道,如果是灰度图就只有一个通道
            for (size_t c = 0; c != image.channels(); c++){
                unsigned char data = data_ptr[c];
            }
        }
    }
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast <chrono::duration<double>> (t2 - t1);
    cout << "遍历图像用时：" << time_used.count() << " 秒。" << endl;

    // cv Mat拷贝
    // 直接赋值不会拷贝数据
    cv::Mat image_another = image;
    // 修改image_another，image也会对应发生变化
    // 将左上角100*100的块置零
    image_another(cv::Rect(0, 0, 100, 100)).setTo(0);
    cv::imshow("image", image);
    cv::waitKey(0);

    // 使用clone函数来拷贝数据
    cv::Mat image_clone = image.clone();
    image_clone(cv::Rect(0, 0, 100, 100)).setTo(0);
    cv::imshow("image", image);
    cv::imshow("image_clone", image_clone);
    cv::waitKey(0);
  
    // 对于图像还有很多基本的操作,如剪切,旋转,缩放等,限于篇幅就不一一介绍了,请参看OpenCV官方文档查询每个函数的调用方法.
    cv::destroyAllWindows();
    return 0;
}