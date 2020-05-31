#include "config.h"

bool config::load(const QString &path, Controller *mcu)
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
        else if(cmd == "p" || cmd == "r")
        {
            pt_port *buff = new pt_port(mcu);
            QString bname;
            int bnum;

            ts >> bnum;
            ts >> bname;

            buff->setName(bname);
            buff->setNum(bnum);
            buff->setAlign(align);

            if(cmd == "r")
                buff->setService(true);

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

pt_port* config::port(const QString &name)
{
    for(auto &p : ports)
    {
        if(p->name() == name)
            return p;
    }
}
