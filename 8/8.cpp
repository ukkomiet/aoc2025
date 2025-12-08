#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <float.h>
#include <set>

using namespace std;


double distance(vector<double> p1, vector<double> p2) {
    return hypot( abs(p1[0]-p2[0]) , abs(p1[1]-p2[1]) , abs(p1[2]-p2[2]) );
}

int main() {

    vector<vector<double>> points;

    ifstream infile("input.txt");
    for (string buff; (getline(infile, buff));) {
        int comma_1 = buff.find(",");
        double x = stod(buff.substr(0,comma_1));
        string left = buff.substr(comma_1+1);
        int comma_2 = left.find(",");
        double y = stod(left.substr(0,comma_2));
        double z = stod(left.substr(comma_2+1));

        points.push_back({x,y,z});
        
    }

    int n_points = points.size();

    vector<set<int>> circuits; // collection of circuits, each circuit is a set of ints (indices of points)

    vector<double> distances(n_points*n_points, -1); // matrix for keeping tab of distances dynamically

    cout << distances.size() << "\n";
    // M_ij == M_ji: distance between points[i] and points[j]
    // diagonal M_ii ignored
    // lookup M_ij (ith row, jth column) == distances[n_points*i + j]

    int connections = 0;
    while (connections < 1000) {

        cout << "connections now: " << connections << "\n";

        double lowest_distance = DBL_MAX;
        int p1, p2;

        #pragma omp parallel for
        for (int i = 0; i < n_points; i++) {
            for (int j = 0; j < n_points; i++) {
                if (i != j) {
                    double d = distances[n_points*i + j];
                    if (d != -2) { // check that this distance hasnt already been added
                        if (d == -1) { // gotta compute the distance and update both M_ij & M_ji
                            d = distance(points[i], points[j]);
                            distances[n_points*i + j] = d;
                            distances[n_points*j + i] = d;
                        }
                        if (d < lowest_distance) {
                            p1 = i;
                            p2 = j;
                            lowest_distance = d;
                        }
                    }
                }
            }
        }

        // now that we have the shortest distance between two points
        // we can make the connection and enlargen their circuit
        // first check if either point is already in a circuit
        bool new_circuit = true;
        for (int c = 0; c < circuits.size(); c++) {
            set<int> circuit = circuits[c];
            if (circuit.contains(p1) || circuit.contains(p2)) {
                circuit.insert(p1);
                circuit.insert(p2);
                circuits[c] = circuit;
                new_circuit = false;
                break;
            }
        }
        if (new_circuit) { // make a new circuit if neither is in one
            set<int> new_c = {p1,p2};
            circuits.push_back(new_c);
        }
        // and finally mark in the matrix that this connection was made
        distances[n_points*p1 + p2] = -2;
        distances[n_points*p2 + p1] = -2;

        connections++;
    }

    // take the top 3 largest circuits
    int top1 = 1;
    int top2 = 1;
    int top3 = 1;
    for (set<int> c : circuits) {
        int size = c.size();
        if (size > top1) {
            top3 = top2;
            top2 = top1;
            top1 = size;
        } else if (size > top2) {
            top3 = top2;
            top2 = size;
        } else if (size > top3) {
            top3 = size;
        }
    }

    cout << "top: " << top1 << ", " << top2 << ", " << top3 << "\n";
    cout << top1*top2*top3 << "\n";

    return 0;
}