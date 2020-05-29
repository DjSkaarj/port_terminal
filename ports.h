#ifndef PORTS_H
#define PORTS_H

#include <QObject>
#include <QString>
#include <vector>
#include "logger.h"

extern logger* conlog;

class pt_port : public QObject
{
    Q_OBJECT

public:
    pt_port() : _value(0), _mode(0) {}
    ~pt_port() {}

    QString name() const;
    void setName(const QString &name);

    bool align() const;
    void setAlign(bool align);

    int num() const;
    bool value() const;
    void setNum(const quint8 &num);

    bool mode() const;

public slots:
    void setMode(int mode);
    void setValue(bool value);

signals:
    void valueChanged(bool value);

private:
    QString _name;
    bool _align;
    int _num;
    bool _value;
    int _mode;
};

#endif // PORTS_H
