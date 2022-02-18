#include <bits/stdc++.h>

using namespace std;

class BasicGraspan {
public:
    unordered_map<int, unordered_map<char, unordered_set<int>>> outputMap;
    unordered_set<int> vertices;
    unordered_set<int> visitedVertices;
    vector<vector<pair<int, char>>> adjlist;

    void printGraph(vector<vector<pair<int, char>>> adj);

    void printMap(unordered_map<int, unordered_map<char, unordered_set<int>>> Map);

    void printUnorderedSet(unordered_set<int> V);

    void basicGraspan(int s, vector<vector<pair<int, char>>> adj,
                      unordered_map<int, unordered_map<char, vector<int>>> inMap);

    unordered_map<int, unordered_map<char, vector<int>>> buildInEdges(vector<vector<pair<int, char>>> adjlist);
};

void BasicGraspan::printGraph(vector<vector<pair<int, char>>> adj) {
    int v, V;
    char w;
    V = vertices.size();
    for (int u = 0; u < V; u++) {
        for (auto it = adj[u].begin(); it != adj[u].end(); it++) {
            v = it->first;
            w = it->second;
            cout << u << "-" << v << " : " << w << endl;
        }
    }
    cout << "\n";
}

void BasicGraspan::printMap(unordered_map<int, unordered_map<char, unordered_set<int>>> Map) {
    cout << "\nOutput is in the format: \n<Source Vertex>\n-------\n<label>: [destination vertex]\n\n";
    auto itr = Map.begin();
    while (itr != Map.end()) {
        cout << itr->first << "\n-------\n";

        auto itr2 = itr->second.begin();
        while (itr2 != itr->second.end()) {
            cout << itr2->first << ": ";
            for (int e: itr2->second)
                cout << e << " ";

            cout << endl;
            itr2++;
        }
        cout << endl;
        itr++;
    }
}

void BasicGraspan::printUnorderedSet(unordered_set<int> V) {
    unordered_set<int>::iterator itr;
    for (itr = V.begin(); itr != V.end(); itr++)
        cout << (*itr) << " ";
}

/** store incoming edges of every vertex in V
 * example for  0 1 A - it is stored as <1, <A, [0,4]>>
 **/
unordered_map<int, unordered_map<char, vector<int>>>
BasicGraspan::buildInEdges(vector<vector<pair<int, char>>> adjlist) {
    int V = vertices.size();
    unordered_map<int, unordered_map<char, vector<int>>> Map;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < adjlist[i].size(); j++) {
            Map[adjlist[i][j].first][adjlist[i][j].second].push_back(i);
        }
    }
    return Map;
}

/** implementation of basic Graspan where
 * if we consider two adjacent edges in a graph labeled with "A" and "B",
 * and if there is a grammar rule C --> AB,
 * then we add a new edge labeled with "C"
 **/
void BasicGraspan::
basicGraspan(int s, vector<vector<pair<int, char>>> adj,
             unordered_map<int, unordered_map<char, vector<int>>> inMap) {
    int V = vertices.size();

    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Create a queue
    list<int> queue;

    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);

    while (!queue.empty()) {
        // pop first vertex v1 from q
        s = queue.front();
        visitedVertices.insert(s);
        queue.pop_front();

        auto itr = adj[s].begin();
        while (itr != adj[s].end()) {
            if (itr->second == 'B') {
                for (auto e: inMap[s]['A']) {
                    outputMap[e]['C'].insert(itr->first);
                }
            }
            if (!visited[itr->first]) {
                visited[itr->first] = true;
                queue.push_back(itr->first);
            }
            itr++;
        }
    }
}

int main() {
    BasicGraspan bg;

    /*Adjacency list representation of the graph having size V*/
    int V;

    /*READING INPUT FROM FILE*/
    int u, v;
    char label;

    freopen("C:\\Users\\USER\\CLionProjects\\CFL-reachability\\input.txt", "r", stdin);
    cin >> V;
    bg.adjlist.resize(V);
    while ((cin >> u >> v >> label)) {
        bg.vertices.insert(u);
        bg.vertices.insert(v);
        bg.adjlist[u].push_back({v, label});
    }

    /*printGraph(adjlist);*/

    /*PREPROCESSING STEP*/
    unordered_map<int, unordered_map<char, vector<int>>> inMap;
    inMap = bg.buildInEdges(bg.adjlist);

    /*RUN BASIC GRASPAN*/

    for (int i = 0; i < bg.vertices.size(); ++i) {
        if (bg.visitedVertices.find(i) == bg.visitedVertices.end()) {
            cout << "started running from " << i << endl;
            bg.basicGraspan(i, bg.adjlist, inMap);
        }
    }

    bg.printMap(bg.outputMap);

    return 0;
}