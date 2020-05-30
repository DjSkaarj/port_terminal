#ifndef PORTS_H
#define PORTS_H

#include <QObject>
#include <QString>
#include <vector>
#include "logger.h"
#include "controller.h"

extern logger* conlog;

class pt_port : public QObject
{
    Q_OBJECT

public:
    pt_port(class Controller *mcu) : _service(0), _value(0), _mode(0), _mcu(mcu) {}
    ~pt_port() {}

    QString name() const;
    void setName(const QString &name);

    bool align() const;
    void setAlign(bool align);

    int num() const;
    int value() const;
    void setNum(const quint8 &num);

    bool mode() const;

    bool service() const;
    void setService(bool service);

    QChar getPortLetter();
    int getPinNumber();

    // for Controller to call
    void setMode(int mode);
    void setValue(int value);

public slots:

    // for gui to call
    void changePortMode(int mode); // for ioMode
    void changePortValue();    // for valueButton

    //void switchValue();

signals:

    //for gui to confirm changing
    void portValueChangeConfirmed(int value);
    void portModeChangeConfirmed(int mode);

private:
    bool _service;

    QString _name;
    bool _align;
    int _num;
    int _value;
    int _mode;
    Controller *_mcu;
};

#endif // PORTS_H
