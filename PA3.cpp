#include "PA3.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << "You must supply a filename argument!" << endl;
        exit(1);
    }
    
    Stack<string> programLines = Stack<string>(5);
    loadFile(argv[1], programLines);

    programLines.print(true);
    Stack< Stack<string> > tokens = tokenize(programLines);
    tokens.pop();
    return 0;
}
