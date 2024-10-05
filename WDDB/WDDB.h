#ifndef WDDB_H
#define WDDB_H
#include <QVector>
#include <QMap>

#define VALIDATE_ALL

constexpr int MidiValueBreathCustom = -2;
enum class NoteDuration
{
    Whole, // 全音符
    Half, // 半音符
    Quarter, // 四分音符
    Eighth, // 八分音符
    Sixteenth, // 十六分音符
    ThirtySecond,// 三十二分音符
    NoTime, // 无效
};

struct Note
{
    NoteDuration m_duration = NoteDuration::NoTime;
    int midiVal = -1;
};

struct part {
    int m_partSpeed = -1; // 没有partSpeed就使用generalSpeed
    QVector<Note> notes;
    int m_size = 0;

    void add(const Note& n) {
        notes.push_back(n);
        m_size++;
    }

    bool isEmpty() {
        return m_size == 0;
    }

    Note& at(int idx);

    void clear() {
        m_partSpeed = -1;
        notes.clear();
        m_size = 0;
    };
};

class NoteResolver {
public:
    static NoteResolver* getInstance();
    NoteResolver(const NoteResolver&) = delete;
    NoteResolver& operator=(const NoteResolver&) = delete;
    Note resolveNote(const QString& noteStr);

protected:
    static NoteResolver* nr;
    NoteResolver();
};

class WDDB {
    QMap<QString, NoteDuration> str2NoteDurationMap = {
        { "0", NoteDuration::NoTime },
        { "1", NoteDuration::Whole },
        { "2", NoteDuration::Half },
        { "4", NoteDuration::Quarter },
        { "8", NoteDuration::Eighth },
        { "16", NoteDuration::Sixteenth },
        { "32", NoteDuration::ThirtySecond }
    };
public:
    static WDDB* getInstance();
    WDDB(const WDDB&) = delete;
    WDDB& operator=(const WDDB&) = delete;

public:
    bool import(const QString& path);
    void format();
    QVector<part> getSheet(bool format = false);

protected:
    static WDDB* db;
    QString m_curTable;
    WDDB();

private:
    void parseInfomation(const QString& str);
    void removeComment(QString& str);
    void resolveNote(const QString& line, part& part);
    void resolveDuration(const QString& line, part& part);
    void test();

private:
    int m_generalSpeed = 60;
    QString m_name;
    QVector<part> m_curSheet;
};

#endif // !WDDB_H
