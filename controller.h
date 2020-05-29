#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>


class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);


//signals:


protected:
    QString controllerName;
    QSerialPort *serialPort;

protected:
    //int sendDataToMCU();
    virtual bool sendByteToMCU(QChar data) = 0;

public:
    //GPIO methods
    virtual bool setPinMode();
    virtual bool setPinValue();

    virtual bool getPinMode();
    virtual bool getPinValue();




};

#endif // CONTROLLER_H
