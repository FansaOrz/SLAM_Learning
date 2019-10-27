#include <iostream>
#include <ctime>
// Eigen 核心部分
#include <eigen3/Eigen/Core>
// 稠密矩阵的代数运算
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

#define MATRIX_SIZE 50

int main(int argc, char **argv){
    // 声明一个2x3的float矩阵
    Matrix <float, 2, 3> matrix_23;
    
    // Vector3d相当于Matrix<double, 3, 1>
    Vector3d v_3d;
    Matrix <float, 3, 1> vd_3d;
    // 生成一个3x3矩阵，初始化为0
    Matrix3d matrix_33 = Matrix3d::Zero();
    // 不确定大小可以使用动态大小矩阵
    Matrix <double, Dynamic, Dynamic> matrix_dynamic;
    // 更简单的
    MatrixXd matrix_x;

    // Eigen输入数据
    matrix_23 << 1, 2, 3, 4, 5, 6;
    // 输出
    cout << "matrix_23 from 1 to 6:\n" << matrix_23 << endl;

    // 用()访问矩阵中元素
    cout << "print matrix_23:" << endl;
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 3; j++){
            cout << matrix_23(i, j) << "\t";
        }
        cout << endl;
    }

    // 矩阵和向量相乘
    v_3d << 3, 2, 1;
    vd_3d << 4, 5, 6;

    // 两个矩阵相乘必须是同一类型的数据
    Matrix <double, 2, 1> result = matrix_23.cast<double>() * v_3d;
    cout << "[1,2,3;4,5,6] * [3,2,1]=" << result << endl;
    cout << "[1,2,3;4,5,6] * [3,2,1]=" << result.transpose() << endl;

    Matrix <float, 2, 1> result2 = matrix_23 * vd_3d;
    cout << "[1,2,3;4,5,6] * [3,2,1]=" << result2 << endl;

    // 矩阵运算
    matrix_33 = Matrix3d::Random();
    cout << "random matrix: \n" << matrix_33 << endl;
    cout << "transpose: \n" << matrix_33.transpose() << endl;
    cout << "sum: \n" << matrix_33.sum() << endl;
    cout << "trace: \n" << matrix_33.trace() << endl;
    cout << "times 10: \n" << 10 * matrix_33 << endl;
    cout << "inverse: \n" << matrix_33.inverse() << endl;
    cout << "det: \n" << matrix_33.determinant() << endl;

    //特征值
    SelfAdjointEigenSolver <Matrix3d> eigen_solver(matrix_33.transpose() * matrix_33);
    cout << "Eigen values: \n" << eigen_solver.eigenvalues() << endl;
    cout << "Eigen vectors: \n" << eigen_solver.eigenvectors() << endl;


}