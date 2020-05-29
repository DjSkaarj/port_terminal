#include "mainwindow.h"
#include "ui_mainwindow.h"

#define NUM_MODES 2
QString port_modes[NUM_MODES] = {
    "output",   // 0
    "input"     // 1
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    conlog->setMsgbox(ui->console);
    conlog->setLogFile("log.txt");

    connect(this, SIGNAL(cfgChanged(const std::vector<std::pair<QString, QString>>*)), ui->cfgComboBox, SLOT(updateList(const std::vector<std::pair<QString, QString>>*)));
    connect(ui->cfgComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadCfg(int)));
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
    if(buff->load(config_list[num_config - 1].first))
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

        QToolButton* settings_button = new QToolButton;

        ioMode* mode_list = new ioMode;
        mode_list->setPort(v);

        valueButton* value_button = new valueButton;
        value_button->setPort(v);

        port_layout->addWidget(settings_button);
        port_layout->addWidget(mode_list);
        port_layout->addWidget(value_button);
    }
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

void ioMode::setPort(pt_port *port)
{
    if(_port != port)
    {
        _port = port;
        connect(this, SIGNAL(currentIndexChanged(int)), port, SLOT(setMode(int)));
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
        connect(this, SIGNAL(clicked()), port, SLOT(switchValue()));
        connect(port, SIGNAL(valueChanged(int)), this, SLOT(updateText(int)));
    }
}

void valueButton::updateText(int value)
{
    setText(QString::number(value));
}
