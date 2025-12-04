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


    #pragma omp parallel for
    for (int i = 0; i < banks.size(); i++) {

        int len = banks[i].size();
        //cout << i << "\n";
        string s = banks[i];
        int first = 0;
        int second = 0;        
        // check values starting from 9
        int first_p = 0;
        for (int c = 9; c > 0; c--) {
            if (second != 0 && first != 0) {break;}
            
            size_t t = s.find_first_of(to_string(c),first_p+1);
            while (t != string::npos) {


                if (t+1!=len && first == 0) {
                    first = c;
                    first_p = t;
                } else {
                    second = c;
                }
                t = s.find_first_of(to_string(c),t+1);
            }
            
        }
        cout << s << " - " << 10*first + second << "\n";
        sum += second;
        sum += 10*first;
    }

    cout << sum << "\n";

    return 0;
}

