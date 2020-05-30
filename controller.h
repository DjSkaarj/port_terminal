#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QQueue>
#include "ports.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

    virtual bool connectToDevice() = 0;
    virtual bool disconnect() = 0;

    //possible modes and values of pins
    enum Value {LOW = 0, HIGH = 1};
    enum Mode {OUTPUT = 0, INPUT = 1};

    //GPIO methods
    virtual void setPinValue(char portLetter, int pin,  Value value) = 0;
    virtual void setPinMode(char portLetter, int pin, Mode mode) = 0;

protected:
    QString controllerName;
    QSerialPort *serialPort;
    struct Task {
        //pt_port *pin;
        Mode modeToSet;
        Value valueToSet;
        bool modeOrValue;  // 0-value, 1-mode
    };

    QQueue<Task*> taskQueue;

    virtual bool sendDataToMCU(QChar data) = 0;


protected slots:
    virtual void handleDataFromDevice() = 0;
};

#endif // CONTROLLER_H
