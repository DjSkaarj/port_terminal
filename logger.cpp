#include "logger.h"

logger* conlog;

logger::logger()
{
    tofile = false;
    toconsole = false;
}

void logger::setLogFile(const QString &path)
{
    logfile.setFileName(path);
    if (!logfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        error("Failed to open log file " + path + " for writing.");
        return;
    }

    tofile = true;
}

void logger::setMsgbox(QPlainTextEdit *value)
{
    msgbox = value;
    toconsole = true;
    msgbox->setReadOnly(true);
}

void logger::appendMessage(const QString &msg)
{
    QString fmsg = "[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] " + msg;
    if(toconsole)
        this->msgbox->appendPlainText(fmsg);
    if(tofile)
    {
        QTextStream out(&logfile);
        out << fmsg + "\n";
    }
}

void logger::info(const QString &msg)
{
    appendMessage("[INFO] " + msg);
}

void logger::warning(const QString &msg)
{
    appendMessage("[WARNING] " + msg);
}

void logger::error(const QString &msg)
{
    appendMessage("[ERROR] " + msg);
}
