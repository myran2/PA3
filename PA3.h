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

        void push(string line, bool allowDupes);
        string pop();
        void print(bool newLine);
        bool empty() { return top != -1; };

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
void Stack::push(string line, bool allowDupes = true)
{
    if (top == maxSize)
    {
        cout << "Stack is full!" << endl;
        cout << "Top: " << top << " maxSize: " << maxSize << endl;
        exit(1);
    }

    if (!allowDupes)
    {
        for (int i = 0; i <= top; i++)
        {
            if (data[i] == line)
                return;
        }
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
void Stack::print(bool newLine)
{
    if (top == -1)
    {
        cout << "Stack is empty!" << endl;
        return;
    }
    
    for (int i = 0; i <= top; i++)
        cout << data[i] << (newLine ? "\n" : " ");
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

bool remove(string& line, const string& r)
{
    cout << "Removing " << r << " from " << line << endl;
    bool found = false;
    while (line.find(r) != string::npos)
    {
        found = true;
        line.erase(line.find(r), r.length());
        cout << line << endl;
    }

    return found;
}

void tokenize(Stack& lines)
{
    const string KEYWORDS[] = { "BEGIN", "END", "FOR" };
    //const string OPERATORS[] = { "+", "*", "-", "/", "++", "=" };
    const string OPERATORS[] = { "+", "*", "-", "/", "=" };
    const string DELIMITERS[] = { ",", ";" };

    Stack keywords = Stack(2);
    Stack operators = Stack(2);
    Stack delimitors = Stack(2);

    while (!lines.empty())
    {
        string line = lines.pop();

        if (line == "BEGIN" || line == "END")
        {
            keywords.push(line, false);
            continue;
        }
        
        // 5 is length of OPERATORS
        for (int i = 0; i < 5; i++)
        {
            if (remove(line, OPERATORS[i]))
                operators.push(OPERATORS[i], false);
        }

        // 2 is length of DELIMITERS
        for (int i = 0; i < 2; i++)
        {
            if (remove(line, DELIMITERS[i]))
                delimitors.push(DELIMITERS[i], false);
        }
    }

    cout << "Keywords: ";
    keywords.print(false);
    cout << endl;

    cout << "Operators: ";
    operators.print(false);
    cout << endl;

    cout << "Delimiters: ";
    delimitors.print(false);
    cout << endl;
}
