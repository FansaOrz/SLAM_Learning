#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <boost/format.hpp>
#include <Sophus/sophus/se3.hpp>
#include <pangolin/pangolin.h>
#include <Eigen/Core>

using namespace std;
typedef vector<Sophus::SE3d, Eigen::aligned_allocator<Sophus::SE3d>> TrajectoryType;
typedef Eigen::Matrix<double, 6, 1> Vector6d;
typedef vector<Vector6d, Eigen::aligned_allocator<Vector6d>> PointCloudType;

void showPointCloud(const PointCloudType &pointcloud);

int main(int argc, char **argv){
    vector<cv::Mat> colorImgs, depthImgs;
    TrajectoryType poses;

    ifstream fin("./poses.txt");
    if (!fin){
        cerr << "请在poses.txt目录下运行" << endl;
        // 函数非正常终止
        return 1;
    }
    
    for (size_t i = 0; i < 5; i++){
        boost::format fmt("./%s/%d.%s");
        colorImgs.push_back(cv::imread((fmt % "color" % (i + 1) % "png").str()));
        // flags = -1 读取原始图像
        depthImgs.push_back(cv::imread((fmt % "depth" % (i + 1) % "pgm").str(), -1));
        double data[7] = {0};
        // 遍历7长度的data数组，每次都输入一次fin里面的值
        for (auto &d : data){
            fin >> d;
        }
        Sophus::SE3d pose(Eigen::Quaterniond(data[6], data[3], data[4], data[5]), Eigen::Vector3d(data[0], data[1], data[2]));
        poses.push_back(pose);
    }

    // 计算点云并拼接
    // 内参
    double cx = 325.5, cy = 253.5, fx = 518.0, fy = 519.0;
    double depthScale = 1000.0;
    PointCloudType pointcloud;
    pointcloud.reserve(1000000);

    for (size_t i = 0; i < 5; i++){
        cout << "converting image ..." << endl;
        cv::Mat color = colorImgs[i];
        cv::Mat depth = depthImgs[i];
        Sophus::SE3d T = poses[i];
        for (int v = 0; v < color.rows; v++){
            for (int u = 0; u < color.cols; u++){
                unsigned int d = depth.ptr<unsigned short>(v)[u];
                // 如果深度是0，代表没有测量到
                if (d == 0) continue;
                Eigen::Vector3d point;
                point[2] = double(d) / depthScale;
                point[0] = (u - cx) * point[2] / fx;
                point[1] = (v - cy) * point[2] / fy;
                Eigen::Vector3d pointWorld = T * point;

                Vector6d p;
                p.head<3>() = pointWorld;
                p[5] = color.data[v * color.step + u * color.channels()];
                p[4] = color.data[v * color.step + u * color.channels() + 1];
                p[3] = color.data[v * color.step + u * color.channels() + 2];
                pointcloud.push_back(p);
            }
        }
    }
    cout << "total num of pointcloud is " << pointcloud.size() << endl;
    showPointCloud(pointcloud);
    return 0;
 }

void showPointCloud(const PointCloudType &pointcloud) {

    if (pointcloud.empty()) {
        cerr << "Point cloud is empty!" << endl;
        return;
    }

    pangolin::CreateWindowAndBind("Point Cloud Viewer", 1024, 768);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pangolin::OpenGlRenderState s_cam(
        pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
        pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0)
    );

    pangolin::View &d_cam = pangolin::CreateDisplay()
        .SetBounds(0.0, 1.0, pangolin::Attach::Pix(175), 1.0, -1024.0f / 768.0f)
        .SetHandler(new pangolin::Handler3D(s_cam));

    while (pangolin::ShouldQuit() == false) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        d_cam.Activate(s_cam);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glPointSize(2);
        glBegin(GL_POINTS);
        for (auto &p: pointcloud) {
            glColor3d(p[3] / 255.0, p[4] / 255.0, p[5] / 255.0);
            glVertex3d(p[0], p[1], p[2]);
        }
        glEnd();
        pangolin::FinishFrame();
        usleep(5000);   // sleep 5 ms
    }
    return;
}