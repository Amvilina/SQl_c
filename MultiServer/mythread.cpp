#include "mythread.h"

//Список всех возможных команд
enum Commands{
    notACommand,
    listOfAllCommands,
    connect,
    disconnect,
    tableGetRow,
    tableAddRow,
    tableDeleteRow,
    listOfAllTableNames,
    tableChangeName,
    tableGetFormat,
    listOfAllTables,
    getTable,
    addTable,
    deleteTable
};

MyThread::MyThread(MyDB* in_DB, ui ID, QObject* parent) : QThread(parent),DB(in_DB)
{
    //устанавливаем сокету уникальный номер
    this->socketDescriptor = ID;

    //создаем новый сокет
    socket = new QTcpSocket();
    socket->setSocketDescriptor(this->socketDescriptor);

    //связываем сигналы и слоты
    connect(socket,SIGNAL(readyRead()), this, SLOT(readyRead()),Qt::DirectConnection);
    connect(socket,SIGNAL(disconnected()), this, SLOT(disconnected()),Qt::DirectConnection);

    //после этой команды наш потом начинает 'жить' пока его не закроют
    exec();
}

void MyThread::readyRead(){

    //считываем данные из сокета
    QByteArray Data = socket->readAll();
    std::string str(Data);

    //если клиент уже подключен, то выводим, что он нам отправил
    if(isConnected)
        std::cout<<name<<" entered \""<<Data.data()<<"\"\n";

    //определяем команду
    int command = CheckCommand(str);

    std::cout<<command<<std::endl;
    //выполняем команду :/
    //во многих команд есть str.substr(i,str.length()-i)
    //эта команда возвращает строку без команды
    switch (command) {
    case Commands::notACommand:
        socket->write(">>>>>Sorry, I don't know this command!!\n\n");
        socket->waitForBytesWritten(500);
        break;

    case Commands::listOfAllCommands:
        socket->write(listOfAllCommands.data());
        socket->waitForBytesWritten(500);
        break;

    case Commands::connect:
        if(isConnected)
        {
            socket->write(">>>>>I'm sorry but you're already connected!!\n\n");
            socket->waitForBytesWritten(500);
        }else
        {
            isConnected = true;
            name = str.substr(4,str.length()-4);
            std::cout<<name<<" just connected!\n";
            socket->write(">>>>>You are connected! For list of commands type: loc\n\n");
            socket->waitForBytesWritten(500);
        }
        break;

    case Commands::disconnect:
        socket->write("Well actually this never will be evaluated cause I disconnect client from his side");
        socket->waitForBytesWritten(500);
        break;

    case Commands::tableGetRow:
        socket->write(DB->GetRow(str.substr(8,str.length()-8)).data());
        socket->waitForBytesWritten(500);
        break;

    case Commands::tableAddRow:
        socket->write(DB->AddRow(str.substr(8,str.length()-8)).data());
        socket->waitForBytesWritten(500);
        break;

    case Commands::tableDeleteRow:
        socket->write(DB->DeleteRow(str.substr(8,str.length()-8)).data());
        socket->waitForBytesWritten(500);
        break;

    case Commands::listOfAllTableNames:
        socket->write(DB->GetTableNames().data());
        socket->waitForBytesWritten(500);
        break;

    case Commands::tableChangeName:
        socket->write(DB->ChangeTableName(str.substr(6,str.length()-6)).data());
        socket->waitForBytesWritten(500);
        break;

    case Commands::tableGetFormat:
        socket->write(DB->GetTableFormat(str.substr(8,str.length()-8)).data());
        socket->waitForBytesWritten(500);
        break;

    case Commands::listOfAllTables:
        socket->write(DB->GetTables().data());
        socket->waitForBytesWritten(500);
        break;

    case Commands::getTable:
        socket->write(DB->GetTable(str.substr(4,str.length()-4)).data());
        socket->waitForBytesWritten(500);
        break;

    case Commands::addTable:
        socket->write(DB->AddTable(str.substr(7,str.length()-7)).data());
        socket->waitForBytesWritten(500);
        break;

    case Commands::deleteTable:
        socket->write(DB->DeleteTable(str.substr(7,str.length()-7)).data());
        socket->waitForBytesWritten(500);
        break;

    default:
        socket->write("Some troubles");
        socket->waitForBytesWritten(500);
        break;
    }


}

void MyThread::disconnected(){

    std::cout<<name<<" disconnected :(\n";

    //библиотека сама закроет и удалит связь и отчистит память
    socket->deleteLater();
    exit(0);
}

MyThread::ui MyThread::CheckCommand(const std::string& str){

    //считаем количество пробелов в команде
    ui numberOfSpaces = 0;
    for(ui i = 0; i < str.length(); ++i)
        if(str[i] == ' ')
            ++numberOfSpaces;

    //узнаем какая команда
    if(str.substr(0,3)=="loc"     && numberOfSpaces==0)
        return Commands::listOfAllCommands;
    if(str.substr(0,3)=="con"     && numberOfSpaces==1)
        return Commands::connect;
    if(str.substr(0,3)=="dis"     && numberOfSpaces==0)
        return Commands::disconnect;
    if(str.substr(0,7)=="tblgetr" && numberOfSpaces==2)
        return Commands::tableGetRow;
    if(str.substr(0,7)=="tbladdr" && numberOfSpaces>=2)
        return Commands::tableAddRow;
    if(str.substr(0,7)=="tbldelr" && numberOfSpaces==2)
        return Commands::tableDeleteRow;
    if(str.substr(0,6)=="lotbln"  && numberOfSpaces==0)
        return Commands::listOfAllTableNames;
    if(str.substr(0,5)=="ctbln"   && numberOfSpaces==2)
        return Commands::tableChangeName;
    if(str.substr(0,7)=="tblgetf" && numberOfSpaces==1)
        return Commands::tableGetFormat;
    if(str.substr(0,5)=="lotbl"   && numberOfSpaces==0)
        return Commands::listOfAllTables;
    if(str.substr(0,4)=="tbl "    && numberOfSpaces==1)
        return Commands::getTable;
    if(str.substr(0,6)=="tbladd"  && numberOfSpaces>=2)
        return Commands::addTable;
    if(str.substr(0,6)=="tbldel"  && numberOfSpaces==1)
        return Commands::deleteTable;

    //если не подходит ни под один из шаблонов
    return Commands::notACommand;
}
