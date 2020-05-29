#include "ports.h"

QString pt_port::name() const
{
    return _name;
}

void pt_port::setName(const QString &name)
{
    _name = name;
}

bool pt_port::align() const
{
    return _align;
}

void pt_port::setAlign(bool align)
{
    _align = align;
}

int pt_port::num() const
{
    return _num;
}

void pt_port::setNum(const quint8 &num)
{
    _num = num;
}

int pt_port::value() const
{
    return _value;
}

void pt_port::setValue(int value)
{
    if(_value != value)
    {
        _value = value;
        conlog->info(QStringLiteral("Port %1 value: %2").arg(_name).arg(_value));
        emit valueChanged(value);
    }
}

void pt_port::switchValue()
{
    _value = !_value;
    conlog->info(QStringLiteral("Port %1 value: %2").arg(_name).arg(_value));
    emit valueChanged(_value);
}

bool pt_port::service() const
{
    return _service;
}

void pt_port::setService(bool service)
{
    _service = service;
}

bool pt_port::mode() const
{
    return _mode;
}

void pt_port::setMode(int mode)
{
    if(_mode != mode)
    {
        _mode = mode;
        conlog->info(QStringLiteral("Port %1 mode: %2").arg(_name).arg(_mode));

        emit modeChanged(mode);
    }
}
