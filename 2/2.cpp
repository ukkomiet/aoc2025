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
    cout << id_ranges.size() << "\n";

    /*
    string temp = id_ranges[3];
    int eka = stoi(temp.substr(0,temp.find('-')));
    int toka = stoi(temp.substr(temp.find('-')+1));
    cout << eka << " " << toka << "\n";
    string eka_s = to_string(eka);
    int s = eka_s.size()/2;
    string a = eka_s.substr(0,s);
    string b = eka_s.substr(s);
    cout << "a:" + a << " " << "b: " + b << "\n";
    */
    
    long long sum = 0;

    // brutus
    #pragma omp parallel for
    for (int i = 0; i<id_ranges.size(); i++) {
        string t = id_ranges[i];
        long long eka = stoll(t.substr(0,t.find('-')));
        long long toka = stoll(t.substr(t.find('-')+1));
        for (eka; eka <= toka; eka++) {
            string s = to_string(eka);
            string s0 = "";
            bool passes = true;
            s0.push_back(s[0]);
            int len = 1;
            for (int j=1; j< s.size(); j++) {
                if (s0 != s.substr(len,len)) {
                    s0 = s.substr(0,j);
                    passes = false;
                    len = s0.size();
                }
            }
            if (passes) {
                sum += eka;
            }
        }
    }

    cout << sum << "\n";
    return 0;
}