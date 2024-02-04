#include <iostream>
#include "includes/sha256.hpp"

using std::string;
using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    string input = "coco42";
    string output1 = sha256(input);

    cout << "sha256('"<< input << "'):" << output1 << endl;
    return 0;
}
