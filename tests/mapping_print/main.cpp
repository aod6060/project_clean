/*
    This is basically a simple script to take a list of strings and converts it to this format

    {"[%s]", [%s]},

    I need it because I'm freaking lazy and don't want to type in a tone of repedative code.
*/
#include <iostream>
#include <fstream>


int main() {
    std::ifstream in("keyboard.txt");
    std::ofstream out("keyboard_out.txt");
    std::string temp;

    while(std::getline(in, temp)) {
        out << "{\""<<temp<<"\", "<<temp<<"}," << std::endl;
    }
    
    in.close();
    out.close();

    return 0;
}