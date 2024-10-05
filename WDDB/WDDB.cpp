#include "WDDB.h"
#include <QFile>
#include <QDebug>
#include "../Midi/MidiInputReader.h"

WDDB* WDDB::db = Q_NULLPTR;
NoteResolver* NoteResolver::nr = Q_NULLPTR;

WDDB* WDDB::getInstance()
{
    if (!db) {
        db = new WDDB;
    }

    return db;
}

bool WDDB::import(const QString & path)
{
    m_curSheet.clear();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Can't open the file!" << endl;
        return false;
    }

    part linePart;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        removeComment(str);
        if (str.startsWith('#')) {
            parseInfomation(line);
        }
        else if (str.startsWith('!')) {
            resolveNote(str, linePart);
        }
        else if (str.startsWith('@')) {
            Q_ASSERT(!linePart.isEmpty());
            resolveDuration(str, linePart);
            m_curSheet.push_back(linePart);
            linePart.clear();
        }
    }

    Q_ASSERT(linePart.isEmpty());
    return true;
}

void WDDB::format()
{
    QVector<part> result;
    part p;
    for (const auto& originPart : m_curSheet) {
        for (const auto& n : originPart.notes) {
            p.add(n);
            if (n.midiVal == MidiValueBreathCustom) {
                result.push_back(p);
                p.clear();
            }
        }
    }

    if (!p.isEmpty()) {
        p.add({ NoteDuration::NoTime , MidiValueBreathCustom });
        result.push_back(p);
        p.clear();
    }

#ifdef VALIDATE_ALL
    int resultSize = 0, curSheetSize = 0;
    std::for_each(result.begin(), result.end(), [&](const part& p) {
        resultSize += p.m_size;
    });
    std::for_each(m_curSheet.begin(), m_curSheet.end(), [&](const part& p) {
        curSheetSize += p.m_size;
    });
    Q_ASSERT(resultSize == curSheetSize || resultSize == curSheetSize + 1);
#endif // VALIDATE_ALL

    m_curSheet = result;
}

QVector<part> WDDB::getSheet(bool format)
{
    if (format) {
        this->format();
    }

    return m_curSheet;
}

WDDB::WDDB()
{
    test();
}

void WDDB::parseInfomation(const QString& str)
{
    auto headinfo = str;
    headinfo = headinfo.remove(0, 1).trimmed(); // 删掉第一个 去掉前后空格
    if (headinfo.startsWith('J')) { // 是速度
        auto qualStart = headinfo.indexOf("=");
        auto speed = headinfo.right(headinfo.length() - qualStart - 1);
        m_generalSpeed = speed.toInt();
        return;
    }

    m_name = headinfo;
}

void WDDB::removeComment(QString& str)
{
    auto commentStart = str.indexOf("//"); // 0 //
    str = str.left(commentStart).trimmed();
}

void WDDB::resolveNote(const QString& line, part& part)
{
    auto nr = NoteResolver::getInstance();
    Q_ASSERT(part.isEmpty());
    QString li = line;
    auto noteInfo = li.remove(0, 1).trimmed();
    QStringList notes = noteInfo.split(QRegExp("[| ]+"), QString::SkipEmptyParts);
    for (const auto& noteStr : notes) {
        part.add(nr->resolveNote(noteStr));
    }

    notes.removeAll(""); // 如果需要，可以移除空字符串
}

void WDDB::resolveDuration(const QString& line, part& part)
{
    
    Q_ASSERT(!part.isEmpty());
    QString li = line;
    auto noteInfo = li.remove(0, 1).trimmed();
    QStringList durations = noteInfo.split(QRegExp("[| ]+"), QString::SkipEmptyParts);
    for (int index = 0; index < durations.size(); index++) {
        if (part.at(index).midiVal == MidiValueBreathCustom) {
            part.at(index).m_duration = str2NoteDurationMap["0"];
        }
        else {
            part.at(index).m_duration = str2NoteDurationMap[durations[index]];
        }
    }
}

void WDDB::test()
{
    //parseInfomation("# 碎月");
    //parseInfomation("# J=92");
    //parseInfomation("# J=942");
    //parseInfomation("# J=2");
    //part partNotes;
    //removeComment(QString("! #D #E #G #A #E #D | #E A #C #D | B #C B G | A S // S代表换行"));
    ////resolveNote(QString("! #D #E #G #A #E #D | #E A #C #D | B #C B G | A S"), partNotes);
    //auto nr = NoteResolver::getInstance();
    //nr->resolveNote("D4"); // 62 通过
    //nr->resolveNote("C#4"); // 61 通过
    //nr->resolveNote("Db4"); // 61 通过
    //nr->resolveNote("E4"); // 64
    //Q_ASSERT(nr->resolveNote("C8").midiVal == 108);
    //Q_ASSERT(nr->resolveNote("A#7").midiVal == 106);
    //Q_ASSERT(nr->resolveNote("Bb7").midiVal == 106);
    //Q_ASSERT(nr->resolveNote("F#7").midiVal == 102);
    //Q_ASSERT(nr->resolveNote("C#7").midiVal == 97);
    //Q_ASSERT(nr->resolveNote("G#6").midiVal == 92);
    //Q_ASSERT(nr->resolveNote("A#5").midiVal == 82);
    //Q_ASSERT(nr->resolveNote("G5").midiVal == 79);
    //Q_ASSERT(nr->resolveNote("F5").midiVal == 77);
    //Q_ASSERT(nr->resolveNote("C#5").midiVal == 73);
    //Q_ASSERT(nr->resolveNote("A#4").midiVal == 70);
    //Q_ASSERT(nr->resolveNote("F#4").midiVal == 66);
    //Q_ASSERT(nr->resolveNote("D#4").midiVal == 63);
    //Q_ASSERT(nr->resolveNote("C#2").midiVal == 37);
    //Q_ASSERT(nr->resolveNote("C2").midiVal == 36);
    //Q_ASSERT(nr->resolveNote("A#1").midiVal == 34);
    //Q_ASSERT(nr->resolveNote("G#1").midiVal == 32);
    if (import("D:\\Code\\WDPractice\\碎月.txt")) {
        format();
    }
}

Note& part::at(int idx)
{
    Q_ASSERT(idx >= 0 && idx < m_size);
    return notes[idx];
}

NoteResolver* NoteResolver::getInstance()
{
    if (!nr) {
        nr = new NoteResolver;
    }

    return nr;
}

Note NoteResolver::resolveNote(const QString& noteStr)
{
    auto getCharDifference = [&](QChar a, QChar b) -> int {
        QMap<QChar, int> value{
            { 'C', 1 }, { 'D', 2 }, { 'E', 3 }, { 'F', 4 }, { 'G', 5 }, { 'A', 6 }, { 'B', 7 }
        };
        Q_ASSERT(value.contains(a) && value.contains(b));
        return abs(value[b]-value[a]);
    };

    // https://inspiredacoustics.com/en/MIDI_no-te_numbers_and_center_frequencies
    Note ret;
    QString str = QString(noteStr.toUpper()).trimmed();
    auto firstKey = str.at(0);
    if (firstKey == 'S') {
        ret.midiVal = MidiValueBreathCustom;
        return ret;
    }

    str = str.remove(0, 1);
    int i = getCharDifference(firstKey, QChar('C')); // C0 D1 E2
    //int shift[] = { 0, 2, 2, 1, 2, 2, 2 }; // 音符C到B的差值
    int shift[] = { 0, 2, 4, 5, 7, 9, 11 }; // 音符C到B的差值
    // C开头的为12 D开头的为14 例C4 = '12'+4*12  D4 = '14'+4*12
    int midiValue = 12 + shift[i];
    auto last = str.at(str.length() - 1); // 获取最后一个数字
    Q_ASSERT(last.unicode() >= '1' && last.unicode() <= '9');
    str.remove(str.length() - 1, 1);
    midiValue += ((last.unicode() - '0') * 12);
    // D#4/Eb4 中间的那个
    if (!str.isEmpty()) {
        if (str == "B") {
            midiValue -= 1;
        }
        else if (str == "#") {
            midiValue += 1;
        }
        else {
            Q_ASSERT(false);
        }
    }

    ret.midiVal = midiValue;
    return ret;
}

NoteResolver::NoteResolver()
{
}
