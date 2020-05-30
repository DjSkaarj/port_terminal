#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include "atmega8mcu.h"

#define NUM_MODES 2
QString port_modes[NUM_MODES] = {
    "output",   // 0
    "input"     // 1
};

const QString PT_VERSION = "1.0";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      mcu(nullptr)
{

    ui->setupUi(this);
    conlog->setMsgbox(ui->console);
    conlog->setLogFile("log.txt");

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(this, SIGNAL(cfgChanged(const std::vector<std::pair<QString, QString>>*)), ui->cfgComboBox, SLOT(updateList(const std::vector<std::pair<QString, QString>>*)));
    connect(ui->cfgComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadCfg(int)));

    mcu = new Atmega8MCU(this, "Atmega");

    connect(mcu, SIGNAL(controllerConnected()), this, SLOT(enableTerminal()));
    connect(mcu, SIGNAL(controllerDisconnected()), this, SLOT(disableTerminal()));

    connect(ui->connectMCU, SIGNAL(clicked()), this, SLOT(connectMCU()));
    connect(ui->disconnectMCU, SIGNAL(clicked()), this, SLOT(disconnectMCU()));

    ui->Left_ports->setAlignment(Qt::AlignRight);
    ui->Right_ports->setAlignment(Qt::AlignLeft);

    ui->disconnectMCU->setEnabled(false);
    ui->container_widget->setEnabled(false);

    fillPortsInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadCfg(int num_config)
{
    clearPorts();

    if(num_config == -1 || num_config == 0)
        return;

    config *buff = new config;
    if(buff->load(config_list[num_config - 1].first, mcu))
    {
        conlog->error("Failed to load configuration.");
        return;
    }

    _cfg = buff;

    ui->mcu_rect->setText(_cfg->name());

    for(auto &v : _cfg->ports)
    {
        QHBoxLayout* port_layout = new QHBoxLayout;

        if(v->align())
            ui->Right_ports->addLayout(port_layout);
        else
            ui->Left_ports->addLayout(port_layout);

        QLabel* port_name = new QLabel(v->name());
        port_layout->addWidget(port_name);

        if(v->service())
            continue;

        QToolButton* settings_button = new QToolButton();

        ioMode* mode_list = new ioMode();
        mode_list->setPort(v);

        valueButton* value_button = new valueButton();
        value_button->setPort(v);

        port_layout->addWidget(settings_button);
        port_layout->addWidget(mode_list);
        port_layout->addWidget(value_button);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, "About Port Terminal",
                       "<b>Port Terminal v." + PT_VERSION + "</b><br>Powered by Qt " + QT_VERSION_STR + "<br>© Demichev S., Gerasimenko E.");
}

void MainWindow::enableTerminal()
{
    ui->container_widget->setEnabled(true);
    ui->disconnectMCU->setEnabled(true);
    ui->serialPortInfoListBox->setEnabled(false);
    ui->cfgComboBox->setEnabled(false);
    ui->connectMCU->setEnabled(false);
}

void MainWindow::disableTerminal()
{
    ui->container_widget->setEnabled(false);
    ui->connectMCU->setEnabled(true);
    ui->serialPortInfoListBox->setEnabled(true);
    ui->cfgComboBox->setEnabled(true);
    ui->disconnectMCU->setEnabled(false);
}

void MainWindow::loadCfgFiles(const QString &directory)
{
    QDir dir(directory);
    if (!dir.exists())
    {
        conlog->error("Cannot find given directory.");
        return;
    }

    config_list.clear();

    QFileInfoList list = dir.entryInfoList();

    for(QFileInfo &i : list)
    {
        if(i.suffix() != "cfg")
            continue;

        QFile cur(i.filePath());
        if (!cur.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        QTextStream tmp(&cur);
        QString mcu_name;
        tmp >> mcu_name;

        config_list.push_back(std::pair<QString, QString>(i.filePath(), mcu_name));
    }

    emit cfgChanged(&config_list);
}

void MainWindow::connectMCU() {
    if(!mcu->isConnecedToPort() && (ui->serialPortInfoListBox->currentText() != "none")) {
        mcu->connectToDevice(ui->serialPortInfoListBox->currentText());
    }
}

void MainWindow::disconnectMCU(){
    if(mcu->isConnecedToPort())
        mcu->disconnect();
}

void MainWindow::clearPorts()
{
    QLayoutItem *pl;
    QLayoutItem *cl;

    while ((pl = ui->Left_ports->takeAt(0)) != 0)
    {
        while((cl = pl->layout()->takeAt(0)) != 0)
            delete cl->widget();
        delete pl;
    }

    while ((pl = ui->Right_ports->takeAt(0)) != 0)
    {
        while((cl = pl->layout()->takeAt(0)) != 0)
            delete cl->widget();
        delete pl;
    }

}

void MainWindow::fillPortsInfo()
{
    ui->serialPortInfoListBox->clear();
    ui->serialPortInfoListBox->addItem("<none>");
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto portList = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : portList) {
        QStringList list;
        list << portInfo.portName()
             << (!description.isEmpty() ? description : "N/A")
             << (!manufacturer.isEmpty() ? manufacturer : "N/A")
             << (!serialNumber.isEmpty() ? serialNumber : "N/A")
             << portInfo.systemLocation()
             << (portInfo.vendorIdentifier() ? QString::number(portInfo.vendorIdentifier(), 16) : "N/A")
             << (portInfo.productIdentifier() ? QString::number(portInfo.productIdentifier(), 16) : "N/A");

        ui->serialPortInfoListBox->addItem(list.first(), list);
    }
}

//------------------------------------------------------------------------------------------------

ioMode::ioMode(QWidget* parent):
    QComboBox(parent)
{
    for(int i = 0; i < NUM_MODES; i++)
        addItem(port_modes[i], i);
}

pt_port *ioMode::port() const
{
    return _port;
}

// changing the mode of the port
void ioMode::setPort(pt_port *port)
{
    if(_port != port)
    {
        _port = port;
        connect(this, SIGNAL(currentIndexChanged(int)), port, SLOT(changePortMode(int)));
    }
}

void cfgBox::updateList(const std::vector<std::pair<QString, QString>>* list)
{
    clear();
    addItem("<none>", "<none>");
    for(const auto &v : *list)
        addItem(v.second, v.second);

    setCurrentIndex(0);
}

pt_port *valueButton::port() const
{
    return _port;
}

void valueButton::setPort(pt_port *port)
{
    if(_port != port)
    {
        _port = port;
        setText(QString::number(port->value()));
        connect(this, SIGNAL(clicked()), port, SLOT(changePortValue()));
        connect(port, SIGNAL(portValueChangeConfirmed(int)), this, SLOT(updateText(int)));
    }
}

void valueButton::updateText(int value)
{
    setText(QString::number(value));
}
