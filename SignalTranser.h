#ifndef SIGNAL_TRANSFER_H
#define SIGNAL_TRANSFER_H

#include <QObject>
#include <QString>

class SignalTranser : public QObject
{
    Q_OBJECT

public:
    static SignalTranser& instance()
    {
        if (!instance) {
            instance = new SignalTranser();
        }

        return instance;
    }

signals:
    // 定义一些通用信号，比如按钮点击、选择项变化等
    void buttonClicked(const QString& buttonName);

private:
    static SignalTranser* instance = Q_NULLPTR;
    // 私有化构造函数和析构函数
    SignalTranser() {}
    ~SignalTranser() {}

    // 禁止复制
    SignalTranser(const SignalTranser&) = delete;
    SignalTranser& operator=(const SignalTranser&) = delete;
};

#endif // SIGNAL_TRANSFER_H
