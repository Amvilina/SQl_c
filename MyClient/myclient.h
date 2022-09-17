#pragma once

#include <QObject>
#include <QTcpSocket>
#include <string>

//КЛАСС КЛИЕНТ
//Отправляет команды на сервер
//Может получать данны из БД и редактировать ее
//Обмен информации через сокет
//connect и disconnect обрабатываются на стороне клиента
class MyClient : public QObject
{
    Q_OBJECT
public:

    //конструктор
    explicit MyClient(QObject *parent = nullptr);

    //сокет :/
    QTcpSocket* socket;

    //дата
    QByteArray Data;

    //вспомогательная функция, ждет команды подключения
    void waitForConnection();

public slots:

    //вызывается каждый раз, когда сервер пишет что-то в сокет
    void sockReady();

    //вызывается, когда соединение разрываетися
    void sockDisc();

};

