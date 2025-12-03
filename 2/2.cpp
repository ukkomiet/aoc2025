#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {

    vector<string> id_ranges;
    ifstream idfile("input.txt");
    for (string buff; (getline(idfile, buff, ','));) {
        id_ranges.push_back(buff);
    }

    long long sum = 0;

    // brutus
    #pragma omp parallel for
    for (int i = 0; i<id_ranges.size(); i++) {
        string t = id_ranges[i];
        long long eka = stoll(t.substr(0,t.find('-')));
        long long toka = stoll(t.substr(t.find('-')+1));
        for (eka; eka <= toka; eka++) {
            string s = to_string(eka);
            bool passes = true;
            // check for same int
            if (s.find_first_not_of(s[0]) == string::npos) {
                // all same and passes
            } else { // process substrings
                string s0 = "";
                s0.push_back(s[0]);
                int len = 1;
                int j = 1;
                while (j*len < s.size() && s0 != s) {
                    if (s0 != s.substr(j*len,len)) {
                        s0 = s.substr(0,len+1);
                        passes = false;
                        len = s0.size();
                        j = 1;
                    } else {j++; passes = true;}
                }
            }
            if (passes && s.size() > 1) {
                //cout << s << "\n";
                sum += eka;
            }
        }
    }

    cout << sum << "\n";
    return 0;
}