#pragma once

#include <QTcpServer>
#include "mythread.h"

//КЛАСС СЕРВЕР
//Выделяет потоки, которые связаны с клиентами
//Сервер хранит базу данных
class MyServer : public QTcpServer
{
    Q_OBJECT
public:

    //Конструктор
    MyServer(QObject *parent = 0);

    //База данных
    MyDB DB;

private:

    //вызывается каждый раз, когда клиент пытается подключиться
    void incomingConnection(qintptr socketDescriptor);
};

