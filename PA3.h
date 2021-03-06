#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

template <class T>
class Stack
{
    private:
        T* data;
        int maxSize;
        int top;

    public:
        Stack<T>(int size);
        Stack<T>() { Stack<T>(5); }

        void push(T line, bool allowDupes = true);
        T pop();
        void print(bool newLine);
        bool empty() { return top == -1; };

        // only needed for stacks of stacks (tokens stack required by analyze function)
        friend bool operator==(const Stack<T>& L, const Stack<T>& R)
        {
            if (L.top != R.top)
                return false;

            for (int i = 0; i < L.top; i++)
            {
                if (L.data[i] != R.data[i])
                    return false;
            }

            return true;
        }
        friend bool operator!=(const Stack<T>& L, const Stack<T>& R) { return !(L == R); }

};

// Creates a new stack of initial size 'initSize'
template <class T>
Stack<T>::Stack(int initSize)
{
    if (initSize < 1)
    {
        cout << "Stack size must be positive!";
        exit(1);
    }

    maxSize = initSize;
    data = new T[maxSize];
    top = -1;
}

// adds a new entry to the end of the stack
template <class T>
void Stack<T>::push(T line, bool allowDupes)
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
        T* holdData = data;
        maxSize *= 2;
        data = new T[maxSize];
        for (int i = 0; i <= top; i++)
            data[i] = holdData[i];
    }
}

// pops the last entry off the stack, returning the popped value
template <class T>
T Stack<T>::pop()
{
    if (top == -1)
    {
        cout << "Stack is empty!" << endl;
        exit(1);
    }

    return data[top--];
}

// prints the stack to console, 1 new line per entry
template <class T>
void Stack<T>::print(bool newLine)
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
void loadFile(const string& fileName, Stack<string>& lines)
{
    ifstream file;
    file.open(fileName.c_str());

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

// trims spaces at the beginning of 'line'
string trim(string& line)
{
    int len = line.length();
    for (int i = 0; i < len; i++)
    {
        if (line[i] != ' ')
            break;

        line.erase(i, 1);
        len--;
        i--;
    }

    return line;
}

// removes 'r' from 'line'. returns true if the removal was successful
bool remove(string& line, const string& r)
{
    bool found = false;
    while (line.find(r) != string::npos)
    {
        found = true;
        line.erase(line.find(r), r.length());
    }

    return found;
}

bool isDigit(char c)
{
    switch (c)
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return true;
    }

    return false;
}

// extracts AND REMOVES constants from 'line'
Stack<string> extractConstants(string& line)
{
    Stack<string> constants(5);

    for (unsigned int i = 0; i < line.length(); i++)
    {
        string constant = "";
        while (isDigit(line[i]))
        {
            constant += line[i];
            i++;
        }
        if (constant.length())
        {
            remove(line, constant);
            constants.push(constant);
        }
    }

    return constants;
}

// extracts AND REMOVES variable from 'line'
string extractVariable(string& line)
{
    // if there's no '=' or ',' in the line, it can't contain a variable declaration
    if (line.find("=") == string::npos && line.find(",") == string::npos)
        return "";
    
    string variable = "";
    string newLine = "";

    bool record = true;

    for (unsigned int i = 0; i < line.length(); i++)
    {   
        if (line[i] == ';' || line[i] == '=' || line[i] == ',')
            record = false;
        
        if (islower(line[i]) && record)
        {
            variable += line[i];
            continue;
        }

        if (!islower(line[i]))
            newLine += line[i];
    }

    remove(newLine, variable);
    line = newLine;
    return variable;
}

Stack< Stack<string> > tokenize(Stack<string>& lines)
{
    const string KEYWORDS[] = { "BEGIN", "END", "FOR" };
    const string OPERATORS[] = { "++", "--", "+", "*", "-", "/", "=" };
    const string DELIMITERS[] = { ",", ";" };

    Stack<string> keywords = Stack<string>(2);
    Stack<string> operators = Stack<string>(2);
    Stack<string> delimitors = Stack<string>(2);
    Stack<string> variables = Stack<string>(2);
    Stack<string> constants = Stack<string>(2);
    Stack<string> unknown = Stack<string>(2);

    Stack< Stack<string> > tokens = Stack< Stack<string> >(5);

    while (!lines.empty())
    {
        string line = lines.pop();

        // 3 is the length of KEYWORDS
        for (int i = 0; i < 3; i++)
        {
            if (remove(line, KEYWORDS[i]))
                // we want duplicates of keywords so we can compute loop depth later
                keywords.push(KEYWORDS[i], true);
        }

        // remove leading spaces
        trim(line);

        // trim tabs (for indentation)
        remove(line, "\t");

        // apparently these don't count as delimiters, so we'll remove them
        remove(line, "(");
        remove(line, ")");

        // extract all constants from the line, then remove them from the line
        Stack<string> constantsTemp = extractConstants(line);
        while (!constantsTemp.empty())
        {
            string constant = constantsTemp.pop();
            constants.push(constant, false);
        }

        string var = extractVariable(line);
        if (var != "")
			variables.push(var, false);

        // 6 is the length of OPERATORS
        for (int i = 0; i < 7; i++)
        {
            if (remove(line, OPERATORS[i]))
                operators.push(OPERATORS[i], false);
        }

        // 2 is the length of DELIMITERS
        for (int i = 0; i < 2; i++)
        {
            if (remove(line, DELIMITERS[i]))
                delimitors.push(DELIMITERS[i], false);
        }

        remove(line, " ");

        // Put all the unknown stuff into its own stack
        if (line != "")
            unknown.push(line);
    }

    tokens.push(unknown);
    tokens.push(keywords);
    tokens.push(operators);
    tokens.push(delimitors);
    tokens.push(constants);
    tokens.push(variables);

    return tokens;
}

int calcMaxLoopDepth(Stack<string> keywords)
{
    int curDepth = 0;
    int maxDepth = 0;

    while (!keywords.empty())
    {
        string keyword = keywords.pop();

        if (keyword == "FOR")
            curDepth++;

        if (keyword == "END")
        {
            if (curDepth > maxDepth)
                maxDepth = curDepth;

            // if we encounter an END before a FOR, curDepth might become negative.
            // this check prevents that
            if (curDepth > 0)
                curDepth--;
        }
    }

    // needed?
    if (curDepth > maxDepth)
        maxDepth = curDepth;

    return maxDepth;
}

Stack<string> makeUnique(Stack<string> stack)
{
    Stack<string> res(5);
    while (!stack.empty())
        res.push(stack.pop(), false);

    return res;

}

void analyze(Stack< Stack<string> > tokens)
{
    Stack<string> variables = tokens.pop();
    Stack<string> constants = tokens.pop();
    Stack<string> delimitors = tokens.pop();
    Stack<string> operators = tokens.pop();
    Stack<string> keywords = tokens.pop();
    Stack<string> errors = tokens.pop();

    cout << "OUTPUT> The maximum depth of nested loops is " << calcMaxLoopDepth(keywords) << endl << endl;

    Stack<string> keywordsUnique = makeUnique(keywords);
    std::cout << "Keywords: ";
    keywordsUnique.print(false);
    cout << endl;

    std::cout << "Identifiers: ";
    variables.print(false);
    cout << endl;

    std::cout << "Constants: ";
    constants.print(false);
    cout << endl;

    std::cout << "Operators: ";
    operators.print(false);
    cout << endl;

    std::cout << "Delimiters: ";
    delimitors.print(false);
    cout << endl;

    std::cout << "Syntax Error(s): ";
    if (!errors.empty())
        errors.print(false);
    cout << endl;
}
