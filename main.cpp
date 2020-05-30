#include "mainwindow.h"
#include "logger.h"

#include <QApplication>
#include <QFile>

extern logger* conlog;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile stylesheet(":/style.css");
    stylesheet.open(QFile::ReadOnly);
    a.setStyleSheet(stylesheet.readAll());

    conlog = new logger;

    MainWindow w;
    w.loadCfgDirectory(":/ports");
    w.show();
    return a.exec();
}
