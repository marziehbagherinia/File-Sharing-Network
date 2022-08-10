#include "Tools.h"

bool Tools::doesContainDot(string line)
{
    size_t found = line.find(DOT);
    return (found != string::npos);
}

void Tools::splitCommand(string command, int &id, string &start_date, string &end_date)
{
    string field;
    stringstream ss(command);
    vector<string> output;

    while(getline(ss, field, SPACE))
    {
        output.push_back(field);
    }

    if (output.size() >= 4)
    {
        id = stoi(output[1]);
        start_date = output[2];
        end_date = output[3];
    }
}

int Tools::findMaximum(vector <int> input)
{
    int result = -1;

    if (input.size() == 0)
    {
        return -1;
    }

    for (int i = 0; i < input.size(); ++i)
    {
        if (input[i] > result)
        {
            result = input[i];
        }
    }

    return result;
}

int Tools::findMinimum(vector <int> input)
{
    if (input.size() == 0)
    {
        return -1;
    }

    int result = input[0];

    for (int i = 0; i < input.size(); ++i)
    {
        if (input[i] < result)
        {
            result = input[i];
        }
    }

    return result;
}

string Tools::getNamedFifoName(string processName)
{
    string result = FIFO_FILE_PATH;
    result += UNDERLINE;
    result += processName;
    
    return result;
}

string Tools::vectorToString(const vector<int> &vec)
{
    string result = EMPTY_STRING;

    for(int i = 0; i < vec.size(); i++)
    {
        result += (vec[i] + '0');
        result += SPACE;
    }

    return result;
}

string Tools::removeAllSpaces(string line)
{
    line.erase(remove(line.begin(), line.end(), SPACE), line.end());
    return line;
}

vector <int> Tools::splitSlash(string date)
{
    vector<int> output;
    string field;
    stringstream ss(date); 
    while(getline(ss, field, DIR_SEPERATOR))
    {
        output.push_back(stoi(field));
    }

    return output;
}

vector <string> Tools::splitSpace(string date)
{
    vector<string> output;
    string field;
    stringstream ss(date); 
    while(getline(ss, field, SPACE))
    {
        if (field != EMPTY_STRING)
        {
            output.push_back(field);
        }
    }

    return output;
}