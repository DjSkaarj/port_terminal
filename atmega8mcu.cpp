#include "atmega8mcu.h"

#include <QSerialPort>

Controller::Controller(QObject *parent, QString name)
    :controllerName(name), serialPort(new QSerialPort(this)) {}

Atmega8MCU::Atmega8MCU(QObject *parent, QString name)
    : Controller(parent, name), firstConnectFlag(true) {}

bool Atmega8MCU::connectToDevice(QString serialPortName) {
    serialPort->setPortName(serialPortName);
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    if (!serialPort->open(QIODevice::ReadWrite)) {
        conlog->error("Can't connect to device");
        return 1;
    }

    conlog->appendMessage("Device connected");

    if (firstConnectFlag) {
        firstConnectFlag = false;
        connect(serialPort, SIGNAL(QSerialPort::ReadRead()), this, SLOT(handleDataFromDevice()));
    }
    return 0;
}

bool Atmega8MCU::disconnect(){
    if (serialPort->isOpen()) {
        serialPort->close();
        conlog->appendMessage("Device disconnected");
    }
    return 0;}

// GPIO methods
void Atmega8MCU::setPinValue(char portLetter, int pin,  Value value){


}

void Atmega8MCU::setPinMode(char portLetter, int pin, Mode mode){}

void Atmega8MCU::sendDataToMCU(QByteArray &data){
    serialPort->write(data);
}

void Atmega8MCU::getDataFromDevice(){
    if(!serialPort->canReadLine()) {
        conlog->error("Not enough incoming data");
        return;
    }
    const QByteArray data = serialPort->readLine();




}
