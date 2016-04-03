#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Stack
{
    private:
        string* data;
        int maxSize;
        int top;

    public:
        Stack(int size);

        void push(string line);
        string pop();
        void print();

};

// Creates a new stack of initial size 'initSize'
Stack::Stack(int initSize)
{
    cout << "Creating stack of size " << initSize << endl;
    if (initSize < 1)
    {
        cout << "Stack size must be positive!";
        exit(1);
    }

    maxSize = initSize;
    data = new string[maxSize];
    top = -1;
}

// adds a new entry to the end of the stack
void Stack::push(string line)
{
    if (top == maxSize)
    {
        cout << "Stack is full!" << endl;
        cout << "Top: " << top << " maxSize: " << maxSize << endl;
        exit(1);
    }

    top++;
    data[top] = line;

    // if the stack is almost full, double its size
    if (top >= maxSize - 2)
    {
        string* holdData = data;
        maxSize *= 2;
        data = new string[maxSize];
        for (int i = 0; i <= top; i++)
            data[i] = holdData[i];

        //delete holdData;
    }
}

// pops the last entry off the stack, returning the popped value
string Stack::pop()
{
    if (top == -1)
    {
        cout << "Stack is empty!" << endl;
        exit(1);
    }

    return data[top--];
}

// prints the stack to console, 1 new line per entry
void Stack::print()
{
    if (top == -1)
    {
        cout << "Stack is empty!" << endl;
        return;
    }
    
    for (int i = 0; i <= top; i++)
        cout << data[i] << "\n";
}

// loads a file from 'fileName' into the 'lines' stack
void loadFile(const string& fileName, Stack& lines)
{
    ifstream file;
    file.open(fileName);

    if (!file.is_open())
    {
        cout << "Error: Couldn't open " << fileName << endl;
        exit(1);
    }

    string line;
    while (getline(file, line))
        lines.push(line);

    file.close();
}