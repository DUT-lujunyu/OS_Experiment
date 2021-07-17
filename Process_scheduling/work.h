#ifndef WORK_H
#define WORK_H
#include <QMainWindow>
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>

//#define N 5

QT_BEGIN_NAMESPACE
namespace Ui { class work; }
QT_END_NAMESPACE

using namespace std;

class work
{
public:
    friend class MainWindow;
    int flag;  //是否为空进程(1:NULL)
    int standard;  //优先级标准
    char name;  //进程名
    int arrival_time;  //进程到达时间
    int service_time;  //进程服务时间
    int re_service_time;  //剩余服务时间
    int finished_time;  //进程完成时间
    int turnover_time;  //进程周转时间
    int wait_time;  //进程等待时间
    int time;  //时间片
    float hrn;
    float authorized_turnover_time;  //加权周转时间
    bool operator <(const work& a) const  //使用优先队列解决SJF
    {
        if (!standard)
            return service_time > a.service_time; //小顶堆
        else
            return hrn < a.hrn;
    }
    void print();  //打印函数
    void add_result();  //结果写回函数
};

//void work::print();


#endif // WORK_H
