#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

int main() {

    vector<string> id_ranges;
    vector<long long> ids;
    ifstream idfile("input.txt");

    for (string buff; (getline(idfile, buff));) {
        if (buff == "") {
            break;
        } else {
            id_ranges.push_back(buff);
        }
    }

    int fresh = 0;

    // check the fresh ids
    #pragma omp parallel for
    for (string buff; (getline(idfile, buff));) {
        long long fresh_id = stoll(buff);
        for (string range : id_ranges) {
            long long eka = stoll(range.substr(0,range.find('-')));
            long long toka = stoll(range.substr(range.find('-')+1));
            if (fresh_id <= toka && fresh_id >= eka) {
                fresh++;
                break;
            }
        }

    }


    cout << fresh << "\n";
    return 0;
}