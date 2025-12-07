#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

int main() {

    vector<string> lines;
    ifstream infile("input.txt");
    for (string buff; (getline(infile, buff));) {
        lines.push_back(buff);
    }

    size_t start_pos = lines[0].find("S");
    //cout << start_pos << "-" << lines[0][start_pos] << "\n";


    int splits = 0;

    set<size_t> laser_positions = {start_pos}; // laser positions in each timeline
    map<size_t, int> pos_count = { {start_pos, 1} }; // how many lasers are at this count

    for (int row = 1; row < lines.size(); row++) {
        cout << "row: " << row << "  laser_positions size: " << laser_positions.size() << "\n";
        string s = lines[row];
        // find all positions of splitters
        size_t pos = s.find("^",0);
        while(pos != string::npos) {
            //cout << pos << ",";
            if (laser_positions.contains(pos)) { // its a hit; splits the laser
                laser_positions.erase(pos);

                laser_positions.insert(pos+1);
                laser_positions.insert(pos-1);
                if (pos_count.contains(pos+1)) { // raise pos_count for one of the new laser pos
                    pos_count[pos+1] = pos_count[pos+1] + pos_count[pos];
                } else {
                    pos_count[pos+1] = pos_count[pos];
                }
                if (pos_count.contains(pos-1)) { // raise pos_count for one of the new laser pos
                    pos_count[pos-1] = pos_count[pos-1] + pos_count[pos];
                } else {
                    pos_count[pos-1] = pos_count[pos];
                }
                pos_count[pos] = 0; // only now clean the pos_count of the laser that hit
            }
            pos = s.find("^",pos+1);
        }
        //cout << "\n";
    }

    // counting the split time
    for (size_t laser_pos : laser_positions) {
        splits += pos_count[laser_pos];
    }

    cout << splits << "\n";

    return 0;
}