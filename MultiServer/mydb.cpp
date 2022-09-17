#include "mydb.h"
#include <QDebug>

std::string MyDB::MyTable::AddSpaces(std::string str, ui length) const{

    //находим длину строки
    ui strLen = str.length();

    //если она больше нужной длины или равна ей, то ничего не делаем
    if(length<=strLen)
        return str;

    //пока количество символов не сравняется добавляем ' ' в конец
    while(strLen != length){
        str.push_back(' ');
        ++strLen;
    }

    //возврщаем готовую строку
    return str;
}

MyDB::ui MyDB::MyTable::IdLength() const{

    //находим максимальный id
    ui maxId = rows.size()-1;

    //находим количество цифп в id
    ui numberOfDigits = 0;
    while(maxId /= 10)
        ++numberOfDigits;
    ++numberOfDigits;

    //столбец ID cостоит из двух символов, поэтому минимальная длина поля будет 2
    if(numberOfDigits==1)
        numberOfDigits++;

    //возвращаем нужное количество цифр для поля ID
    return numberOfDigits;
}

bool MyDB::MyTable::AddRow(std::vector<std::string> row){

    //если количество элементов не совпадает с количеством столбцов, то ничего не делаем
    if(row.size() != tableFormat.size())
        return false;

    //обновляем вектор самых длинных слов
    for(ui i = 0; i<row.size();++i)
        if(row[i].size()>longestWord[i])
            longestWord[i] = row[i].size();

    //добавляем строчку в таблицу
    rows.push_back(row);
    return true;
}

bool MyDB::MyTable::DeleteRow(ui id){

    //если id не принадлежит таблице, то ничего не делаем
    if(id>=rows.size())
        return false;

    //обновляем вектор самых длинных слов
    for(ui i = 0; i<rows[id].size(); ++i)
        //если мы удалили строку, в который был самый длинный элемент
        if(longestWord[i] == rows[id][i].size()){
            ui newMax = 0;
            //то мы делаем новый перебор, не учитывая удаляемую строку
            for(ui j = 0; j < rows.size(); ++j){
                if(j != id && rows[j][i].size() > newMax)
                    newMax = rows[j][i].size();
                if(tableFormat[i].size() > newMax)
                    newMax = tableFormat[i].size();
            }
            longestWord[i] = newMax;
        }

    //удаляем строку
    rows.erase(rows.begin()+id);
    return true;
}

std::string MyDB::MyTable::GetStringRow(ui id) const{

    //создаем пустую строку(это будет наш результат)
    std::string row;

    //если id не принадлежит таблице, то возвращаем пустую строку
    if(id>=rows.size())
        return row;

    //добавляем поле ID
    row = row + AddSpaces(std::to_string(id),IdLength()) +" | ";

    //добавляем другие поля
    for(ui i = 0; i<tableFormat.size() ; ++i)
        row = row + AddSpaces(rows[id][i], longestWord[i]) + " | ";

    //возвращаем строку в нужном формате
    return row;
}

std::string MyDB::MyTable::GetTableName() const{

    //возвращаем имя таблицы :/
    return tableName ;
}

void MyDB::MyTable::SetTableName(const std::string& nm){

    //устанавливаем новое имя таблицы
    tableName = nm;
}

std::string MyDB::MyTable::GetStringTableFormat() const{

    //создаем пустую строку(это будет наш результат)
    std::string format;

    //добавляем названия столбцов в строку
    for(ui i=0;i<tableFormat.size();++i)
        format = format + AddSpaces(tableFormat[i],longestWord[i]) + " | ";

    //возврщаем шапку в нужном формате
    return format;
}

void MyDB::MyTable::SetTableFormat(const std::vector<std::string>& tf){

    //можно установить формат таблицы только если она еще не инициализирована
    if(isInitialize)
        return;

    //уставливаем нужную длину вектора длин
    longestWord.resize(tf.size());

    //устанавливаем новую шапку
    tableFormat = tf;

    //обновляем длины в массиве самых длинных слов
    for(ui i = 0; i < tf.size(); ++i)
        longestWord[i] = tf[i].size();
}

std::string MyDB::MyTable::GetStringTable() const{

    //создаем пусткую строку(это будет наш результат)
    std::string table;

    //добавляем название таблицы
    table = ">>> " + GetTableName() + " <<<" + "\n";

    //находим нужную длину поля ID
    ui idLength = IdLength();

    //добавляем шапку
    table = table + AddSpaces("ID",idLength) +" | " + GetStringTableFormat() + "\n";

    //добавляем линию, которая разделяет шапку от данных
    //...для id
    for(ui j = 0; j < idLength; ++j)
        table.push_back('-');
    table += " | ";

    //...для столбцов
    for(ui i = 0; i < longestWord.size(); ++i)
    {
        for(ui j = 0; j < longestWord[i]; ++j)
            table.push_back('-');
        table += " | ";
    }
    table += "\n";

    //добавляем строки
    for(ui i = 0; i<rows.size(); ++i)
    {
        table = table + GetStringRow(i) + "\n";
    }

    //возвращаем таблицу в нужном формате
    return table;
}

//===========================================================================================

MyDB::MyDB() : tables(2)
{
    tables[0].SetTableFormat({"Name","Age","Country","Profession"});
    tables[1].SetTableFormat({"Name","Brand","Price"});

    tables[0].SetTableName("People");
    tables[1].SetTableName("Products");

    tables[0].AddRow({"Ann",    "31", "Russia",  "Teacher"});
    tables[0].AddRow({"Jack",   "26", "Britain", "Firefighter"});
    tables[0].AddRow({"Wall-E", "5",  "USA",     "Robot"});
    tables[0].AddRow({"Mike",   "81", "France",  "Pensioner"});
    tables[0].AddRow({"Willy",  "18", "USA",     "Student"});
    tables[0].AddRow({"Dima ",  "13", "Russia",  "Schoolboy"});
    tables[0].AddRow({"Seif",   "41", "UAE",     "Billionaire"});

    tables[1].AddRow({"iPhone 13",   "Apple", "799$"});
    tables[1].AddRow({"iPhone 13",   "Apple", "699$"});
    tables[1].AddRow({"AirPods 3",   "Apple", "179$"});
    tables[1].AddRow({"AirPods Pro", "Apple", "249$"});
    tables[1].AddRow({"MacBook Pro", "Apple", "1299$"});
    tables[1].AddRow({"MacBook Air", "Apple", "999$"});
    tables[1].AddRow({"Tesla S",     "Tesla", "88.740$"});
    tables[1].AddRow({"Tesla 3",     "Tesla", "39.940$"});
    tables[1].AddRow({"Tesla X",     "Tesla", "98.940$"});
    tables[1].AddRow({"Tesla Y",     "Tesla", "53.940$"});
}

std::string MyDB::GetRow(const std::string& command) const{

    //создаем новый поток в который добавляем входную строку
    //поток разобьет нашу строку с пробелами на отдельные слова(подстроки)
    std::strstream words;
    words << command;

    //считываем из потока две строки
    std::string comName, comRow;
    words>>comName>>comRow;

    //проверяем является ли числом введенный id
    bool isNumber = true;
    for(char el : comRow)
        isNumber = (isNumber && isdigit(el));

    ui indRow = 0;
    //переводим в число введенный id
    if(isNumber)
        indRow= std::stoi(comRow);
    else
        //возвращаем строку с описанной ошибкой
        return ">>>>>Your index is not a number!!\n\n";

    //находим нужную таблицу и возвращаем строку с отформатированной строкой
    for (const MyTable& tbl : tables)
    {
        if(tbl.GetTableName() == comName)
            return tbl.GetStringRow(indRow) + "\n\n";
    }

    //если мы не нашли таблицу, то возвращаем строку с описанной ошибкой
    return ">>>>>Don't know this table!!\n\n";
}

std::string MyDB::AddRow(const std::string& command){

    //создаем новый поток в который добавляем входную строку
    //поток разобьет нашу строку с пробелами на отдельные слова(подстроки)
    std::strstream words;
    words << command;

    //считываем строку с именем
    std::string comName;
    words>>comName;

    //считываем аргументы комманды в массив
    std::vector<std::string> comArgs;
    std::string word;
    while (words >> word)
        comArgs.push_back(word);

    //находим нужную таблицу и пытаемся в нее добавить строку
    for (MyTable& tbl : tables)
    {
        if(tbl.GetTableName() == comName){
            if(tbl.AddRow(comArgs))
                return ">>>>>New row was added in " + tbl.GetTableName() + "!\n\n";
            else
                //возвращаем строку с описанной ошибкой
                return ">>>>>Wrong number of Arguments!!\n\n";
        }
    }

    //если мы не нашли таблицу, то возвращаем строку с описанной ошибкой
    return ">>>>>Don't know this table!!\n\n";
}

std::string MyDB::DeleteRow(const std::string& command){

    //создаем новый поток в который добавляем входную строку
    //поток разобьет нашу строку с пробелами на отдельные слова(подстроки)
    std::strstream words;
    words << command;

    //считываем из потока две строки
    std::string comName, comRow;
    words>>comName>>comRow;

    //проверяем является ли числом введенный id
    bool isNumber = true;
    for(char el : comRow)
        isNumber = (isNumber && isdigit(el));

    ui indRow = 0;
    //переводим в число введенный id
    if(isNumber)
        indRow= std::stoi(comRow);
    else
        //возвращаем строку с описанной ошибкой
        return ">>>>>Wrong index!!\n\n";

    //находим нужную таблицу и пытаемся удалить нужную строку
    for (MyTable& tbl : tables)
    {
        if(tbl.GetTableName() == comName)
        {
            if(tbl.DeleteRow(indRow))
                return ">>>>>Row was deletted from " + tbl.GetTableName() + "\n\n";
            else
                //возвращаем строку с описанной ошибкой
                return ">>>>>Wrong index!!\n\n";
        }
    }

    //если мы не нашли таблицу, то возвращаем строку с описанной ошибкой
    return ">>>>>Don't know this table!!\n\n";
}

std::string MyDB::GetTableNames() const{

    //создаем пустую строку(это будет наш результат)
    std::string result;

    //добавляем названия таблиц в строку
    for(const MyTable& tbl : tables)
    {
        result = result + tbl.GetTableName() + "\n";
    }

    //возвращаем отформатированную строку с именами всех таблиц
    return result + "\n";
}

std::string MyDB::ChangeTableName(const std::string& command){

    //создаем новый поток в который добавляем входную строку
    //поток разобьет нашу строку с пробелами на отдельные слова(подстроки)
    std::strstream words;
    words << command;

    //считываем из потока две строки
    std::string oldName, newName;
    words>>oldName>>newName;

    //находим нужную таблицу и меняем ее название
    for (MyTable& tbl : tables)
    {
        if(tbl.GetTableName() == oldName)
        {
            tbl.SetTableName(newName);
            return oldName + " was renamed into " + newName + "!\n\n";
        }
    }

    //если мы не нашли таблицу, то возвращаем строку с описанной ошибкой
    return ">>>>>Don't know this table!!\n\n";
}

std::string MyDB::GetTableFormat(const std::string& name) const{

    //ищем нужную таблицу и возвращаем отформатированную строку с шапкой таблицы
    for (const MyTable& tbl : tables)
    {
        if(tbl.GetTableName() == name)
            return tbl.GetStringTableFormat() + "\n\n";
    }

    //если мы не нашли таблицу, то возвращаем строку с описанной ошибкой
    return ">>>>>Don't know this table!!\n\n";
}

std::string MyDB::GetTables() const{

    //создаем пустую строку(это будет наш результат)
    std::string result;

    //добавляем таблицы
    for(const MyTable& tbl : tables)
    {
        result = result + tbl.GetStringTable() + "\n\n";
    }

    //возвращаем отформатированную строку с таблицами
    return result;
}

std::string MyDB::GetTable(const std::string& name) const{

    //ищем нужную таблицу и возвращаем отформатированную строку с ней
    for (const MyTable& tbl : tables)
        if(tbl.GetTableName() == name)
            return tbl.GetStringTable() + "\n\n";

    //если мы не нашли таблицу, то возвращаем строку с описанной ошибкой
    return ">>>>>Don't know this table!!\n\n";
}

std::string MyDB::AddTable(const std::string& command){

    //создаем новый поток в который добавляем входную строку
    //поток разобьет нашу строку с пробелами на отдельные слова(подстроки)
    std::strstream words;
    words << command;

    //считываем строку с именем
    std::string comName;
    words>>comName;

    //если такая таблица уже существует, то возвращаем строку с описанной ошибкой
    for (const MyTable& tbl : tables)
        if(tbl.GetTableName() == comName)
            return tbl.GetTableName() + " is already exist!!\n\n";

    //считываем аргументы комманды в массив
    std::vector<std::string> comArgs;
    std::string word;
    while (words >> word)
        comArgs.push_back(word);

    //добавляем таблицу в базу данных
    tables.push_back(MyTable());
    tables[tables.size()-1].SetTableName(comName);
    tables[tables.size()-1].SetTableFormat(comArgs);

    //возвращаем строку с информацией
    return comName + " was created!\n\n";
}

std::string MyDB::DeleteTable(const std::string& name){

    //находим нужную таблицу и пытаемся удалить
    for(ui i = 0; i <tables.size(); ++i){
        if(tables[i].GetTableName() == name){
            tables.erase(tables.begin()+i);
            return name + " was deleted!\n\n";
        }
    }

    //если мы не нашли таблицу, то возвращаем строку с описанной ошибкой
    return ">>>>>Don't know this table!!\n\n";
}



