#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QPlainTextEdit>
#include <QTextStream>
#include <QFile>
#include <QDateTime>

class logger : public QObject
{
    Q_OBJECT
public:
    logger();
    ~logger() {}

    void appendMessage(const QString &msg);
    void info(const QString &msg);
    void warning(const QString &msg);
    void error(const QString &msg);

    void setLogFile(const QString &path);
    void setMsgbox(QPlainTextEdit* value);

private:
    bool tofile;
    bool toconsole;
    QFile logfile;
    QPlainTextEdit* msgbox;
};

#endif // LOGGER_H
