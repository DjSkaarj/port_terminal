#include "controller.h"

#include <QSerialPort>

Controller::Controller(QObject *parent, QString name)
    :controllerName(name), serialPort(new QSerialPort(this)) {}

bool Controller::isConnecedToPort() {
    return serialPort->isOpen();
}

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

        case Controller::SET_MODE:
            task->port->setMode(task->modeToSet);
        break;
    default: conlog->error("Error terminate task");
    }
    delete task;
}

void Controller::redoTask(Task *task){
    switch (task->typeOfTask) {
        case Controller::SET_VALUE:
            setPinValue(task->port, task->modeToSet, task->valueToSet);
            taskQueue.enqueue(task);
        break;

        case Controller::SET_MODE:
            setPinMode(task->port, task->modeToSet);
            taskQueue.enqueue(task);
        break;

    default: conlog->error("Error task redoing");
    }
}
