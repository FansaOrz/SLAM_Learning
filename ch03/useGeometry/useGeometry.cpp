/*
演示如何使用Eigen集合模块
*/

#include <iostream>
#include <cmath>

using namespace std;

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

using namespace Eigen;

int main(int argc, char **argv){
    // Geometry提供了各种旋转平移的表示
    // 3D旋转矩阵直接使用Matrix3d或Matrix3f
    // 利用单位矩阵初始化
    Matrix3d rotation_matrix = Matrix3d::Identity();
    // 旋转向量使用AngleAxis，可以直接当做矩阵运算
    // 沿Z轴旋转45度
    AngleAxisd rotation_vector(M_PI / 4, Vector3d(0, 0, 1));
    // 输出三位小数的精度
    cout.precision(3);
    // 把向量转化为矩阵
    cout << "rotation matrix = \n" << rotation_vector.matrix() << endl;
    // 向量转化为旋转矩阵
    rotation_matrix = rotation_vector.toRotationMatrix();
    // 用AngleAxis进行坐标变化
    Vector3d v(1, 0, 0);
    Vector3d v_rotated = rotation_vector * v;
    // 使用向量做旋转变化
    cout << "(1,0,0) after rotation (by angle axis) = " << v_rotated.transpose() << endl;
    // 
    v_rotated = rotation_matrix * v;
    cout << "(1,0,0) after rotation (by matrix) = " << v_rotated.transpose() << endl;

    // 将旋转矩阵变成欧拉角
    // ZYX的顺序，即roll pitch yaw顺序
    Vector3d euler_angles = rotation_matrix.eulerAngles(2, 1, 0);
    cout << "yaw pitch roll = " << euler_angles.transpose() << endl;

    // 欧氏变换矩阵使用 Eigen::Isometry
    // 虽然是4*4的矩阵，但是写成3d
    Isometry3d T = Isometry3d::Identity();
    // 按照旋转矩阵旋转
    T.rotate(rotation_vector);
    // 平移向量定为 1,3,4
    T.pretranslate(Vector3d(1, 3, 4));
    cout << "Transform matrix = \n" << T.matrix() << endl;

    // 用变换矩阵进行坐标变换
    Vector3d v_transformed = T * v;
    cout << "v tranformed = " << v_transformed.transpose() << endl;

    // 四元数
    // 可以直接把AngleAxis赋值给四元数
    Quaterniond q = Quaterniond(rotation_vector);
    // 请注意coeffs的顺序是(x,y,z,w),w为实部，前三者为虚部
    // q.coeffs() 多项式系数
    cout << "quaternion from rotation vector = " << q.coeffs().transpose() << endl;
    // 也可以直接把旋转矩阵赋值给四元数
    q = Quaterniond(rotation_matrix);
    cout << "quaternion from rotation matrix = " << q.coeffs().transpose() << endl;
    // 使用四元数旋转一个向量，直接使用重载的乘法就可以
    v_rotated = q * v;
    cout << "(1,0,0) after rotation = " << v_rotated.transpose() << endl;
    // 用常规向量乘法表示，则应该如下计算
    cout << "should be equal to " << (q * Quaterniond(0, 1, 0, 0) * q.inverse()).coeffs().transpose() << endl;

    return 0;
}