#include <charconv>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;

int main() {

    vector<string> banks;
    ifstream banksfile("input.txt");
    for (string buff; (getline(banksfile, buff));) {
        banks.push_back(buff);
    }

    long sum = 0;

    int len = banks[0].size();

    #pragma omp parallel for
    for (int i = 0; i < banks.size(); i++) {
        //cout << i << "\n";
        string s = banks[i];
        int first = 0;
        int second = 0;        
        // check values starting from 9
        for (int ci = 9; ci > 0; ci--) {
            if (first != 0 && second != 0) {break;}
            
            //cout << c << "\n";

            while (s.find_first_of(to_string(ci)) != string::npos) {
                int pos = s.find_first_of(to_string(ci));
                if (pos+1 != len && first == 0) {
                    first = ci;
                    s.erase(pos,1);
                } else if (pos+1 == len && second == 0) {
                    second = ci;
                    s.erase(pos,1);
                } else if (first != 0) {
                    second = ci;
                    s.erase(pos,1);
                } else {
                    break;
                }
            }
        }
        sum += second;
        sum += 10*first;
    }

    cout << sum << "\n";

    return 0;
}

