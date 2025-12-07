#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

int main() {

    vector<string> id_ranges;
    vector<string> ids;
    ifstream idfile("input.txt");
    bool id_b = true;
    for (string buff; (getline(idfile, buff));) {
        if (buff == "") {
            id_b = false;
        } else if (id_b) {
            id_ranges.push_back(buff);
        } else {
            ids.push_back(buff);
        }
    }
    cout << id_ranges[0] << " " << ids[0] << "\n";

    int fresh = 0;
    int spoiled = 0;

    set<long long> fresh_ids;

    // brutus for inserting fresh ids into a set
    #pragma omp parallel for
    for (int i = 0; i<id_ranges.size(); i++) {
        string t = id_ranges[i];
        long long eka = stoll(t.substr(0,t.find('-')));
        long long toka = stoll(t.substr(t.find('-')+1));
        for (eka; eka <= toka; eka++) {
            fresh_ids.insert(eka);
        }
    }

    // checking ids from fresh id set
    #pragma omp parallel for 
    for (int i = 0; i<ids.size(); i++) {
        if (fresh_ids.contains(ids[i])) {
            fresh++;
        } else {
            spoiled++;
        }
    }




    cout << fresh << "\n";
    return 0;
}