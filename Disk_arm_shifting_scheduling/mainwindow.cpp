#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDir"
#include "QFileDialog"
#include "QTextStream"
#include "QDebug"
#include "qstring.h"

QString path;
vector<int> create_work();


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

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    vector<int> order;
    order = create_work();
    //QString s = ui->textEdit->document()->toPlainText();
    /*
    if(!ui->spinBox->value()) //判断lineedit输入内容是否为空
    {
        QMessageBox::information(NULL, "Error", "You haven't set the number of page.");
        return;
    }
    */
    int pri = ui->spinBox->value();
    int dir;
    if(ui->comboBox_2->currentText()=="序号增加")
        dir = 1;
    else if(ui->comboBox_2->currentText()=="序号减少")
        dir = 0;
    if(ui->comboBox->currentText()=="SSTF")
        SSTF(order, pri);
    else if(ui->comboBox->currentText()=="SCAN")
        SCAN(order, pri, dir);
}


void MainWindow::print_track(vector<int> order, int flag, int p)
{
    if (flag == 1)
    {
        for (int i = 0; i < order.size(); i++)
            ui->textBrowser_2->insertPlainText(QString::number(order[i])+" ");
    }
    else if (flag == 2)
    {
        for (int i = order.size() - 1; i >= 0; i--)
            ui->textBrowser_2->insertPlainText(QString::number(order[i])+" ");
    }
    else if (flag == 3)
    {
        for (int i = p - 1; i >= 0; i--)
            ui->textBrowser_2->insertPlainText(QString::number(order[i])+" ");
        for (int i = p; i < order.size(); i++)
            ui->textBrowser_2->insertPlainText(QString::number(order[i])+" ");
    }
    else
    {
        for (int i = p; i < order.size(); i++)
            ui->textBrowser_2->insertPlainText(QString::number(order[i])+" ");
        for (int i = p - 1; i >= 0; i--)
            ui->textBrowser_2->insertPlainText(QString::number(order[i])+" ");
    }
    ui->textBrowser_2->insertPlainText("\n");
}

void MainWindow::SSTF(vector<int> order, int pri)
{
    ui->textBrowser_2->clear();
    vector<int> flag(order.size(), 1);
    int sum = 0;
    int i;
    for (i = 0; i < order.size(); i++)
    {
        if (order[i] > pri)  //如果磁道号大于当前所在磁道，跳出
            break;
    }
    if (i == 0)  //当前磁道小于队列中所有
    {
        sum = *--order.end() - pri;
        for (int i = 0; i < order.size(); i++)
            ui->textBrowser_2->insertPlainText(QString::number(order[i])+" ");
    }
    else if (i == order.size())  //当前磁道大于队列所有
    {
        sum = pri - *order.begin();
        for (int i = order.size() - 1; i >= 0; i--)
            ui->textBrowser_2->insertPlainText(QString::number(order[i])+" ");
    }
    else
    {
        int left = i - 1;
        int right = i;
        while (left >= 0 && right <= order.size() - 1)
        {
            if (pri - order[left] < order[right] - pri)
            {
                sum += pri - order[left];
                flag[left] = 0;
                pri = order[left];
                left--;
            }
            else
            {
                sum += order[right] - pri;
                flag[right] = 0;
                pri = order[right];
                right++;
            }
            ui->textBrowser_2->insertPlainText(QString::number(pri)+" ");
        }
        if (left == -1)
        {
            while (right <= order.size() - 1)
            {
                sum += order[right] - pri;
                pri = order[right];
                ui->textBrowser_2->insertPlainText(QString::number(pri)+" ");
                right++;
            }
        }
        else
        {
            while (left >= 0)
            {
                sum += pri - order[left];
                pri = order[left];
                ui->textBrowser_2->insertPlainText(QString::number(pri)+" ");
                left--;
            }
        }
    }
    ui->lcdNumber->display(sum);
}

void MainWindow::SCAN(vector<int> order, int pri, int dir)
{
    ui->textBrowser_2->clear();
    int sum = 0;  //寻道总数
    int i;
    int flag;
    for (i = 0; i < order.size(); i++)
    {
        //如果磁道号大于当前所在磁道，跳出
        if (order[i] > pri)
            break;
    }
    if (i == 0)  //当前磁道小于队列中所有
    {
        sum = *--order.end() - pri;
        flag = 1;
    }
    else if (i == order.size())  //当前磁道大于队列所有
    {
        sum = pri - *order.begin();
        flag = 2;
    }
    else if (dir == 0) //向小号磁道移动
    {
        sum += pri - *order.begin(); //第一段（小号磁道）移动总数
        sum += order[i] - *order.begin();  //从最小编号跳到节点
        sum += *--order.end() - order[i];  //第二段（大号磁道）移动总数
        //也可以将后两步进行合并
        //sum += *--order.end() - *order.begin();
        flag = 3;
    }
    else //向大号磁道移动
    {
        sum += *--order.end() - pri; //第一段（大号磁道）移动总数
        sum += *--order.end() - order[i - 1];  //从最大编号跳到节点
        sum += order[i - 1] - *order.begin();  //第二段（大号磁道）移动总数
        //sum += *--order.end() - *order.begin();
        flag = 4;
    }
    ui->lcdNumber->display(sum);
    print_track(order, flag, i);
}

vector<int> create_work()
{
    ifstream infile;
    vector<int> order;
    int reg;
    if(path.isEmpty())
    {
        QMessageBox::information(NULL, "Error", "You haven't chosen file.");
        return order;
    }
    infile.open(path.toStdString());
    while (!infile.eof())
    {
        infile >> reg;
        order.push_back(reg);
    }
    sort(order.begin(), order.end());
    infile.close();
    return order;
}
