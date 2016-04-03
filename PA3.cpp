#include "PA3.h"

int main()
{
    Stack stack(5);

    stack.push("1");
    stack.push("2");
    stack.push("3");
    stack.push("4");
    stack.push("5");
    stack.print();

    stack.pop();
    cout << endl;

    stack.print();
    
    return 0;
}

