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

//-------------------slots-----------------------------------------------
void pt_port::setValue(int value)
{
    if(_value != value)
    {
        _value = value;
        conlog->info(QStringLiteral("Port %1 value: %2").arg(_name).arg(_value));
        emit portValueChangeConfirmed(value);
    }
}

void pt_port::setMode(int mode)
{
    if(_mode != mode)
    {
        _mode = mode;
        conlog->info(QStringLiteral("Port %1 mode: %2").arg(_name).arg(_mode));

        emit portModeChangeConfirmed(mode);
    }
}

/*
void pt_port::switchValue()
{
    _value = !_value;
    conlog->info(QStringLiteral("Port %1 value: %2").arg(_name).arg(_value));
    emit valueChanged(_value);
}
*/

void pt_port::changePortMode(int mode) {
    switch (mode) {
        case 0:
            this->_mcu->setPinMode(this, Controller::OUTPUT);

        break;
        case 1:
            this->_mcu->setPinMode(this, Controller::INPUT);
        break;
    }
}

void pt_port::changePortValue() {
    switch (this->_mode) {
        case 0:
            this->_mcu->setPinValue(this, Controller::OUTPUT, (Controller::Value)(this->_value ? 0:1 ));
        break;
    }
}

//---------------------------------------------------------------------------------

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

QChar pt_port::getPortLetter(){
    return _name[1];
}

int pt_port::getPinNumber(){
    return _name[2].digitValue();
}
