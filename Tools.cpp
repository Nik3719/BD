#include"Tools.h"


DL<string> splitString(const string& str)
{
    DL<string> result;

    string word;

    for (char sym : str)
    {
        if (sym == ' ')
        {
            if (!word.empty())
            {
                result.LDPUSHT(word);
            }
            word = "";
        }
        else
        {
            word += sym;
        }
    }
    if (!word.empty())
    {
        result.LDPUSHT(word);
    }
    return result;
}

string input()
{
    string command;
    cout << "<<";
    char oldSym;
    while (true)
    {
        char sym = getchar();
        if (sym=='\n' || sym == '\\')
        {
            if (sym == '\n' && oldSym != '\\')
            {
                break;
            }
            oldSym = sym;
            command+=' ';
            continue;
        }
        oldSym = sym;
        command += sym;
    }  
    return command;
}

string PlusOne(string& num)
{
    int len = num.size();
    int countLoop = 0;
    for (size_t i = len - 1; i >= 0; i--)
    {
        if (num[i] < '9')
        {
            num = num.substr(0, i) + char(num[i] + 1);
            string countNull = "";
            for (int j = 0; j < countLoop; j++)
            {
                countNull += '0';
            }
            num = num + countNull;
            return num;
        }
        countLoop++;
    }
}