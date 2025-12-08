#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <float.h>
#include <set>

using namespace std;


double distance(vector<double> p1, vector<double> p2) {
    return hypot( abs(p1[0]-p2[0]) , abs(p1[1]-p2[1]) , abs(p1[2]-p2[2]) );
}

int main() {

    int connections_max = 1000;

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
    int circuits_max_size = 0; // all points should be in a circuit of size n_points;
    while (circuits_max_size < n_points) {

        double lowest_distance = DBL_MAX;
        int p1, p2;

        #pragma omp parallel for
        for (int i = 0; i < n_points; i++) {
            for (int j = 0; j < n_points; j++) {
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
        vector<int> in_circuits = {};
        for (int c = 0; c < circuits.size(); c++) {
            set<int> circuit = circuits[c];
            if (circuit.contains(p1) || circuit.contains(p2)) {
                in_circuits.push_back(c);
            }
        }
        if (in_circuits.size() == 2) { // gotta combine the circuits

            set<int>circuit_1 = circuits[in_circuits[0]];
            set<int>circuit_2 = circuits[in_circuits[1]];

            circuit_1.insert(circuit_2.begin(), circuit_2.end());

            circuits[in_circuits[0]] = circuit_1;
            circuits.erase(circuits.begin() + in_circuits[1]);


        } else if (in_circuits.size() == 1) { // add the points to the same circuit
            set<int> circuit = circuits[in_circuits[0]];
            circuit.insert(p1);
            circuit.insert(p2);
            circuits[in_circuits[0]] = circuit;
        } else { // make a new circuit if neither is in an existing one
            set<int> new_c = {p1,p2};
            circuits.push_back(new_c);
        }

        // and finally mark in the matrix that this connection was made
        distances[n_points*p1 + p2] = -2;
        distances[n_points*p2 + p1] = -2;

        for (set<int> c : circuits) {
            int size = c.size();
            if (size > circuits_max_size) {
                circuits_max_size = size;
            }
        }

        if (circuits_max_size == n_points) {
            cout << "final points p1_x: " << points[p1][0] << ", p2_x: " << points[p2][0] << "\n";
            cout << "their * = " << points[p1][0] * points[p2][0] << "\n";
        }

        connections++;
        cout << "connections now: " << connections << " circuits_max: " << circuits_max_size << "\n";
    }

    return 0;
}