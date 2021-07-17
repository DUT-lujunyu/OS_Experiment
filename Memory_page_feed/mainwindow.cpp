#include "mainwindow.h"
//#include "work.h"
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

    if(!ui->spinBox->value()) //判断lineedit输入内容是否为空
    {
        QMessageBox::information(NULL, "Error", "You haven't set the number of page.");
        return;
    }

    int sum = ui->spinBox->value();
    if(ui->comboBox->currentText()=="FIFO")
        FIFO(order, sum);
    else if(ui->comboBox->currentText()=="LRU")
        LRU(order, sum);
}

void MainWindow::print(vector<int> obj)
{
    string res;
    for (int i = 0; i < obj.size(); i++)
    {
        res += to_string(obj[i]);
        res += " ";
    }
    ui->textBrowser_3->append(QString::fromStdString(res));
}

void MainWindow::FIFO(vector<int> order, int num)
{
    cout << "FIFO";
    ui->textBrowser_2->clear();
    ui->textBrowser_3->clear();
    ui->lcdNumber->clearMask();
    ui->lcdNumber_2->clearMask();
    vector<int> page(num, 0);
    int count = 0;  //未命中数量
    for (int i = 0; i < order.size(); i++)
    {
        auto find_p = std::find(page.begin(), page.end(), order[i]);
        int erase_page = -1;
        if (find_p == page.end())
        {
            count++;
            erase_page = *page.begin();
            page.erase(page.begin());
            page.push_back(order[i]);
            ui->textBrowser_2->append("miss and ");
        }
        else ui->textBrowser_2->insertPlainText("\n");
        print(page);

        if(erase_page != -1)
        {
            ui->textBrowser_2->insertPlainText("erase_page:");
            ui->textBrowser_2->insertPlainText(QString::number(erase_page));
        }
    }
    ui->lcdNumber->display(count);
    ui->lcdNumber_2->display(count*1.0/order.size());
}


void MainWindow::LRU(vector<int> order, int num)
{
    cout << "LRU" << endl;
    ui->textBrowser_2->clear();
    ui->textBrowser_3->clear();
    ui->lcdNumber->clearMask();
    ui->lcdNumber_2->clearMask();
    vector<int> page(num, 0);
    int count = 0;  //未命中数量
    for (int i = 0; i < order.size(); i++)
    {
        int erase_page = -1;
        auto find_p = std::find(page.begin(), page.end(), order[i]);
        if (find_p == page.end())
        {
            count++;
            erase_page = *page.begin();
            page.erase(page.begin());
            page.push_back(order[i]);
            ui->textBrowser_2->append("miss and");
        }
        else
        {
            page.erase(find_p);
            page.push_back(order[i]);
            ui->textBrowser_2->insertPlainText("\n");
        }
        print(page);
        if(erase_page != -1)
        {
            ui->textBrowser_2->insertPlainText("erase_page:");
            ui->textBrowser_2->insertPlainText(QString::number(erase_page));
        }
    }
    ui->lcdNumber->display(count);
    ui->lcdNumber_2->display(count*1.0/order.size());;
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
    infile.close();
    //FIFO(order, 5);
    return order;
}
