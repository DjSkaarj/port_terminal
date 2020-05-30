#ifndef ATMEGA8MCU_H
#define ATMEGA8MCU_H

#include "controller.h"

extern logger* conlog;

class Atmega8MCU : public Controller
{
public:
    Atmega8MCU(QObject *parent, QString name);

    // GPIO methods
    virtual void setPinValue(char portLetter, int pin,  Value value);
    virtual void setPinMode(char portLetter, int pin, Mode mode);

public slots:
    virtual bool connectToDevice(QString serialPortName);
    virtual bool disconnect();

protected:
    bool firstConnectFlag;

    virtual void sendDataToMCU(QByteArray &data);

protected slots:
    virtual void getDataFromDevice();


};

#endif // ATMEGA8MCU_H
