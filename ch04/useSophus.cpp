/*
Sophus基本用法
*/

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

#include <iostream>
#include <cmath>
#include "../3rdparty/Sophus/sophus/se3.hpp"

using namespace std;
using namespace Eigen;

typedef Eigen::Matrix<double, 6, 1> Vector6d;

int main(int argc, char **argv){
    // 生成一个沿z轴旋转90度的旋转矩阵
    Matrix3d R = AngleAxisd(M_PI / 2, Vector3d(0, 0, 1)).toRotationMatrix();
    // 或者一个四元数
    Quaterniond q(R);
    // SO3d可以直接从旋转矩阵构造或者四元数构造
    Sophus::SO3d SO3d_R(R);
    Sophus::SO3d SO3d_q(q);
    // 二者是等价的
    cout << "SO(3) from matrix:\n" << SO3d_R.matrix() << endl;
    cout << "SO(3) from quaternion:\n" << SO3d_q.matrix() << endl;
    cout << "they are equal" << endl;

    // 使用对数映射获取它的李代数
    Vector3d so3 = SO3d_R.log();
    cout << "so3 = " << so3.transpose() << endl;
    // hat函数是向量到反对称矩阵的变换
    cout << "so3 hat=\n" << Sophus::SO3d::hat(so3) << endl;
    // 相对的，vee为反对称到向量
    cout << "so3 hat vee = " << Sophus::SO3d::vee(Sophus::SO3d::hat(so3)).transpose() << endl;

    // 增量扰动模型的更新
    // 假设这是更新量
    Vector3d update_so3(1e-4, 0, 0);
    // 求指数映射，变成矩阵，然后矩阵相乘，即为更新后的旋转矩阵
    Sophus::SO3d SO3_updated = Sophus::SO3d::exp(update_so3) * SO3d_R;
    cout << "SO3 updated = \n" << SO3_updated.matrix() << endl;

    cout << "*************************************************\n";

    // 对SE3操作一遍
    // 声明一个平移向量t
    Vector3d t(1, 0, 0);
    Sophus::SE3d SE3_Rt(R, t);
    Sophus::SE3d SE3_qt(q, t);
    cout << "SE3 from R,t= \n" << SE3_Rt.matrix() << endl;
    cout << "SE3 from q,t= \n" << SE3_qt.matrix() << endl;  
    Vector6d se3 = SE3_Rt.log();
    cout << "se3 = " << se3.transpose() << endl;
    // 观察输出，会发现在Sophus中，se(3)的平移在前，旋转在后.
    // 同样的，有hat和vee两个算符
    cout << "se3 hat = \n" << Sophus::SE3d::hat(se3) << endl;
    cout << "se3 hat vee = \n" << Sophus::SE3d::vee(Sophus::SE3d::hat(se3)).transpose() << endl;

    Vector6d update_se3;
    update_se3.setZero();
    update_se3(0, 0) = 1e-4d;
    Sophus::SE3d SE3_updated = Sophus::SE3d::exp(update_se3) * SE3_Rt;
    cout << "SE3 updated = " << endl << SE3_updated.matrix() << endl;

    
    
    Matrix <double, 4, 4> test = Sophus::SE3d::hat(se3);
    // test << .0, -1.0, 0.0, 3.5, 1.0, 0.0, 0.0, -3.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;
    cout << test.transpose() << endl;

    return 0;
}