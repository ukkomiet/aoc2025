#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>

using namespace std;

void reverseString(string& str) {
    int n = str.length();
    for (int i = 0; i < n / 2; ++i) {
        swap(str[i], str[n - i - 1]);
    }
}

int main() {

    vector<string> lines;
    ifstream infile("input.txt");
    for (string buff; (getline(infile, buff));) {
        lines.push_back(buff);
    }

    int operator_line = lines.size() - 1;

    vector<vector<string>> list_of_lines(lines.size());
    
    // modify the string input lines into vectors, and place them in the list_of_lines vector
    #pragma omp parallel for
    for (int i = 0; i < lines.size(); i++) {
        string s = lines[i];
        reverseString(s); // reverse the string, since we read from right to left
        istringstream stream(s);
        string buff;
        while (getline(stream, buff, ' ')) {
            if (i == operator_line) {
                if (buff != "" && buff != " ") { // disregard spaces in the operator line
                    list_of_lines[i].push_back(buff);
                }
            } else {
                if (buff == "" || buff == " ") { // problem is how to differentiate between (in input.txt) the last 9 columns, some start with spaces while some end with them...
                    list_of_lines[i].back().push_back(' ');
                } else {
                    list_of_lines[i].push_back(buff);
                }
            }
        }
    }

    int columns = list_of_lines[0].size();
    int operators = list_of_lines[operator_line].size();
    cout << "columns:" << columns << "\n";
    cout << "nof ops:" << operators << "\n";

    cout << "values (8-10) from row 0:" << list_of_lines[0][8] << "," << list_of_lines[0][9] << "," << list_of_lines[0][10] << "X" << "\n";
    cout << "values (8-10) from row 1:" << list_of_lines[1][8] << "," << list_of_lines[1][9] << "," << list_of_lines[1][10] << "X" << "\n";
    cout << "last 4 ops:" << list_of_lines[operator_line][operators-1][0] << list_of_lines[operator_line][operators-2][0] << list_of_lines[operator_line][operators-3][0] << list_of_lines[operator_line][operators-4][0] <<"\n";

    long long sum = 0;
    long long temp = 0;


    #pragma omp parallel for
    for (int col = 0; col < columns; col++) {
        char op = list_of_lines[operator_line][col][0];

        vector<string> numbers_str_temp;
        int numbers_size = list_of_lines[0][col].size(); // how long each number is digits-wise

        for (int i = 0; i < operator_line; i++) {
            //cout << list_of_lines[i][col] << "-"<< "\n";
            numbers_str_temp.push_back(list_of_lines[i][col]);
        }

        vector<long long> numbers;
        
        for (int spot = 0; spot < numbers_size; spot++) {
            string temp_str = "";
            for (string& n_str : numbers_str_temp) { // for every string, check every spot
                if (isdigit(n_str[spot])) {
                    temp_str.push_back(n_str[spot]);
                }
            }
            cout << "---" << stoll(temp_str) << "---" << "\n"; 
            long long temp_n = stoll(temp_str);
            numbers.push_back(temp_n);
        }

        cout << "nubmer size:" << numbers.size() << "first:" << numbers[0] << "\n";


        switch (op) {
            case '+':
                temp = 0;
                for (long long& n : numbers) {
                    cout << "n:" << n << "\n";
                    temp += n;
                }
                sum += temp;
                cout << "temp:" << temp << "\n";
                break;
            case '*':
                temp = 1;
                for (long long& n : numbers) {
                    temp *= n;
                }
                sum += temp;
                break;
            default:
                cout << "what the helly" << "\n";
                break;
        }
    }

    
    cout << sum << "\n";
    return 0;
}