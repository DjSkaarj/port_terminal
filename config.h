#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <vector>
#include "ports.h"
#include "logger.h"

extern logger* conlog;

class config : public QObject
{
    Q_OBJECT
public:
    config() {}
    ~config() {}
    bool load(const QString &path);

    QString name() const;
    void setName(const QString &name);

    std::vector<pt_port*> ports;

private:
    QString _name;
};

#endif // CONFIG_H
