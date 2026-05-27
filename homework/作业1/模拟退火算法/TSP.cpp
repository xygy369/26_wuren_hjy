#include <iostream>
#include <fstream>   //作者想要打开txt文件配置原始数据  用来打开文件
#include <string.h>
#include <stdlib.h>    //随机数
#include <algorithm>   //swap算法
#include <vector>
#include <stdio.h>     //c语言式的输出
#include <time.h>    //时间种子
#include <math.h>    //exp函数  指数
#include <string>
 
#define N     30      //城市数量30   相当于是设置的最大
#define T     3000    //初始温度3000
#define EPS   1e-8    //终止温度
#define DELTA 0.98    //温度衰减率
 
 
#define OLOOP 1000    //外循环次数  每个温度下搜索1000次
 
 
using namespace std;
 
//定义路线结构体
struct Path
{
    int citys[N];   //城市数组
    double len;     //距离
};
 
//定义城市点坐标
struct Point
{
    double x, y;   //位置
};
 
//均为全局变量
Path bestPath;        //记录最优路径   path结构体  看数组中的排列关系
double w[N][N];   //两两城市之间路径长度
int nCase;        //测试次数
int sity_num;     //城市数量
vector<Point> P;//记录所有城市的地点   每个数组中的元素都有两个值
 
//正常的计算距离
double dist(Point A, Point B)
{
    return sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
}
 
//计算任意两个城市之间的距离   传入动态数组P   传出数据存入二维数组w
void GetDist()
{
    for(int i = 0; i < sity_num; i++)
        for(int j = i + 1; j < sity_num; j++)
            w[i][j] = w[j][i] = dist(P[i], P[j]);
}   
 
//初始化退火路径   相当于起点   
void Init()
{
    //后来发现作者这一行多余   nCase = 0;      //每次在次函数中初始化nCase,即每次搜索次数
    bestPath.len = 0;      //初始   累加
    for(int i = 0; i < sity_num; i++)
    {
        bestPath.citys[i] = i;
        if(i != sity_num - 1)    //没有循环完成
        {
            printf("%d--->", i);   //打印路径
            bestPath.len += w[i][i + 1];   //累加距离
        }
        else
            printf("%d\n", i);  //最后一个只打印编号
    }
    printf("\nInit path length is : %.3lf\n", bestPath.len);  //打印此时的路径距离
    printf("-----------------------------------\n\n");
}
 
//打印函数   传入路径path
void Print(Path t, int n)
{
    printf("Path is : ");
    for(int i = 0; i < n; i++)
    {
        if(i != n - 1)
            printf("%d-->", t.citys[i]);
        else
            printf("%d\n", t.citys[i]);
    }
    printf("\nThe path length is : %.3lf\n", t.len);
    printf("-----------------------------------\n\n");
}
 
//邻居解   函数
Path GetNext(Path p, int n)
{
    //复制当前路径
    Path ans = p;
    /*随机选择两个位置
    rand() / (RAND_MAX + 1.0)此式大小范围是0到1
    乘上n后大小扩大n倍，取整数   正好随机到0到n-1
    其实就是x=rand()%n;
    */
    int x = (int)(n * (rand() / (RAND_MAX + 1.0)));
    int y = (int)(n * (rand() / (RAND_MAX + 1.0)));
    //保证两个位置不一样
    while(x == y)
    {
        x = (int)(n * (rand() / (RAND_MAX + 1.0)));
        y = (int)(n * (rand() / (RAND_MAX + 1.0)));
    }
    swap(ans.citys[x], ans.citys[y]);  //交换位置
    //计算交换后的总距离
    ans.len = 0;
    for(int i = 0; i < n - 1; i++)
        ans.len += w[ans.citys[i]][ans.citys[i + 1]];
    /*  这一部分不打印了，否则行数太多，本来不知道，后来打印上万次
        cout << "nCase = " << nCase << endl;
    Print(ans, n);   //用print函数打印当前路径距离*/
    nCase++;  //次数加一
    return ans;
}
 
//模拟退火主函数
void SA()
{
    double t = T;    //初始温度
    srand((unsigned)time(NULL));   //设置随机数种子
    Path curPath = bestPath;    //复制当前路径
    Path newPath;
    int iter = 0;   //温度段计数（）
    //这一部分也有改动  作者是只有for循环  改为了while循环加外层的降温路径
    while(t > EPS)    //外层：温度不断降低
    {
        for(int i = 0; i < OLOOP; i++)    //内层：当前温度下搜索1000次
        {
            newPath = GetNext(curPath, sity_num);
            double dE = newPath.len - curPath.len;   //能量差值
            if(dE < 0)   //找到更优解，直接接受
            {
                curPath = newPath;
            }
            else
            {
                double rd = rand() / (RAND_MAX + 1.0);
                //以一定概率接受差解，温度越低概率越小
                if(exp(-dE / t) > rd)
                    curPath = newPath;
            }
            //更新最优解
            if(curPath.len < bestPath.len)
                bestPath = curPath;
        }
        printf("iter = %-5d  T = %-8.2f  best = %.3lf\n", iter, t, bestPath.len);
        t *= DELTA;   //一个温度段结束后降温
        iter++; //（）
    }
}
 
//作者所使用的读文件工作运行   TSP.txt
void readFile(char dataFile[]){
	ifstream input;   //文件输入流
	input.open(dataFile);   //打开传入的文件
 
	if (input.is_open())  //成功写入
    {
		input >> sity_num ;   //读城市数量（第一行）   作者写的sity 应该是city
		
 
		//initialize the vector
		Point temp;   //城市位置坐标传入
		for(int i = 0; i < sity_num; i++){
			P.push_back(temp);   //扩充P的大小
		}
 
		for (int i = 0; i < sity_num; i++){
			try{
				input >> P[i].x;   //>>自动跳过空格和换行
				input >> P[i].y;
				//读取每行所代表的位置坐标
			}
			catch(string &err){
				cout << err <<endl;
			}
		}
	}
	else{
		cout << "open file failed!" <<endl;
	}
}
 
 
int main() {
 
	char dataFile[100] = "TSP.txt";  //固定好文件名
	readFile(dataFile);   //读取文件  初始化path point数据
	GetDist();       //计算距离存入W
	Init();       //初始化路径
	SA();      //开始退火
    Print(bestPath, sity_num);   //最优路径
    printf("Total test times is : %d\n", nCase);
    system("pause");
	
}
  
//其实6个城市没有说服力   我又改成12个了   后来又改成了18个  最后又改成了30个
/*
TSP.txt
6 
80 10 
60 45 
23 80 
55 75 
100 9 
40 90
8*/