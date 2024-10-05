#include "MidiInputReader.h"
#include <iostream>
#include <qdebug.h>
#include <QFile>

MidiInputReader::MidiInputReader()
{


    std::function<void(libremidi::message&& message)> myCallBack = std::bind(&MidiInputReader::midiCallBackFunc, this, std::placeholders::_1);
    midiInObj = new libremidi::midi_in{
        libremidi::input_configuration{.on_message = myCallBack }
    };

    midiOutObj = new libremidi::midi_out{
    libremidi::output_configuration{}, libremidi::midi_out_configuration_for(obs)
    };
}

QStringList MidiInputReader::getAvaliableDevice()
{
    QStringList ret;
    for (const libremidi::input_port& port : obs.get_input_ports()) {
        QString portName = QString::fromStdString(port.port_name);
        ret.push_back(portName);
        ports[portName] = port;
    }

    return ret;
}

bool MidiInputReader::openPort(const QString& name)
{
    if (name.isEmpty() || !ports.contains(name)) {
        return false;
    }

    auto result = midiInObj->open_port(ports[name]);
    auto test = stdx::error{};
    if (result == stdx::error{}) {
        auto res = midiOutObj->open_virtual_port("My Virtual MIDI Output");
        if (res != stdx::error{}) {
            qDebug() << "";
        }

        std::cout << "MIDI port opened successfully." << std::endl;
        return true;
    }
    else {
        return false;
        std::cerr << "Failed to open MIDI port: " << name.toStdString() << std::endl;
    }

}

void MidiInputReader::midiCallBackFunc(libremidi::message&& message)
{
    midiOutObj->send_message(message);
    QFile file("output.txt");

    // 以 WriteOnly 模式打开文件，如果文件无法打开，显示错误信息
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return;  // 退出程序
    }
    // 使用 QTextStream 将文本写入文件
    QTextStream out(&file);

    // how many bytes
    message.size();
    // access to the individual bytes
    //message[i];
    // access to the timestamp
    message.timestamp;
    // 获取消息的类型
    switch (message.get_message_type()) {
        case libremidi::message_type::NOTE_ON: {
            int channel = message.get_channel();      // 获取通道
            int note = message.bytes[1];              // 获取音符编号
            int velocity = message.bytes[2];          // 获取力度

            if (velocity > 0) {
                out << "Note ON: "
                    << "Channel: " << channel << ", "
                    << "Note: " << note << ", "
                    << "Velocity: " << velocity << "\n";
            }
            else {
                // 如果力度为0，有些设备会发送 NOTE_ON + velocity 0 来表示 NOTE_OFF
                out << "Note OFF (via NOTE_ON with velocity 0): "
                    << "Channel: " << channel << ", "
                    << "Note: " << note << "\n";
            }
            break;
        }

        case libremidi::message_type::NOTE_OFF: {
            int channel = message.get_channel();      // 获取通道
            int note = message.bytes[1];              // 获取音符编号
            int velocity = message.bytes[2];          // 获取力度

            out << "Note OFF: "
                << "Channel: " << channel << ", "
                << "Note: " << note << ", "
                << "Velocity: " << velocity << "\n";
            break;
        }

                                              // 处理其他类型的 MIDI 消息，例如控制改变、弯音等
        case libremidi::message_type::CONTROL_CHANGE: {
            int channel = message.get_channel();
            int control = message.bytes[1];
            int value = message.bytes[2];

            out << "Control Change: "
                << "Channel: " << channel << ", "
                << "Control: " << control << ", "
                << "Value: " << value << "\n";
            break;
        }

        default:
            out << "Unhandled MIDI message type." << "\n";
            break;
    }
}
