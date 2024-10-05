#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include "WDDB/WDDB.h"
#include "midi/MidiInputReader.h"
#include "GUI/Widget/DeviceSelectWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    WDDB* db = Q_NULLPTR;
    QWidget* centralWidget;
    QMenuBar* menuBar;
    QStatusBar* statusBar;
    DeviceSelectWidget* deviceSelectBox;
};
#endif // MAINWINDOW_H
