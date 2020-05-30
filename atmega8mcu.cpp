#include "atmega8mcu.h"
#include "commands.h"

#include <QSerialPort>

Atmega8MCU::Atmega8MCU(QObject *parent, QString name)
    : Controller(parent, name), firstConnectFlag(true) {}

bool Atmega8MCU::connectToDevice(const QString &serialPortName) {
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

    emit Controller::controllerConnected();
    return 0;
}

bool Atmega8MCU::disconnect(){
    if (serialPort->isOpen()) {
        serialPort->close();
        conlog->appendMessage("Device disconnected");

        emit Controller::controllerDisconnected();
    }
    return 0;
}

// GPIO methods------------------------------------------------------------------------
void Atmega8MCU::setPinValue(pt_port* pin, Mode mode, Value newValue){
    QByteArray request;
    switch(mode){
        case Controller::OUTPUT:
            request.append(GPIO);
            request.append(SET);
            request.append(VALUE);
            request.append(pin->getPortLetter());
            if (newValue == HIGH) {
                request.append(HIGH_STATE);
                request.append((1 << pin->getPinNumber()));
            }
            else {
                request.append(LOW_STATE);
                request.append(~(1 << pin->getPinNumber()));
            }
            pushTask(Controller::SET_VALUE, pin, mode, newValue);
        break;

        case Controller::INPUT:
            // nothing for now
        break;
    }
    if(request.length() == 0)
        return;
    else sendDataToMCU(request);
}

void Atmega8MCU::setPinMode(pt_port* pin, Mode mode){}

//--------------------------------------------------------------------------------

void Atmega8MCU::sendDataToMCU(QByteArray &data){
    conlog->info("Send request to MCU:" + data);
    serialPort->write(data);
}

void Atmega8MCU::getDataFromDevice(){
    if(!serialPort->canReadLine()) {
        conlog->error("Not enough incoming data");
        return;
    }

    QByteArray response = serialPort->readLine();
    responseHandler(response);

}
//-----------------------------------------------------------------------------------

void Atmega8MCU::responseHandler(QByteArray &response){
   switch (response[0]) {
       case ACK:
            terminateTask(taskQueue.dequeue());
       break;
       case ERROR:
            errorResponseHandler(response);
       break;
       case LOG_INFO:
            conlog->info("MCU:"+response.right(response.size()-1));
       break;
       default:
           conlog->error("Undefined response from MCU:"+response);
   }
}

void Atmega8MCU::errorResponseHandler(QByteArray &response){
    switch(response[1]) {
        case TASK:
            if(!taskQueue.isEmpty()) {
                redoTask(taskQueue.dequeue());
            }
            else
                conlog->error("MCU task response for no existing task:" + response.right(response.size()-1));
        break;
        default:
            conlog->error("MCU error task response:" + response.right(response.size()-1));
    }
}
