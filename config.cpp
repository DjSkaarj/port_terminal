#include "config.h"

config default_cfg;

bool config::load(const QString &path)
{
    bool align = 0;

    QFile cfg(path);
    if (!cfg.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        conlog->error("Failed to load " + path + " MCU configuration file.");
                return 1;
    }

    _name = cfg.readLine();

    while (!cfg.atEnd())
    {
        QString line = cfg.readLine();
        QTextStream ts(&line);
        QString cmd;

        ts >> cmd;
        if(cmd == "left")
            align = 0;
        else if(cmd == "right")
            align = 1;
        else if(cmd == "p")
        {
            pt_port *buff = new pt_port();
            QString bname;
            int bnum;

            ts >> bnum;
            ts >> bname;

            buff->setName(bname);
            buff->setNum(bnum);
            buff->setAlign(align);

            ports.push_back(buff);
        }
        else continue;
    }

    conlog->info("MCU configuration " + path + " was successfully loaded.");
    return 0;
}

QString config::name() const
{
    return _name;
}

void config::setName(const QString &name)
{
    _name = name;
}