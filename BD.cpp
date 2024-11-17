#include "BD.h"

BD::BD(string schema)
{
    parsSheme(schema);
}

void BD::pk_sequence(string path, string nameTable)
{
    string nameFile = path + "/" + nameTable + "_pk_sequence.txt";
    ofstream CreatePK_sequence(nameFile);
    CreatePK_sequence << 0;
    CreatePK_sequence.close();
}

void BD::lock(string path, string nameTable)
{
    string nameFile = path + "/" + nameTable + "_lock.txt";
    ofstream CreatePK_sequence(nameFile);
    CreatePK_sequence << 0;
    CreatePK_sequence.close();
}

	
	
void BD::parsSheme(string schema)
{
    ifstream readschema(schema); // write in data
    if (!readschema.is_open())
    {
        cout << "shema dont open\n";
        return;
    }
    json data = json::parse(readschema);
    //readschema >> data;
    readschema.close();

    string dirName = data["name"];
    this->nameSheme = dirName;

    //(mkdir(dirName, 0755);

    filesystem::create_directory(dirName);

    this->tuple_limits = data["tuples_limit"];
    json structureColumns = data["structure"];

    for (const auto& table : structureColumns.items())
    {
        string nameOfTable = table.key();
        string path = dirName + '/' + nameOfTable;
        filesystem::create_directory(path);
        
        pk_sequence(path, nameOfTable);
        lock(path, nameOfTable);

        DL<string> columns;
        for (string columnName : table.value())
        {
            columns.LDPUSHT(columnName);
        }
        DataOfTable newTable(nameOfTable, path, columns);
        this->structure.HSET(nameOfTable, newTable);
        
        string pathForCSVFile = path + "/" + "1.csv";
        if (!filesystem::exists(pathForCSVFile))
        {
            ofstream csvFile(pathForCSVFile);
            csvFile.close();
        }
    }
}

string BD::separateCommand(DL<string>& command)
{
    if (command[0]=="SELECT")
    {
        return SELECT(command);
    }
    else if (command[0] == "INSERT")
    {
        return INSERT(command);
    }
    else if (command[0]=="DELETE")
    {
        return DELETE(command);
    }
    else
    {
        return "incorrect command\n";
    }
}

DL<string> BD::parsForInsert(string values)
{
    int len = values.size();
    DL<string> res;
    for(int i=0; i< len; i++)
    {
        if (values[i] == '\'')
        {
            string oneValue;
            while(values[++i]!='\'') oneValue += values[i];
            res.LDPUSHT(oneValue);
        }
    }
    return res;
}

string BD::INSERT(DL<string>& command)
{
    string tableName = command[2];
    if (rqstForIntervention(tableName))
    {
        string insertvalues = command[4];
        DL<string> values = parsForInsert(insertvalues);

        if (values.len < structure[tableName].columns.len)
        {
            while (values.len < structure[tableName].columns.len)
            {
                values.LDPUSHT("None");
            }
        }

        string numberOfLastFile = GetNumberOfLastFile(tableName);
        string path = structure[tableName].path + "/" +  numberOfLastFile + ".csv";

        // lock
        ForbidIntervention(tableName);
        ifstream readCSVFile(path);
        string table;
        int countRow = -1;
        while (!readCSVFile.eof())
        {
            string buf;
            getline(readCSVFile, buf);
            //buf+='\n';
            countRow += 1;
            if (!buf.empty())
            {
                table += buf + "\n";
            }
            buf.clear();
        }
        readCSVFile.close();

        if (countRow == tuple_limits)
        {
            PlusOne(numberOfLastFile);
            path = structure[tableName].path + "/" +  numberOfLastFile + ".csv";
            CreateNewCSVFile(path);
            table = "";
        }

        for (int i = 0; i < structure[tableName].columns.len; i++)
        {
            if (i-1 != structure[tableName].columns.len)
            {
                table += values[i] + ',';
            }
            else 
            {
                table += values[i];
            }
            
        }
        table += '\n';

        ofstream writeCSVFile(path);
        writeCSVFile << table;
        writeCSVFile.close();
        allowIntervention(tableName);
        PlusOneSequence(tableName);

    }
    else
    {
        return "Вмешательство запрещено\n";
    }
    return "0";



}



bool BD::rqstForIntervention(string tableName)
{
    string path = structure[tableName].path + "/" + tableName + "_lock.txt";
    ifstream fileRead(path);
    int value;
    fileRead >> value;
    fileRead.close();

    if (value==1)
    {
        return false;
    }
    else if (value == 0)
    {
        return true;
    }
    else
    {
        cout << "Ошибка в lock " << tableName << '\n';
    }
}

string BD::GetNumberOfLastFile(string tableName)
{
    string startNum = "1";
    string oldNum;
    while(true)
    {
        string path  = structure[tableName].path + "/" + startNum + ".csv";
        if (filesystem::exists(path))
        {
            oldNum = startNum;
            PlusOne(startNum);
        }
        else
        {
            break;
        }
    }
    return (oldNum);
}

void BD::CreateNewCSVFile(string path)
{
    ofstream NewFile(path);
    NewFile.close();

}

void BD::ForbidIntervention(string tableName)
{
    string path = structure[tableName].path + "/" + tableName + "_lock.txt";
    ofstream fileRead(path);
    fileRead << 1;
    fileRead.close();
}



void BD::allowIntervention(string tableName)
{
    string path = structure[tableName].path + "/" + tableName + "_lock.txt";
    ofstream fileRead(path);
    fileRead << 0;
    fileRead.close();
}

void BD::PlusOneSequence(string tableName)
{
    string path = structure[tableName].path + "/" + tableName + "_pk_sequence.txt";
    ifstream fileRead(path);
    int num;
    fileRead >> num;
    fileRead.close();
    num++;

    ofstream fileWrite(path);
    fileWrite << num;
    fileWrite.close();
}

Pair<string, string> ParsingLexem(string str)
{
    string one, two;
    bool isOne = true;
    for(char sym: str)
    {
        if (sym==',') continue;
        if (sym == '.')
        {
            isOne = false;
            continue;
        }
        if (isOne)
        {
            one  += sym;
        }
        else if (!isOne)
        {
            two += sym;
        }
    }
    return {one,two};
}

string BD::PrintColumn(print& ForPrint, DL<Pair<string, string>>& Lexems)
{
    int countColumn = Lexems.len;
    DL<DL<string>> table;
    for (int i = 0;i < countColumn;i++)
    {
        table.LDPUSHT(DL<string>());
    }
    for (int i = 0; i < Lexems.len; i++)
    {
        DL<string> column = ForPrint[Lexems[i].first][Lexems[i].second];
        for (int k = 0; k < column.len; k++)
        {
            if (column[k] != "DELETE")
            {
                table[i].LDPUSHT(column[k]);
            }
            else
            {
                table[i].LDPUSHT("            ");
            }
        }

    }
    string res;
    for (int i = 0; i < Lexems.len; i++)
    {
        res += Lexems[i].first + "." + Lexems[i].second;
        cout << Lexems[i].first << "." << Lexems[i].second << "\t";
    }
    res += '\n';
    cout << "\n";
    int index = 0;
    DL<int> forbiddenIndexes;
    while (true)
    {
        for (int i = 0;i < countColumn;i++)
        {
            if (forbiddenIndexes.search(i)) continue;
            if (table[i].len <= index+1)
            {
                forbiddenIndexes.LDPUSHT(i);
                continue;
            }
            string elem = table[i][index];
            res += elem + "                   ";
            cout << elem << "                   ";
            

        
        }
        res += '\n';
        cout << "\n";
        index++;
        if(forbiddenIndexes.len == countColumn) break;
   }

   return res;

}

string BD::SELECT(DL<string>& command)
{
    int len = command.len;
    DL<Pair<string,string>> Lexems;
    bool wasFrom  = false;
    DL<string> tables;
    DL<Pair<string,string>> allLexemFromWhere;
    bool isWhere = false;
    int indexWhere;
    Tree operatorsTree;
    for (int i = 1; i < len; i++)
    {
        if (command[i] == "FROM")
        {
            wasFrom = true;
            i++;
            while (i < len && command[i]!="WHERE") // all tables
            {
                if (command[i][command[i].size()-1] == ',')
                {
                    command[i] = command[i].substr(0, command[i].length() - 1);
                }

                tables.LDPUSHT(command[i++]);
            }
            i--;


            
            for (int j=0;j<tables.len;j++)
            {
                if (!rqstForIntervention(tables[j])) return "forbidden";
            }

            for (int j=0;j<tables.len;j++)
            {
                ForbidIntervention(tables[j]);
            }
            
            
            continue;
        }
        if (!wasFrom)
        {
            Pair<string,string> lexem = ParsingLexem(command[i]);
            Lexems.LDPUSHT(lexem);
        }
        else if (wasFrom)
        {
            while ( i < len && command[i]!="WHERE")
            {
                i++;
            }

            if (command[i] == "WHERE") 
            {
                isWhere = true;
                indexWhere = i+1;
                operatorsTree = WHERE(command, Lexems, allLexemFromWhere);
            }  
            break;     
        }
    }

    if (!isWhere)
    {
        allLexemFromWhere = Lexems;
    }


    print ForPrint;
    CreateStructure(allLexemFromWhere, ForPrint);
    
    readFile(allLexemFromWhere, ForPrint);

    for (int i=0;i < Lexems.len;i++)
    {
        string table = Lexems[i].first;
        string col = Lexems[i].second;
        if (isWhere)
        {
            DL<string>  oneColumn = fillter(ForPrint, operatorsTree.root,table, col).second;
            if (oneColumn.head!=oneColumn.tail) // there is not conditions
            {
                ForPrint[table][col] = oneColumn;
            }
        }

    }
    // возможно стоит добавить что если в тсроке есть делит то вся строка не выводиться
    // перебрать ForPrint если один из делит то все делит
    // создать функцию
    // например чтобы при
/*
    SELECT таблица1.колонка1\
    FROM таблица1\
    WHERE таблица1.колонка7 = '748'

        колонка 7 отфильтровалась

*/

    // for (int i = 0; i < tables.len;i++)
    // {
    //     specialSelect(tables[i],allLexemFromWhere, ForPrint);
    // }

    string res = PrintColumn(ForPrint,Lexems);


    for (int j=0;j<tables.len;j++) 
    {
        allowIntervention(tables[j]);
    }
    return res;


}

void BD::specialSelect(string table, DL<Pair<string,string>>& allLexemsFromWhere, print& ForPrint)
{
    int countColumn = ForPrint[table].len;
    DL<string> keys = ForPrint[table].getKeys();
    int lenColumns = ForPrint[table][keys[0]].len;

    for (int k = 0;k < lenColumns; k++)
    {
        for (int i = 0; i < keys.len; i++)
        {
            if (ForPrint[table][keys[i]][k] == "DELETE")
            {
                for (int j = 0; j < keys.len; j++)
                {
                    ForPrint[table][keys[i]][j] = "DELETE";
                }
                break;

            }
        }
    }
}
string BD::findConst(int left, int right)
{
    if (left == 3)
    {
        return "lc";
    }
    if (right == 3)
    {
        return "rc";
    }
    return "none";

}

void BD::compare(DL<string>& leftColumn, DL<string>& rightColumn)
{
    int index = 0;
    while(index<leftColumn.len && index < rightColumn.len)
    {
        if (leftColumn[index]!=rightColumn[index])
        {
            leftColumn[index] = "DELETE";
            rightColumn[index] = "DELETE";
        }
        index++;
    }
}

void BD::compareConst(DL<string>& list, string Const)
{
    int index = 0;
    while(index<list.len)
    {
        if (list[index]!=Const)
        {
            list[index] = "DELETE";
        }
        index++;
    }
}

//1-true 2-false 3-const
Pair<int,DL<string>> BD::fillter(print& ForPrint, TreeNode* Where, string table, string column)
{
    string data = Where->data;
    char firstS = data[0];

    if (firstS == -47) // variable
    {
        Pair<string,string> lex = ParsingLexem(data);
        DL<string> res;
        
        if (lex.first == table && lex.second == column)
        {
            res = ForPrint[lex.first][lex.second];
            return {1,res};
        }
        return {2,res};
    }

    if (data[0] == '\'') // const
    {
        data = data.substr(1, data.length() - 2);
        DL<string> noEmt;
        noEmt.LDPUSHT(data);
        return {3, noEmt};
    }

    if (data == "res") // fictivniy element
    {
        return fillter(ForPrint, Where->left, table, column);
    }

    

    if (data == "=")
    {
        Pair<int,DL<string>> left = fillter(ForPrint, Where->left, table, column);
        Pair<int,DL<string>>  right = fillter(ForPrint, Where->right, table, column);
        
        DL<string> leftColumn = left.second;
        DL<string> rightColumn = right.second;

        string ourSide;

        if ((left.first != 1 && right.first != 1))
        {
            DL<string> empt;
            return {2, empt};
        }
        else if (left.first == 1)
        {
            ourSide = "l";
        }
        else if (right.first == 1)
        {
            ourSide = "r";
        }
        else
        {
            DL<string> ept;
            return {2, ept};
        }
        
        string constPresence = findConst(left.first, right.first);

        if (constPresence=="none")
        {
            compare(leftColumn, rightColumn);
        }
        else
        {
            if (constPresence == "lc")
            {
                compareConst(rightColumn, leftColumn[0]);


            }
            else if (constPresence == "rc")
            {
                compareConst(leftColumn, rightColumn[0]);
            }

        }   

        if (ourSide == "l")
        {
            return {1,leftColumn};
        }
        else if(ourSide == "r")
        {
            return {1,rightColumn};
        }

    }

    if (data == "AND")
    {
        Pair<int,DL<string>> left = fillter(ForPrint, Where->left, table, column);
        Pair<int,DL<string>>  right = fillter(ForPrint, Where->right, table, column);

        DL<string> leftColumn = left.second;
        DL<string> rightColumn = right.second;

        if (left.first == 1 && right.first == 1)
        {
            compare(leftColumn,rightColumn); 
            return {1, leftColumn};
        }
        else if (left.first == 1)
        {
            return {1,leftColumn};
        }
        else if (right.first == 1)
        {
            return {1,rightColumn};
        }
        else
        {
            DL<string> ept;
            return {2, ept};
        }
    }



    if (data == "OR")
    {
        Pair<int,DL<string>> left = fillter(ForPrint, Where->left, table, column);
        Pair<int,DL<string>>  right = fillter(ForPrint, Where->right, table, column);

        DL<string> leftColumn = left.second;
        DL<string> rightColumn = right.second;

        if (left.first == 1 && right.first == 1)
        {
            int index = 0;
            while(index<leftColumn.len && index < rightColumn.len)
            {
                if (leftColumn[index]== "DELETE" && rightColumn[index] == "DELETE")
                {
                    leftColumn[index] = "DELETE";
                    rightColumn[index] = "DELETE";
                }
                else if (leftColumn[index]== "DELETE" && rightColumn[index] != "DELETE")
                {
                    leftColumn[index] = rightColumn[index];
                }
                else if (leftColumn[index] != "DELETE" && rightColumn[index] == "DELETE")
                {
                    rightColumn[index] = leftColumn[index];
                }
                index++;
            }
            return {1, leftColumn};
        }
        else 
        {
            if (left.first == 1)
            {
                return {1,leftColumn};
            }
            if (right.first == 1)
            {
                return {1, rightColumn};
            }
            else
            {
                DL<string> emt;
                return {2, emt};
            }
        }


    }
}

void BD::GetAllLexemFromWhere(DL<string>& command, DL<Pair<string,string>>& Lexems, DL<Pair<string,string>>& allLexemFromWhere)
{
    int index = 0;
    while(command[index]!="WHERE") index++;
    index++;

    int lenLexems = Lexems.len;
    int lenCommand = command.len;


    allLexemFromWhere = Lexems;
    for (int i = index; i < lenCommand; i++) // собираем все колонки
    {
        if (command[i][0] == '\'') continue;
        if (command[i][0] == '=') continue;
        if (command[i] == "AND" || command[i] == "OR") continue;

        Pair<string, string> lex = ParsingLexem(command[i]);
        if (allLexemFromWhere.search(lex)) continue;

        allLexemFromWhere.LDPUSHT(lex);
        
    }

}

Tree GetTree(DL<string>& command, DL<Pair<string,string>>& Lexems, DL<Pair<string,string>>& allLexemFromWhere)
{
    int index = 0;
    while(command[index]!="WHERE") index++;
    index++;

    int lenLexems = Lexems.len;
    int lenCommand = command.len;

    DL<Tree> operatorEqual; 
    for (int i = index; i < lenCommand; i++) // собираем все равно
    {
        if (command[i] == "=")
        {
            string leftArg = command[i-1];
            string rightArg = command[i+1];
            Tree miniTree;
            miniTree.TINSERT(command[i]);
            miniTree.TINSERT(leftArg);
            miniTree.TINSERT(rightArg);
            operatorEqual.LDPUSHT(miniTree);
        }
    } 
    Tree miniTree;
    Tree operatorsTree;
    operatorsTree.TINSERT("res");
    int indexOperatorEqual = 0;
    bool AndOr = false;
    for (int i = index; i < lenCommand; i++)
    {
        if (command[i] == "AND" || command[i] == "OR")
        {
            AndOr = true;
            miniTree.TINSERT(command[i]);
            cout << command[i] << '\n';

            operatorEqual[indexOperatorEqual].PRINT(operatorEqual[indexOperatorEqual].root);
            cout << "\n\n\n";

            miniTree.JoinTree(miniTree.root, operatorEqual[indexOperatorEqual++].root);
            cout << "\n\n\n";
            miniTree.PRINT(miniTree.root);
            cout << "\n\n\n";
            operatorEqual[indexOperatorEqual].PRINT(operatorEqual[indexOperatorEqual].root);
            cout << "\n\n\n";

            miniTree.JoinTree(miniTree.root, operatorEqual[indexOperatorEqual++].root);
            cout << "\n\n\n";
            
            miniTree.PRINT(miniTree.root);
            cout << "\n\n\n";

            operatorEqual.LDPUSHT(miniTree);
            if (!operatorsTree.root->left)
            {
                operatorsTree.JoinTree(operatorsTree.root, miniTree.root);
               
            }
            else
            {
                operatorsTree = miniTree;
            }




            operatorsTree.PRINT(operatorsTree.root);
            cout << "\n\n\n";
            miniTree.Allocation(miniTree.root);
            //int a=0;
        }
    }
    if(!AndOr)
    {
        operatorsTree.JoinTree(operatorsTree.root, operatorEqual[0].root);
    }
    return operatorsTree;
}
                                     // Lexems это из SELECT
Tree BD::WHERE(DL<string>& command, DL<Pair<string,string>>& Lexems, DL<Pair<string,string>>& allLexemFromWhere) // здесь мы получим дополнительные колонки и дерево операторов
{
    GetAllLexemFromWhere(command, Lexems, allLexemFromWhere);
    return GetTree(command, Lexems,allLexemFromWhere);  
}

void BD::CreateStructure(DL<Pair<string,string>> allLexemFromWhere, print& ForPrint)
{
    int lenAllFromWhere = allLexemFromWhere.len;
    dict<DL<string>> column;
    for (int i = 0; i < lenAllFromWhere; i++) // создаем структуру
    {
        string nameOneTable = allLexemFromWhere[i].first;
        string nameOneColumn = allLexemFromWhere[i].second;

       
        DL<string> col;

        column.HSET(nameOneColumn,col);

        ForPrint.HSET(nameOneTable,column);
    }
}

void BD::readFile(DL<Pair<string,string>> allLexemFromWhere, print& ForPrint)
{
    int lenAllFromWhere = allLexemFromWhere.len;
    string numLastFile = "1";
    for (int i = 0; i < lenAllFromWhere; i++)
    {
        string nameOneTable = allLexemFromWhere[i].first;
        string nameOneColumn = allLexemFromWhere[i].second;

        char lastSym = nameOneColumn[nameOneColumn.size()-1];

        int numberOfColumn = lastSym - 48; // <10
        string numberlastFileForOnetable = GetNumberOfLastFile(nameOneTable);

        while(true)
        {
            DL<string> col;
        
            string path  = structure[nameOneTable].path + "/" + numLastFile + ".csv";
            readOneFile(path, col, nameOneTable, numberOfColumn);


            ForPrint[nameOneTable][nameOneColumn].add(col);
        
            if (numLastFile==numberlastFileForOnetable)
            {
                numLastFile = "1";
                break;
            }
            PlusOne(numLastFile);
        }   
    }
}


void BD::readOneFile(string path, DL<string>& oneColumn, string tableName, int numberOfColumn)
{
    ifstream readCSV(path);
    string data;
    while(!readCSV.eof())
    {
        getline(readCSV,data);
        string buf;
        int countZapataya = 0;
        for(int i=0;i<data.size();i++)
        {
            if (data[i]==',' || i+1 == data.size()) // data[i] == '\n'
            {
                countZapataya++;
                if (countZapataya == numberOfColumn)
                {
                    oneColumn.LDPUSHT(buf);
                    break;
                }
                buf.clear();
                continue;
            }
            buf+=data[i];
        }
        buf.clear();
        data.clear();
    }
    readCSV.close();
}





DL<string> GetTablesFromDelete(DL<string>& command)
{
    DL<string>  res;
    for (int i = 2;i < command.len;i++)
    {
        if (command[i] == "WHERE") break;
        string table = command[i];
        if (table[table.size()-1] == ',' || table[table.size()-1] == '\\')
        {
            table = table.substr(0,table.size()-2);
            
        }
        res.LDPUSHT(table);
    }
    return res;

}


string BD::DELETE(DL<string>& command)
{
    DL<string> tables = GetTablesFromDelete(command);

    for (int i = 0;i < tables.len; i++)
    {
    if (!rqstForIntervention(tables[i])) return "forbidden";
    }
    for (int i = 0; i < tables.len; i++)
    {
        ForbidIntervention(tables[i]);
    }

    bool WherePresence = false;
    DL<Pair<string,string>> Lexems;
    for (int i = 2; i < command.len; i++)
    {
        if (command[i] == "WHERE")
        {
            WherePresence = true;
            continue;
        }
        if (WherePresence)
        {
            if (command[i][0] == -47)
            {
                Pair<string, string> lex = ParsingLexem(command[i]);
                Lexems.LDPUSHT(lex);
            }
        }

    }

    DL<Pair<string,string>> allLexemFromWhere;
    Tree Filtter;
    Filtter = GetTree(command, Lexems, allLexemFromWhere); 
    
    for (int i = 0;i < tables.len; i++)
    {
        string oneTable = tables[i];
        int countColumns = this->structure[oneTable].columns.len;
        for (int j = 0;j < countColumns; j++)
        {
            string columnName = this->structure[oneTable].columns[j];
            allLexemFromWhere.LDPUSHT({oneTable, columnName});
        }
    }
    
    print ForPrint;
    CreateStructure(allLexemFromWhere, ForPrint);

    readFile(allLexemFromWhere, ForPrint);

    for (int i = 0; i < Lexems.len;i++)
    {
        string table = Lexems[i].first;
        string col = Lexems[i].second;
        if (WherePresence)
        {
            DL<string>  oneColumn = fillter(ForPrint, Filtter.root,table, col).second;
            if (oneColumn.head!=oneColumn.tail) // there is not conditions
            {
                for (int j = 0; j < oneColumn.len; j++)
                {
                    if (oneColumn[j] != "DELETE")
                    {
                        ForPrint[table][col][j] = "DELETE";
                    }
                }
               
            }
        }
    }
    return "0";
    for (int i = 0; i < tables.len; i++)
    {
        WriteInFile(ForPrint, tables[i]);
    }

    for (int i = 0; i < tables.len; i++)
    {
        allowIntervention(tables[i]);
    }
}

void BD::WriteInFile(print& ForWrite, string table)
{
    string numberOfFile = "1";
    
    string maxNumberOfFile = GetNumberOfLastFile(table);

    if (numberOfFile == maxNumberOfFile) 
    {
        string path = this->structure[table].path + "/" + numberOfFile + ".csv";
        int countRow = ForWrite[table][structure[table].columns[0]].len;
        WriteOneFile(ForWrite, table, path, countRow);
        PlusOne(numberOfFile);
        return;
    }
    while(numberOfFile != maxNumberOfFile)
    {
        string path = this->structure[table].path + "/" + numberOfFile + ".csv";
        int countRow = ForWrite[table][structure[table].columns[0]].len;
        WriteOneFile(ForWrite, table, path, countRow);
        PlusOne(numberOfFile);
    }

}

void BD::WriteOneFile(print& ForWrite, string table, string path, int countRow)
{
    string newFile = "";
    for (int i = 0;i < countRow; i++)
    {  
        int countColmn = this->structure[table].columns.len;
        for (int j = 0; j < countColmn; j++)
        {
            string nameColumn = this->structure[table].columns[j];
            if (j+1==countColmn)
            {
                newFile += ForWrite[table][nameColumn][i] + "\n";
            }
            else
            {
                newFile += ForWrite[table][nameColumn][i] + ",";
            }
        } 
    }
    ofstream WriteCSV(path);
    WriteCSV << newFile;
    WriteCSV.close();
}