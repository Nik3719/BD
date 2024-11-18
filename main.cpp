#include <iostream>
#include <string>
#include "BD.h"
#include "DL.h"
#include "Tools.h"



//INSERT INTO таблица1 VALUES ('1234','dfdgd')
/*
SELECT таблица1.колонка1, таблица1.колонка3\
FROM таблица1\
WHERE таблица1.колонка1 = '1234' OR таблица1.колонка1 = '0'
*/
int main(int argc, char const *argv[])
{
    while(true)
    {
        string sheme = "/home/nik/Desktop/BD/BD/src/shema.json";
        BD bd(sheme);
        string command = input();
        DL<string> lexems = splitString(command);
        bd.separateCommand(lexems);
    }

    return 0;
}
