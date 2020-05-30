#ifndef ATMEGA8MCU_H
#define ATMEGA8MCU_H

#include "controller.h"

extern logger* conlog;

class Atmega8MCU : public Controller
{
public:
    Atmega8MCU(QObject *parent, QString name);

    // GPIO methods
    virtual void setPinValue(pt_port* pin, Mode mode, Value value);
    virtual void setPinMode(pt_port* pin, Mode mode);

public slots:
    virtual bool connectToDevice(const QString &serialPortName);
    virtual bool disconnect();

protected:
    bool firstConnectFlag;

    virtual void sendDataToMCU(QByteArray &data);
    virtual void responseHandler(QByteArray &response);
    virtual void errorResponseHandler(QByteArray &response);


protected slots:
    virtual void getDataFromDevice();


};

#endif // ATMEGA8MCU_H
