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

bool pt_port::value() const
{
    return _value;
}

void pt_port::setValue(bool value)
{
    if(_value != value)
    {
        _value = value;
        emit valueChanged(value);
    }
}

bool pt_port::mode() const
{
    return _mode;
}

void pt_port::setMode(int mode)
{
    _mode = mode;
    conlog->info(QStringLiteral("Port %1 mode: %2").arg(_name).arg(_mode));
}
