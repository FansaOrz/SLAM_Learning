#include <iostream>

using namespace std;

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

int main(int argc, char ** argv){
    // 两个旋转的四元数
    Eigen::Quaterniond q1(0.35, 0.2, 0.3, 0.1);
    Eigen::Quaterniond q2(-0.5, 0.4, -0.1, 0.2);
    // 生成两个四元数后要先变成单位四元数
    q1.normalize();
    q2.normalize();
    // 两个平移
    Eigen::Vector3d t1(0.3, 0.1, 0.1);
    Eigen::Vector3d t2(-0.1, 0.5, 0.3);
    // 变换矩阵
    Eigen::Isometry3d T1(q1);
    Eigen::Isometry3d T2(q2);
    //T1.rotate(q1);
    T1.pretranslate(t1);
    //T2.rotate(q2);
    T2.pretranslate(t2);

    Eigen::Vector3d P(0.5, 0, 0.2);
    Eigen::Vector3d P_transformed = Eigen::Vector3d::Identity();
    P_transformed = T2 * T1.inverse() * P;
    cout << P_transformed.transpose() << endl;
}