#include <iostream>
#include <string>
#include "BD.h"
#include"Tools.h"
#include "DL.h"


using namespace std;

// INSERT INTO таблица1 VALUES ('somedata','12345')

// SELECT таблица1.колонка1, таблица2.колонка4\
// FROM таблица1, таблица2\
// WHERE таблица1.колонка1 = таблица2.колонка2


//INSERT INTO таблица2 VALUES ('somedata','12345','23456','dfgrdsfgradeg')

// SELECT таблица1.колонка1\
// FROM таблица1

/*DELETE FROM таблица1\
WHERE таблица1.колонка1 = '123'*/

/*
SELECT таблица1.колонка7, таблица1.колонка5\
FROM таблица1\
WHERE таблица1.колонка7 = '4' AND таблица1.колонка5 = '2019/20'*/

/*
SELECT таблица1.колонка5, таблица1.колонка1\
FROM таблица1\
WHERE таблица1.колонка5 = '2019/20' AND таблица1.колонка1 = 'Total'*/






/*
SELECT таблица1.колонка1, таблица2.колонка4\
FROM таблица1, таблица2\
WHERE таблица1.колонка1 = таблица2.колонка2 AND таблица1.колонка1 = 'somedata' OR таблица1.колонка1 = '1'
*/

int main()
{
    string shema ="/home/nik/Desktop/BD/src/shema.json";
    BD BD1(shema);
   
    while (true)
    {
        string command = input();
        DL<string> split = splitString(command);
        //DL<string> exm = {"SELECT", "таблица1.колонка1," "таблица2.колонка4", "FROM", "таблица1,", "таблица2", "WHERE", "таблица1.колонка1", "=", "таблица2.колонка2", "AND", "таблица1.колонка1", "=", "'somedata'", "OR", "таблица1.колонка1", "=", "'1'"};
        BD1.separateCommand(split);
    }
    
/*
SELECT таблица1.колонка4\             
FROM таблица1\
WHERE таблица1.колонка4 = '653ytgrfs'*/

    return 0;
}