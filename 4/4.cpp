#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

int main() {

    vector<string> lines;
    ifstream infile("input.txt");
    for (string buff; (getline(infile, buff));) {
        lines.push_back(buff);
    }

    int x = lines.size();
    int y = lines[0].size();
    // grid is vector representing x*y matrix of input.txt (x-rows, y-columns)
    vector<char> grid(x*y);

    // initializing the matrix
    #pragma omp parallel for
    for (int i = 0; i<x; i++) {
        string s = lines[i];
        for (int j = 0; j<y; j++) {
            grid[y*i+j] = s[j];
        }
    }

    // position vectors, corners excluded from others
    set<int> CORNERS = {0, y-1,x*y-y,x*y-1};
    set<int> TOPS;
    for (int i = 1; i<y-1; i++) {TOPS.insert(i);}
    set<int> BOTTOMS;
    for (int i = x*y-y+1; i < x*y-1; i++) {BOTTOMS.insert(i);}
    set<int> LEFTS; 
    for (int i = 1; i<x-1; i++) {LEFTS.insert(i*y);}
    set<int> RIGHTS; 
    for (int i = 0; i<x-1; i++) {RIGHTS.insert((i+1)*y-1);}

    // creating adjacents-map
    map<int, vector<int>> neighbors;
    neighbors[0] = {1,y,y+1}; neighbors[y-1] = {y-2,y*2-2,y*2-1};
    neighbors[y*(x-1)] = {y*(x-2),y*(x-2)+1, y*(x-1)+1};
    neighbors[y*x-1] = {y*(x-1)-2, y*(x-1)-1, y*x-2};

    for (int pos : TOPS) {
        neighbors[pos] = {pos-1, pos+1, pos+y-1,pos+y,pos+y+1};
    }
    for (int pos : BOTTOMS) {
        neighbors[pos] = {pos-1, pos+1, pos-y-1, pos-y, pos-y+1};
    }
    for (int pos : LEFTS) {
        neighbors[pos] = {pos-y, pos-y+1, pos+1, pos+y, pos+y+1};
    }
    for (int pos : RIGHTS) {
        neighbors[pos] = {pos-y, pos-y-1, pos-1, pos+y-1, pos+y};
    }
    // all OTHERS have 8 neighbors


    // Proceed to check all elements in the matrix, namely iterate through the neighbors of all elements
    int sum = 0;
    #pragma omp parallel for nowait
    for (int pos = 0; pos < x*y; pos++) {
        if (grid[pos] == '.') {
            // skip empty spots
        } else { // else its '@' paper roll
            if (CORNERS.contains(pos)) {sum+=1;} // CORNERS are accessible always since only 3 neighbors
            else if (TOPS.contains(pos) || BOTTOMS.contains(pos) || LEFTS.contains(pos) || RIGHTS.contains(pos)) {
                int c = 0;
                for (int& ij : neighbors[pos]) {
                    if (grid[ij] == '@') {c++;}
                }
                if (c < 4) {sum++;}
            } else { // else check all 8 adjacent neighbors
                vector<int> adjs = {pos-1, pos+1, pos-y-1, pos-y, pos-y+1, pos+y-1, pos+y, pos+y+1};
                int c = 0;
                for (int& ij : adjs) {
                    if (grid[ij] == '@') {c++;}
                }
                if (c < 4) {sum++;}
            }
        }
    }


    cout << sum << "\n";

    return 0;
}

