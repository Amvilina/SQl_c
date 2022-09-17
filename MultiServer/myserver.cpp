#include "myserver.h"

MyServer::MyServer(QObject *parent):QTcpServer(parent)
{
    //'слушаем' входные сигнали по адресу "127.0.0.1", порт: 1234
    if(!this->listen(QHostAddress::Any,1234)){
        std::cout<<"Something wrong with listening\n";
    }else{
        std::cout<<"Listening...\n";
    }
}

void MyServer::incomingConnection(qintptr socketDescriptor){

    //создаем новый поток, который будет управляться библиотекой
    //мы не должны заботиться об его уничтожении
    MyThread *thread = new MyThread(&DB, socketDescriptor, this);

    //соединяем потом с командой удаления
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));

    //запускаем потом
    thread->start();
};

