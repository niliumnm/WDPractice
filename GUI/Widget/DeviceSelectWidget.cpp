#include "DeviceSelectWidget.h"
#include <QMessageBox>
#include <QDebug>

DeviceSelectWidget::DeviceSelectWidget(QWidget* parent)
    : QWidget(parent)
{
    // 创建 QComboBox 和 QPushButton
    comboBox = new QComboBox(this);
    connButton = new QPushButton("Chick Me", this);

    // 添加选项到 QComboBox
    comboBox->addItem("选项 5");
    comboBox->addItem("选项 6");
    comboBox->addItem("选项 7");

    // 创建布局并添加控件
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(comboBox);
    layout->addWidget(connButton);

    // 设置布局
    setLayout(layout);

    // 连接按钮的点击信号
    connect(connButton, &QPushButton::clicked, this, &DeviceSelectWidget::buttonClicked);

    midiReader = new MidiInputReader;
    // 连接信号 刷新设备
    initSignal();
    refreshDevices();
}

void DeviceSelectWidget::initSignal()
{
    connect(connButton, &QPushButton::clicked, this, &DeviceSelectWidget::onConnBtnPushed);
}

void DeviceSelectWidget::refreshDevices()
{
    auto devices = midiReader->getAvaliableDevice();
    comboBox->clear();
    for (const auto& deviceName : devices) {
        comboBox->addItem(deviceName);
    }
}

void DeviceSelectWidget::onConnBtnPushed()
{
    auto deviceName = comboBox->currentText();
    if (deviceName.isEmpty()) {
        QMessageBox::warning(this, "Failed", "Device name empty");
        return;
    }

    if (!midiReader->openPort(deviceName)) {
        QMessageBox::warning(this, "Failed", "Failed to open Device");
        return;
    }

    qDebug() << "OpenDevice Succeed: " << deviceName;
    return;
}
