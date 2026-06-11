#include<iostream>
#include <cstdlib>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

int main()
{
    system("chcp 65001>nul");  //中文输出
    Vector2d X(0,0);
    Vector2d grad;
    double a=0.01;
    int count =0;

    grad(0)=X(0)-3;
    grad(1)=10*(X(1)-3);

    while(grad.norm()>=0.001)
    {
        X(0)=X(0)-a*grad(0);
        X(1)=X(1)-a*grad(1);
        grad(0)=X(0)-3;
        grad(1)=10*(X(1)-3);
        
        count++;

    }
    cout<<"共需"<<count<<"次"<<endl;
    cout << "最终位置: (" << X(0) << ", " << X(1) << ")" << endl;
    return 0;

}