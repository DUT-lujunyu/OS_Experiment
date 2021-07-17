#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <QMessageBox>

using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_toolButton_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void print_track(vector<int> order, int flag, int p);

    void SSTF(vector<int> order, int pri);

    void SCAN(vector<int> order, int pri, int dir);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
