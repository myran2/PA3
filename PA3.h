#include <iostream>
#include <string>

using namespace std;

class Stack
{
    private:
        string* data;
        int maxSize;
        int top;

    public:
        Stack() { Stack(5); }
        Stack(int size);

        void push(string line);
        string pop();
        void print();

};

Stack::Stack(int size)
{
    cout << "Creating stack of size " << size << endl;
    if (size < 1)
    {
        cout << "Stack size must be positive!";
        exit(1);
    }

    maxSize = size;
    data = new string[maxSize];
    top = -1;
}

void Stack::push(string line)
{
    if (top == maxSize)
    {
        cout << "Stack is full!" << endl;
        exit(1);
    }

    top++;
    data[top] = line;
}

string Stack::pop()
{
    if (top == -1)
    {
        cout << "Stack is empty!" << endl;
        exit(1);
    }

    return data[top--];
}

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

