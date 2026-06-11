#include <iostream>
#include <cstdlib>
#include "osqp.h"
using namespace std;

int main() {
    system("chcp 65001>nul");  //中文输出
    c_int n = 2;   //两个变量  x  y
    c_int m = 1;   //一个约束

    // P = [[1, 0], [0, 10]]
    c_int   P_i[] = {0, 1};    //第0行  第1行
    c_float P_x[] = {1, 10};    //1   10
    c_int   P_p[] = {0, 1, 2};   //第0列  第1列   共2个

    // A = [[1, 1]]
    c_int   A_i[] = {0,0};
    c_float A_x[] = {1,1};
    c_int   A_p[] = {0,1,2};

    //代价函数-3x-30y
    c_float q[] = {-3, -30};
    c_float l[] = {-OSQP_INFTY};   //-无穷大
    c_float u[] = {4};      //上界4

    OSQPSettings *settings = (OSQPSettings *)malloc(sizeof(OSQPSettings)); //内存里申请一块空间给settings
    osqp_set_default_settings(settings);   //填好设置（默认设置）

    // 打包数据 上面定义的所有有关数据 
    OSQPData *data = (OSQPData *)malloc(sizeof(OSQPData));  //打包数据  命名data
    data->n = n;
    data->m = m;
    data->P = csc_matrix(n, n, 2, P_x, P_i, P_p);
    data->A = csc_matrix(m, n, 2, A_x, A_i, A_p);
    data->q = q;
    data->l = l;
    data->u = u;

    OSQPWorkspace *work;  //创建工作空间
    osqp_setup(&work, data, settings);  //传递设置内容

    osqp_solve(work); //求解

    cout << "x = " << work->solution->x[0] << endl;
    cout << "y = " << work->solution->x[1] << endl;

    osqp_cleanup(work);  //释放工作空间
    free(data);   //释放data内存
    free(settings);    //释放settings内存  对应第40行


    return 0;
}