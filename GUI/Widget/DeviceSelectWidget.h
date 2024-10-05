#ifndef DEVICE_SELECTION_WIDGET_H
#define DEVICE_SELECTION_WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include "Midi/MidiInputReader.h"

class DeviceSelectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceSelectWidget(QWidget* parent = nullptr);

private:
    void initSignal();
    void refreshDevices();
    void onConnBtnPushed();

signals:
    void buttonClicked();  // 按钮点击的信号

private:
    QComboBox* comboBox = Q_NULLPTR;
    QPushButton* connButton = Q_NULLPTR;
    MidiInputReader* midiReader = Q_NULLPTR;
};

#endif // DEVICE_SELECTION_WIDGET_H
