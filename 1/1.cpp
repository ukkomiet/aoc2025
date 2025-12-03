#include <iostream>
#include <fstream>
#include <string>

int main() {

    std::ifstream rotations_file("rotations.txt");
    int value = 50;
    int c = 0;

    for (std::string buff; std::getline(rotations_file, buff);) {
        std::string rotation = buff.substr(1);
        int rot = std::stoi(rotation);
        while (rot >= 100) {
            rot -= 100;
            c += 1;
        }
        //std::cout << buff[0] << rot << "\n";
        switch (buff[0]) {
            case 'L':
                if (rot == 0) {break;}
                else if (value == 0) {
                    value -= rot;
                    value += 100;
                } else if (value < rot) {
                    c += 1;
                    value -= rot;
                    value += 100;
                } else if (value == rot) {
                    c += 1;
                    value = 0;
                } else {
                    value -= rot;
                }
                break;
            case 'R':
                if (rot == 0) {break;}
                else if (value + rot >= 100) {
                    value += rot;
                    value -= 100;
                    c += 1;
                } else {
                    value += rot;
                }
                break;
            default:
                break;
        }
    }

    rotations_file.close(); 

    std::cout << c << "\n";
    return 0;
}