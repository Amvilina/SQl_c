#pragma once

#include <QThread>
#include <QTcpSocket>
#include <string>
#include <strstream>
#include "mydb.h"


//КЛАСС ПОТОКА
//При подключении клиента сервер выделяет поток для работы с ним
//Общение происходит с помощью сокета
//Потом обрабатывает входные команды пользователя и отправляет ответ ему
//Команды имеют специальный вид, которые описаны в listOfAllCommands
class MyThread : public QThread
{
    Q_OBJECT
    using ui = unsigned int;
public:

    //конструктор потока, связывает сигнали и слоты клиента и потока
    //сигналы и слоты - часть библиотеки
    MyThread(MyDB* DB, ui ID, QObject *parent = 0);

public slots:

    //вызывается каждый раз, когда клиент что-то записывает в сокет
    void readyRead();

    //вызывается, когда клиент отключается
    void disconnected();

private:

    //сам сокет :/
    QTcpSocket *socket;

    //уникальный id сокета
    ui socketDescriptor;

    //имя клиента
    std::string name;

    //сама база данных, потоки владеют одной базой данных
    MyDB* DB;

    //костыль :/
    //служебная переменная
    bool isConnected = false;

    //вспомогательная функция для вывода всех возможных команд
    std::string listOfAllCommands{"loc                              - list of all commands\n"
                                  "con 'name'                       - connect\n"
                                  "dis                              - disconnect\n"
                                  "tblgetr 'name' 'id'              - get row with 'id' from 'table'\n"
                                  "tbladdr 'name' 'Arg1' 'Arg2' ... - add row into 'Name_of_Table'\n"
                                  "tbldelr 'name' 'id'              - delete row with 'id' from 'table'\n"
                                  "lotbln                           - list of all table names\n"
                                  "ctbln 'Old_Name' 'New_Name'      - change the table name\n"
                                  "tblgetf 'name'                   - get format of the table 'name'\n"
                                  "lotbl                            - list of all tables\n"
                                  "tbl 'name'                       - get table 'name'\n"
                                  "tbladd 'name' 'Arg1' 'Arg2' ...  - add a new table\n"
                                  "tbldel 'name'                    - delete table 'name'\n\n"
                                  };

    //распознает введенную комманду
    ui CheckCommand(const std::string& str);
};


