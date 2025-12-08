#include <charconv>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;

int find_max_first_index(string str) {
    int max = 0;
    int index = 0;
    for (int i = 0; i < str.size(); i++) {
        int value = str[i] - '0';
        if (value > max) {
            max = value;
            index = i;
        }
    }
    return index;
}

int main() {

    vector<string> banks;
    ifstream banksfile("input.txt");
    for (string buff; (getline(banksfile, buff));) {
        banks.push_back(buff);
    }

    long long sum = 0;

    #pragma omp parallel for
    for (int i = 0; i < banks.size(); i++) {

        string s = banks[i];
        long long temp = 0;

        // checking for the first value, can ignore the last index (in second part can ignore last 11 indices)
        int last_index = find_max_first_index(s.substr(0,s.size()-11));
        vector<char> values;
        values.push_back(s[last_index]);

        cout << last_index << "\n";

        // loop the values
        for (int digit = 0; digit < 11; digit++) {
            int end_buffer = 10-digit;
            int current_index = find_max_first_index(s.substr(last_index+1, s.size()-last_index-1-end_buffer)) + last_index+1;
            last_index = current_index;
            values.push_back(s[current_index]);
        }

        string t = "";
        for (char& c : values) {
            t.push_back(c);
        }

        temp = stoll(t);
        

        //cout << s << " - " << temp << "\n";
        sum += temp;
    }

    cout << sum << "\n";

    return 0;
}

