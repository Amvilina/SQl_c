#include "myclient.h"
#include <iostream>
#include <string>

MyClient::MyClient(QObject *parent) : QObject(parent){

    //создается сокет и соединяются слоты с сигналами
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket,SIGNAL(disconnected()),this, SLOT(sockDisc()));

    //ожидаем соединения
    waitForConnection();
}

void MyClient::waitForConnection(){

    std::cout<<">>>>>Hello client!! If you wanna connect, just type: con 'Name'\n\n";

    std::string str;

    //сидим в бесконечном цикле, пока не получим нужную команду
    while(true){

        //cчитываем строчку и получаем первые 3 буквы
        std::getline(std::cin,str);
        std::string com = str.substr(0,3);

        //проверяем эти 3 буквы на соответсвие командам
        if(com=="con" && str.length()>4){

            //подключаемся к серверу
            socket->connectToHost("127.0.0.1", 1234);

            //отправляем команду подключения серверу
            if (socket->isOpen()){
                socket->write(str.data());
                socket->waitForBytesWritten(500);
            }else{
                std::cout<<"Can't send the message!!\n\n";
            }
            break;

        }else{
            std::cout<<"Don't know such a command!!\n\n";
        }
    }
}

void MyClient::sockReady(){

    if(socket->waitForConnected(500)){
        socket->waitForReadyRead(500);

        //считываем данные из сокета
        Data = socket->readAll();

        //выводи полученную информацию
        std::cout<<Data.data();

        //считываем новую команду
        std::string str;
        std::getline(std::cin,str);

        //если это команду отключения, то отключаемся :/
        //мы не отправляем эту команду на сервер, так как есть связь засчет сигналов-слотов
        if(str=="dis")
            exit(0);

        //отправляем новую команду серверу
        if (socket->isOpen()){
            socket->write(str.data());
            socket->waitForBytesWritten(500);
        }
    }

}

void MyClient::sockDisc(){

    //сокет удалится библиотекой
    socket->deleteLater();
}




