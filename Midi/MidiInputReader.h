#ifndef MIDI_INPUT_READER
#define MIDI_INPUT_READER

#include <QMap>
#include "libremidi/libremidi.hpp"

class MidiInputReader {
public:
    MidiInputReader();

public:
    QStringList getAvaliableDevice();
    bool openPort(const QString& name);

private:
    void midiCallBackFunc(libremidi::message&& message);

private:
    QMap<QString, libremidi::input_port> ports;
    libremidi::observer obs;
    libremidi::midi_in* midiInObj = Q_NULLPTR;
    libremidi::midi_out* midiOutObj = Q_NULLPTR;
};

#endif MIDI_INPUT_READER
