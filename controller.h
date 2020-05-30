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
    explicit Controller(QObject *parent = nullptr, QString name = "Atmega");

    virtual bool connectToDevice() = 0;
    virtual bool disconnect() = 0;

    //possible modes and values of pins
    enum Value {LOW = 0, HIGH = 1};
    enum Mode {OUTPUT = 0, INPUT = 1};
    enum Type {SET_VALUE = 0, SET_MODE = 1};

    //GPIO methods
    virtual void setPinValue(class pt_port* pin, Mode mode, Value value) = 0; //Mode mode, Value value, char portLetter, int pin
    virtual void setPinMode(class pt_port* pin, Mode mode) = 0;

protected:
    QString controllerName;
    QSerialPort *serialPort;
    struct Task {
        class pt_port *port;
        Mode modeToSet;
        Value valueToSet;
        Type typeOfTask;
    };

    QQueue<Task*> taskQueue;

    virtual void pushTask(Type type, pt_port* &pin, Mode mode, Value value);
    virtual Task* pullTask();
    virtual void redoTask(Task *task);
    virtual void terminateTask(Task *task);

    virtual void sendDataToMCU(QByteArray &data) = 0;
    virtual void responseHandler(QByteArray &data) = 0;

signals:
    void controllerConnected();
    void controllerDisconnected();

protected slots:
    virtual void getDataFromDevice() = 0;
};

#endif // CONTROLLER_H
