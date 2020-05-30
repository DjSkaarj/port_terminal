#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QtAlgorithms>
#include <QDir>
#include <utility>
#include "logger.h"
#include "config.h"
#include "controller.h"

extern logger* conlog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// MainWindow ----------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void loadCfgDirectory(const QString &directory);

public slots:
    void loadCfg(int);
    void about();
    void enableTerminal();
    void disableTerminal();
    void connectMCU();
    void disconnectMCU();

signals:
    void cfgChanged(const std::vector<std::pair<QString, QString>>*);

private:
    Ui::MainWindow *ui;
    config *_cfg;
    std::vector<std::pair<QString, QString>> config_list;
    Controller *mcu;

    void clearPorts();

    void fillPortsInfo();
};

//--------------------------------------------------------------------------

class ioMode: public QComboBox
{
    Q_OBJECT

public:
    ioMode(QWidget* parent = nullptr);
    ~ioMode() {}

    pt_port *port() const;
    void setPort(pt_port *port);

private:
    pt_port* _port;
};

//-----------------------------------------------------------------------------

class cfgBox: public QComboBox
{
    Q_OBJECT

public:
    cfgBox(QWidget* parent = nullptr) : QComboBox(parent) {}
    ~cfgBox() {}

public slots:
    void updateList(const std::vector<std::pair<QString, QString>>*);
};

//------------------------------------------------------------------------------

class valueButton: public QPushButton
{
    Q_OBJECT

public:
    valueButton(QWidget* parent = nullptr) : QPushButton(parent) {}
    ~valueButton() {}

    pt_port *port() const;
    void setPort(pt_port *port);

public slots:
    void updateText(int value);

private:
    pt_port* _port;
};

#endif // MAINWINDOW_H
