#include "controller.h"

#include <QSerialPort>

Controller::Controller(QObject *parent, QString name)
    :controllerName(name), serialPort(new QSerialPort(this)) {}

void Controller::pushTask(Type type, pt_port* &pin, Mode mode, Value value) {
    Task *task = new Task();
    task->port = pin;
    task->typeOfTask = Controller::SET_VALUE;
    task->valueToSet = value;
    taskQueue.enqueue(task);
}

Controller::Task* Controller::pullTask(){
    if(!taskQueue.isEmpty())
        return taskQueue.dequeue();
    else
        return nullptr;
}

void Controller::terminateTask(Task *task){
    switch(task->typeOfTask) {
        case Controller::SET_VALUE:
            task->port->setValue(task->valueToSet);
        break;
            task->port->setMode(task->modeToSet);
        case Controller::SET_MODE:
        break;
    }
}

void Controller::redoTask(Task *task){


}
