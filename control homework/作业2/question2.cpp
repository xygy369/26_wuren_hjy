#include<iostream>
#include <cstdlib>

using namespace std;

int main()
{
    system("chcp 65001>nul");  //中文输出

    /*KKT   原来（3，3）明显不符合   只能选择贴墙走   最优解对x求导为0  对y求导为0  最后约束式等于0
    𝐿=1/2(𝑥−3)^2+5(𝑦−3)^2+𝜇(𝑥+𝑦−4)    拉格朗日函数
    x-3+mu  x导数   10（y-3）+mu   y导数          3-mu+3-mu/10=4  求解mu   即为以下解
    */
    double mu = 20.0 / 11.0;    
    double x = 3.0 - mu;    
    double y = 3.0 - mu / 10.0;
    
    // 验证
    cout << "最优解:" << endl;
    cout << "x = " << x << endl;
    cout << "y = " << y << endl;
    cout << "mu = " << mu << endl;
    cout << "x + y = " << x + y << endl;
    cout << "mu >= 0: " << (mu >= 0 ? "是" : "否") << endl;


    return 0;
}