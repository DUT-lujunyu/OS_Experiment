#include "mainwindow.h"
#include "work.h"
#include "ui_mainwindow.h"
#include "QDir"
#include "QFileDialog"
#include "QTextStream"
#include "QDebug"
#include "qstring.h"

//全局变量声明
QString path;
vector<work> result;
//vector<work> work_book;

//全局函数声明
vector<work> FCFS(vector<work> work_book);
vector<work> RR(vector<work> work_book, int time);
vector<work> SJF(vector<work> work_book);
vector<work> HRN(vector<work> work_book);
vector<work> create_work(vector<work> work_book);
void print(vector<work> work_book);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//浏览目标文件列表方法1
/*
void MainWindow::on_toolButton_clicked()
{
    ui->cbo_path->clear();
    QString path = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Open Directory"),QDir::currentPath()));
    ui->label->setText(path);
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
    QFileInfoList list = dir.entryInfoList();

    for(int i = 0; i < list.length(); i++)
    {
        QString file = list.at(i).fileName();
        ui->cbo_path->addItem(file);
    }
}
*/
void MainWindow::on_toolButton_pressed()
{

}
//浏览目标文件列表方法2
void MainWindow::on_toolButton_clicked()
{
    ui->cbo_path->clear();
    path= QDir::toNativeSeparators(QFileDialog::getOpenFileName(this,tr("Save path"),QDir::currentPath()));  //文件路径
    if(!path.isEmpty())
    {
        if(ui->cbo_path->findText(path)==-1)
            ui->cbo_path->addItem(path);    //在comboBox中显示文件路径

        QFile *file=new QFile;   //申请一个文件指针
        file->setFileName(path);   //设置文件路径
        bool ok=file->open(QIODevice::ReadOnly);
        if(ok)
        {
            QTextStream in(file);
            ui->textBrowser->setText(in.readAll());    //在textBrowser中显示文件内容
            file->close();
        }
    }
}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    vector<work> work_book;
    work_book = create_work(work_book);
    while(!result.empty())
        result.pop_back();
    cout<<"Hello";
    if(ui->comboBox->currentText()=="FCFS")
        result = FCFS(work_book);
    else if(ui->comboBox->currentText()=="SJF")
        result = SJF(work_book);
    else if(ui->comboBox->currentText()=="HRN")
        result = HRN(work_book);
    else if(ui->comboBox->currentText()=="RR(q = 1)")
        result = RR(work_book, 1);
    else if(ui->comboBox->currentText()=="RR(q = 4)")
        result = RR(work_book, 4);
    ui->textBrowser_2->clear();
    ui->lcdNumber->display(0);
    ui->lcdNumber_2->display(0);
    cout<<"Hello";
    for(int i = 0; i < result.size(); i++)
    {
        QString res_;
        string res;
        res.push_back(result[i].name);
        res.push_back('\t');
        res += to_string(result[i].arrival_time) + '\t';
        res += to_string(result[i].service_time) + '\t';
        res += to_string(result[i].finished_time) + '\t';
        res += to_string(result[i].turnover_time) + '\t';
        res += to_string(result[i].authorized_turnover_time) + '\t'+'\n';
        cout<< res;
        res_ = QString::fromStdString(res);
        ui->textBrowser_2->append(res_);
        //}
    }
}

void work::print()
{
    std::cout << name << '\t' << '\t';
    std::cout << arrival_time << '\t' << '\t';
    std::cout << service_time << '\t' << '\t';
    std::cout << finished_time << '\t' << '\t';
    std::cout << turnover_time << '\t' << '\t';
    std::cout << authorized_turnover_time << '\t' << '\t' << endl;
}
/*
void work::add_result()
{
    ofstream write;
    write.open("result.txt", ios::app);                //用ios::app不会覆盖文件内容

    write << name <<  '\t';
    write << arrival_time << '\t';
    write << service_time << '\t';
    write << finished_time << '\t';
    write << turnover_time << '\t';
    write << authorized_turnover_time << '\t' << endl;
    write.close();
}
*/
vector<work> FCFS(vector<work> work_book)
{
    queue<work> q;
    vector<work> result;
    q.push(work_book[0]);
    int i = 0;  //work完成数
    int j = 1;	//最先等待进入队列的索引
    int t = 0;	//时钟数
    while (i < work_book.size())
    {
        t++;  //完成一个时钟的任务(即t~t+1这一时段的所有任务)
        //查看是否有新的进程入列
        if (j < work_book.size() && work_book[j].arrival_time == t)
        {
            q.push(work_book[j]);
            j++;
        }
        //处理正在执行的进程
        if (!q.empty())
        {
            q.front().re_service_time--;
            if (q.front().re_service_time == 0)
            {
                //维护进程的相关信息
                q.front().finished_time = t;
                q.front().turnover_time = t - q.front().arrival_time;
                q.front().authorized_turnover_time =
                        float(q.front().turnover_time) / q.front().service_time;
                result.push_back(q.front());
                //q.front().add_result();
                q.pop();
                i++;
            }
        }
    }
    return result;
}

vector<work> SJF(vector<work> work_book)
{
    priority_queue<work> q;
    vector<work> result;
    work execute = work_book[0];  //当前正在执行的进程
    int i = 0;  //work完成数
    int j = 1;	//最先等待进入队列的索引
    int t = 0;	//时钟数
    while (i < work_book.size())
    {
        t++;  //完成一个时钟的任务(即t~t+1这一时段的所有任务)
        //查看是否有新的进程入列
        if (j < work_book.size() && work_book[j].arrival_time == t)
        {
            q.push(work_book[j]);
            j++;
        }
        //处理正在执行的进程
        if (!execute.flag)
        {
            execute.re_service_time--;
            if (execute.re_service_time == 0)
            {
                execute.finished_time = t;
                execute.turnover_time = t - execute.arrival_time;
                execute.authorized_turnover_time =
                        float(execute.turnover_time) / execute.service_time;
                result.push_back(execute);
                execute.flag = 0;
                i++;
                if (!q.empty())
                {
                    execute = q.top();
                    q.pop();
                }
            }
        }
    }
    return result;
}

vector<work> HRN(vector<work> work_book)
{
    priority_queue<work> q;
    queue<work> q_;
    vector<work> result;
    for (int i = 0; i < work_book.size(); i++)
        work_book[i].standard = 1;
    work execute = work_book[0];  //当前正在执行的进程
    int i = 0;  //work完成数
    int j = 1;	//最先等待进入队列的索引
    int t = 0;	//时钟数
    while (i < work_book.size())
    {
        t++;  //完成一个时钟的任务(即t~t+1这一时段的所有任务)
        //更新已在就绪队列等待的进程响应比
        while(!q.empty())
        {
            work temp = q.top();
            q.pop();
            temp.wait_time++;
            temp.hrn += 1.0 / temp.service_time;
            q_.push(temp);
        }
        while (!q_.empty())
        {
            q.push(q_.front());
            q_.pop();
        }
        //查看是否有新的进程入列
        if (j < work_book.size() && work_book[j].arrival_time == t)
        {
            q.push(work_book[j]);
            j++;
        }
        //处理正在执行的进程
        if (!execute.flag)
        {
            execute.re_service_time--;
            if (execute.re_service_time == 0)
            {
                execute.finished_time = t;
                execute.turnover_time = t - execute.arrival_time;
                execute.authorized_turnover_time =
                        float(execute.turnover_time) / execute.service_time;
                //execute.print();
                result.push_back(execute);
                execute.flag = 0;
                i++;
                if (!q.empty())
                {
                    execute = q.top();
                    q.pop();
                }
            }
        }
    }
    return result;
}

vector<work> RR(vector<work> work_book, int time)
{
    queue<work> q;
    vector<work> result;
    int i = 0;  //work完成数
    int j = 1;	//最先等待进入队列的索引
    int t = 0;	//时钟数
    q.push(work_book[0]);
    for (int i = 0; i < work_book.size(); i++)
        work_book[i].time = time;
    while (i < work_book.size())
    {
        t++;
        if (j < work_book.size() && work_book[j].arrival_time == t)
        {
            q.push(work_book[j]);
            j++;
        }
        if (!q.empty())
        {
            q.front().re_service_time--;
            q.front().time--;
            if (q.front().re_service_time == 0)
            {
                q.front().finished_time = t;
                q.front().turnover_time = t - q.front().arrival_time;
                q.front().authorized_turnover_time =
                        float(q.front().turnover_time) / q.front().service_time;
                result.push_back(q.front());
                q.pop();
                i++;
            }
            else if(q.front().time == 0) //未完成任务放于队列尾
            {
                q.front().time = time;
                work temp = q.front();
                q.pop();
                q.push(temp);
            }
        }
    }
    return result;
}

vector<work> create_work(vector<work> work_book)
{
    ifstream infile;
    if(path.isEmpty())
    {
        QMessageBox::information(NULL, "Error", "You haven't chosen file.");
        return work_book;
    }
    infile.open(path.toStdString());
    while (!infile.eof())
    {
        work obj;
        infile >> obj.name>>obj.arrival_time>>obj.service_time;
        obj.re_service_time = obj.service_time;
        obj.finished_time = obj.turnover_time
            = obj.authorized_turnover_time = obj.wait_time = 0;
        obj.flag = obj.standard = 0;  //默认非空进程，短作业优先
        obj.hrn = 1;
        obj.time = 4;
        //i++;
        work_book.push_back(obj);
    }
    infile.close();
    cout<< work_book.size();
    cout<<"OK"<<endl;
    return work_book;
}

void print(vector<work> work_book)
{
    for (int i = 0; i < work_book.size(); i++)
        work_book[i].print();
}

void MainWindow::on_radioButton_clicked()
{
    float avg = 0;
    for(int i = 0; i < result.size(); i++)
        avg += result[i].turnover_time;
    //QString avg_ = QString::fromStdString(to_string(avg*1.0 / result.size()));
    //ui->textBrowser_3->append(avg_);
    ui->lcdNumber->display(avg*1.0 / result.size());
}

void MainWindow::on_radioButton_2_clicked()
{
    float sum = 0;
    for(int i = 0; i < result.size(); i++)
        sum += result[i].authorized_turnover_time;
    //float avg = sum *1.0 / result.size();
    //QString avg_ = QString::fromStdString(to_string(av));
    //ui->textBrowser_2->append(avg_);
    ui->lcdNumber_2->display(sum*1.0 / result.size());
}
