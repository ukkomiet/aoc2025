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

    vector<vector<long long>> new_ranges; // 0: start, 1: end,

    // preprocess the ranges to check for overlaps, move processed ranges to 'new_ranges'
    while (id_ranges.size() > 0) {
        string r = id_ranges[0];
        long long start_1 = stoll(r.substr(0,r.find('-')));
        long long end_1 = stoll(r.substr(r.find('-')+1));

        long long new_start = 0;
        long long new_end = 0;

        bool merged_with_any = false; // found overlapping range for this range r

        for (int i = 1; i < id_ranges.size(); i++) {
            string r2 = id_ranges[i];
            long long start_2 = stoll(r2.substr(0,r2.find('-')));
            long long end_2 = stoll(r2.substr(r2.find('-')+1));

            if (start_1 >= start_2 && end_1 >= end_2 && end_2 >= start_1) { // falls within (start_2 - (start_1 - end_2) - end_1 )
                new_start = start_2;
                new_end = end_1;
                merged_with_any = true;
            } else if (start_1 <= start_2 && end_1 <= end_2 && end_1 >= start_2) { // falls within (start_1 - (start_2 - end_1) - end_2)
                new_start = start_1;
                new_end = end_2;
                merged_with_any = true;
            } else if (start_1 <= start_2 && end_1 >= end_2) { // falls within (start_1 - (start_2 - end_2) - end_1)
                new_start = start_1;
                new_end = end_1;
                merged_with_any = true;
            } else if (start_2 <= start_1 && end_2 >= end_1) { // falls within (start_2 - (start_1 - end_1) - end_2)
                new_start = start_2;
                new_end = end_2;
                merged_with_any = true;
            } else { // doesn't merge with this range, so we just continue the for-loop
                // continue
            }

            if (merged_with_any) { // check if found an overlapping range
                id_ranges.erase(id_ranges.begin() + i); // erase the one that was merged from the vector of ranges
                break;                                  // and break the for-loop since for now we have finished checking with this range
            }
        }

        if (!merged_with_any) { // if no merging, then insert the vector (or new vector from previous iterations) into the new range vector
            vector<long long> new_one = {start_1, end_1}; 
            new_ranges.push_back(new_one);
            id_ranges.erase(id_ranges.begin()); // erase from the range vector that is being iterated
        } else {
            id_ranges.push_back(to_string(new_start) + "-" + to_string(new_end)); // insert the new merged range to the vector
            id_ranges.erase(id_ranges.begin()); // erase old one from from the range vector that is being iterated
        }
    }


    long long fresh_ids = 0;

    for (vector<long long> range : new_ranges) {
        fresh_ids += (range[1] - range[0] + 1);
    }

    cout << fresh_ids << "\n";
    return 0;
}