#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置界面背景
    QPalette palette;  //创建一个调色板的对象
    // QPixmap pixmap(":/new/prefix1/ico/1.png");
    QPixmap pixmap("D:/Qt/Designer/main_interface_.jpg"); //创建一个qrc文件，添加图片到qrc文件中，把一个图片映射到pixmap这个对象上
    MainWindow w;
    palette.setBrush(w.backgroundRole(),QBrush(pixmap)); //用调色板的画笔，把映射到pixmap上的图片，画到w.bakegroundRole()这个背景上。（这里并没有把颜色加到窗口背景，还需一步如下）
    w.setPalette(palette); //设置窗口调色板为palette，窗口与画笔相关联
    w.show();

    //设置字体大小
    QFont font("msyh",13);
    a.setFont(font);
    return a.exec();
}
