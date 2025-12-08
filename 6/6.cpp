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

string filter_to_digits(string& str) {
    string out = "";
    for (char& c : str) {
	    if (isdigit(c)) out += c;
    }
    return out;
}

int main() {

    vector<string> lines;
    ifstream infile("input.txt");
    for (string buff; (getline(infile, buff));) {
        reverseString(buff);
        lines.push_back(buff);
    }

    int input_length = lines[0].size();

    int operator_line = lines.size() - 1;
    long long sum = 0;
    long long temp = 0;

    string empty_col = "";
    for (int i = 0; i < lines.size(); i++) {
        empty_col += " ";
    }

    int spot = 0;
    char op = 't';
    vector<string> temp_strings;
    string temp_str = "";
    while (spot < input_length) { // iterate the input lines one character(spot) at a time
        for (int row = 0; row < lines.size(); row++) {
            temp_str += lines[row][spot];
        }
        if (temp_str != empty_col) {
            temp_strings.push_back(temp_str);
        }

        if (temp_str.back() == '*' || temp_str.back() == '+') { // operator found which means this column of values is done and to be operated over
            op = temp_str.back();

            switch (op) {
                case '+':
                    temp = 0;
                    for (string& str : temp_strings) {
                        temp += stoll(filter_to_digits(str));
                    }
                    sum += temp;
                    break;
                case '*':
                    temp = 1;
                    for (string& str : temp_strings) {
                        temp *= stoll(filter_to_digits(str));
                    }
                    sum += temp;
                    break;
                default:
                    cout << "what the helly?" << "\n";
                    break;
            }

            // clear the temp stuff
            temp_strings.clear();
            op = 't';
        }
        temp_str = "";
        spot++;
    }
    
    


    
    cout << sum << "\n";
    return 0;
}