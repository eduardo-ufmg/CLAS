#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>  // for std::remove
#include <cstdlib>    // for std::exit

using namespace std;

// A helper function to trim spaces from a string (if needed)
std::string trim(const std::string &s) {
    size_t start = s.find_first_not_of(" \t");
    if(start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
}

int main(int argc, char* argv[])
{
    // Check for proper usage.
    if(argc < 4) {
        cerr << "Usage: " << argv[0] << " <features>.csv <targets>.csv <edges>.csv" << endl;
        return 1;
    }

    string features_file_name_with_path = argv[1];
    string targets_file_name_with_path = argv[2];
    string edges_file_name_with_path = argv[3];

    // -------------------------------
    // 1. Read vertices.csv
    // -------------------------------
    vector< vector<double> > vertices;
    {
        ifstream fin(features_file_name_with_path);
        if (!fin) {
            cerr << "Error: Cannot open vertices file: " << features_file_name_with_path << endl;
            return 1;
        }
        string line;
        while(getline(fin, line)) {
            if(line.empty()) continue;
            vector<double> coords;
            stringstream ss(line);
            string token;
            while(getline(ss, token, ',')) {
                token = trim(token);
                if (!token.empty())
                    coords.push_back(stod(token));
            }
            vertices.push_back(coords);
        }
    }
    size_t numVertices = vertices.size();
    if(numVertices == 0) {
        cerr << "Error: No vertices were read." << endl;
        return 1;
    }

    // -------------------------------
    // 2. Read targets.csv
    // -------------------------------
    vector<int> targets;
    {
        ifstream fin(targets_file_name_with_path);
        if (!fin) {
            cerr << "Error: Cannot open targets file: " << targets_file_name_with_path << endl;
            return 1;
        }
        string line;
        while(getline(fin, line)) {
            line = trim(line);
            if(line.empty()) continue;
            // In this example, targets are integers.
            targets.push_back(stoi(line));
        }
    }
    if(targets.size() != numVertices) {
        cerr << "Error: Number of targets (" << targets.size() 
             << ") does not match number of vertices (" << numVertices << ")." << endl;
        return 1;
    }

    // -------------------------------
    // 3. Read edges.csv
    // -------------------------------
    // Each line is "vertex_a, vertex_b"
    vector< pair<int,int> > edges;
    {
        ifstream fin(edges_file_name_with_path);
        if (!fin) {
            cerr << "Error: Cannot open edges file: " << edges_file_name_with_path << endl;
            return 1;
        }
        string line;
        while(getline(fin, line)) {
            if(line.empty()) continue;
            stringstream ss(line);
            string token;
            int a, b;
            // Get first vertex index
            if(getline(ss, token, ',')) {
                token = trim(token);
                a = stoi(token);
            } else {
                continue;
            }
            // Get second vertex index
            if(getline(ss, token, ',')) {
                token = trim(token);
                b = stoi(token);
            } else {
                continue;
            }
            edges.push_back(make_pair(a, b));
        }
    }

    // -------------------------------
    // 4. Build the graph (adjacency list)
    // -------------------------------
    // Assuming an undirected graph: for each edge (a,b) add both directions.
    vector< vector<int> > adj(numVertices);
    for(auto &e : edges) {
        int a = e.first, b = e.second;
        if(a >= 0 && a < static_cast<int>(numVertices) &&
           b >= 0 && b < static_cast<int>(numVertices))
        {
            adj[a].push_back(b);
            adj[b].push_back(a);
        }
    }

    // -------------------------------
    // 5. Compute q(x_i) for each vertex
    // -------------------------------
    // For a vertex x_i, let:
    //   Gr(x_i) = degree of vertex x_i (i.e. number of neighbors)
    //   Ĝr(x_i) = number of neighbors j with the same class (y_j == y_i)
    // Then, q(x_i) = Ĝr(x_i) / Gr(x_i)
    //
    // (For vertices with degree zero we set q(x_i)=0.)
    vector<double> q(numVertices, 0.0);
    for (size_t i = 0; i < numVertices; ++i) {
        int degree = adj[i].size();
        if(degree == 0) {
            q[i] = 0.0;  // or treat isolated vertices as needed
            continue;
        }
        int sameClassCount = 0;
        for (int nb : adj[i]) {
            if (targets[nb] == targets[i])
                sameClassCount++;
        }
        q[i] = static_cast<double>(sameClassCount) / degree;
    }

    // -------------------------------
    // 6. Compute the per-class threshold t_class
    // -------------------------------
    // For each class, t_class = average of q(x_i) over all vertices in that class.
    map<int, pair<double, int>> classStats; // class -> (sum of q, count)
    for (size_t i = 0; i < numVertices; ++i) {
        int cls = targets[i];
        classStats[cls].first += q[i];
        classStats[cls].second += 1;
    }
    map<int, double> threshold;
    for (auto &kv : classStats) {
        int cls = kv.first;
        double sum_q = kv.second.first;
        int count = kv.second.second;
        threshold[cls] = sum_q / count;
    }

    // -------------------------------
    // 7. Filter vertices based on q(x_i) vs. threshold
    // -------------------------------
    // Remove (mark) vertices for which q(x_i) is strictly less than the threshold for its class.
    vector<bool> keep(numVertices, false);
    for (size_t i = 0; i < numVertices; ++i) {
        int cls = targets[i];
        if (q[i] >= threshold[cls])
            keep[i] = true;
        // else: vertex is filtered (removed)
    }

    // -------------------------------
    // 8. Re-index the remaining vertices
    // -------------------------------
    // The new files should be consistent: only kept vertices appear and are renumbered from 0.
    vector<int> newIndex(numVertices, -1);
    int newCounter = 0;
    for (size_t i = 0; i < numVertices; ++i) {
        if (keep[i]) {
            newIndex[i] = newCounter;
            newCounter++;
        }
    }
    size_t numNewVertices = newCounter;
    cout << "Original vertices: " << numVertices << ", kept vertices: " << numNewVertices << "\n";

    // -------------------------------
    // 9. Write the filtered vertices and targets files
    // -------------------------------

    string features_file_name_without_path = features_file_name_with_path.substr(features_file_name_with_path.find_last_of("/\\") + 1);
    string targets_file_name_without_path = targets_file_name_with_path.substr(targets_file_name_with_path.find_last_of("/\\") + 1);

    string filtered_features_file_name = features_file_name_without_path.substr(0, features_file_name_without_path.find_last_of(".")) + "_filtered.csv";
    string filtered_targets_file_name = targets_file_name_without_path.substr(0, targets_file_name_without_path.find_last_of(".")) + "_filtered.csv";

    string filtered_features_file_name_with_path = "./output/" + filtered_features_file_name;
    string filtered_targets_file_name_with_path = "./output/" + filtered_targets_file_name;

    ofstream foutVertices(filtered_features_file_name_with_path);
    ofstream foutTargets(filtered_targets_file_name_with_path);
    if (!foutVertices || !foutTargets) {
        cerr << "Error: Cannot open output vertices/targets file(s) for writing." << endl;
        return 1;
    }
    // For each kept vertex, write its coordinates and target.
    for (size_t i = 0; i < numVertices; ++i) {
        if (!keep[i])
            continue;
        // Write coordinates as comma–separated values.
        for (size_t j = 0; j < vertices[i].size(); ++j) {
            foutVertices << vertices[i][j];
            if (j < vertices[i].size()-1)
                foutVertices << ", ";
        }
        foutVertices << "\n";
        // Write target.
        foutTargets << targets[i] << "\n";
    }
    foutVertices.close();
    foutTargets.close();

    // -------------------------------
    // 10. Write the filtered edges file
    // -------------------------------
    // Only write an edge if both endpoints are kept; update the vertex indices.

    string edges_file_name_without_path = edges_file_name_with_path.substr(edges_file_name_with_path.find_last_of("/\\") + 1);
    string filtered_edges_file_name = edges_file_name_without_path.substr(0, edges_file_name_without_path.find_last_of(".")) + "_filtered.csv";
    string filtered_edges_file_name_with_path = "./output/" + filtered_edges_file_name;

    ofstream foutEdges(filtered_edges_file_name_with_path);
    if (!foutEdges) {
        cerr << "Error: Cannot open output edges file for writing." << endl;
        return 1;
    }
    for (auto &e : edges) {
        int a = e.first;
        int b = e.second;
        if (keep[a] && keep[b]) {
            foutEdges << newIndex[a] << ", " << newIndex[b] << "\n";
        }
    }
    foutEdges.close();

    cout << "Filtering complete. Files written:\n"
         << "  " << filtered_features_file_name << "\n"
         << "  " << filtered_targets_file_name << "\n"
         << "  " << filtered_edges_file_name << "\n";

    return 0;
}
