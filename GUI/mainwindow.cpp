#include "mainwindow.h"
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置主窗口的几何形状
    this->setGeometry(0, 0, 1600, 1200);
    this->setWindowTitle("MainWindow");

    // 创建中心小部件并设置
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget); // 设置中心小部件

    // 创建菜单栏
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar); // 设置菜单栏

    // 创建状态栏
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar); // 设置状态栏

    // 创建设备选择栏
    deviceSelectBox = new DeviceSelectWidget(centralWidget);

    db = WDDB::getInstance();
}

MainWindow::~MainWindow()
{
    
}
