#include <string>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4 || string(argv[2]) != "-o")
        return 1;

    string src = argv[1];
    string out = argv[3];
    string tmp1 = "tmp.ll";
    string tmp2 = "inst.ll";

    string cmd1 = "clang -S -emit-llvm -O0 -Xclang -disable-O0-optnone " + src + " -o " + tmp1;
    system(cmd1.c_str());

    string cmd2 = "./lab1 " + tmp1 + " " + tmp2;
    system(cmd2.c_str());

    string cmd3 = "clang " + tmp2 + " -o " + out;
    system(cmd3.c_str());

    string cmd4 = "rm -f " + tmp1 + " " + tmp2;
    system(cmd4.c_str());

    return 0;
}
EOF