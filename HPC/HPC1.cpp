#include <iostream>
#include <omp.h>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
using namespace std;
using namespace chrono;

class Graph {
    vector<vector<int>> adjMatrix;
    vector<int> visited;
    int n;

public:
    void accept();
    void display();
    void reset();
    void normalDFS(int v);
    void parallelDFS(int v);
    void normalBFS(int v);
    void parallelBFS(int v);
};

void Graph::accept() {
    cout << "\nEnter the number of vertices: ";
    cin >> n;
    adjMatrix.resize(n, vector<int>(n, 0));
    visited.resize(n, 0);
    cout << "\nEnter the adjacency matrix:\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> adjMatrix[i][j];
}

void Graph::display() {
    cout << "\nAdjacency Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << adjMatrix[i][j] << " ";
        cout << endl;
    }
}

void Graph::reset() {
    fill(visited.begin(), visited.end(), 0);
}

void Graph::normalDFS(int v) {
    stack<int> st;
    st.push(v);
    visited[v] = 1;

    while (!st.empty()) {
        int node = st.top();
        st.pop();
        cout << node << " ";

        for (int i = 0; i < n; i++) {
            if (adjMatrix[node][i] == 1 && visited[i] == 0) {
                st.push(i);
                visited[i] = 1;
            }
        }
    }
}

void Graph::parallelDFS(int v) {
    stack<int> st;
    st.push(v);
    visited[v] = 1;

    while (!st.empty()) {
        int node = st.top();
        st.pop();
        cout << node << " ";

        #pragma omp parallel for
        for (int i = 0; i < n; i++) {  // natural order (0 to n-1)
            if (adjMatrix[node][i] == 1 && visited[i] == 0) {
                #pragma omp critical
                {
                    if (visited[i] == 0) {
                        visited[i] = 1;
                        st.push(i);
                    }
                }
            }
        }
    }
}

void Graph::normalBFS(int v) {
    queue<int> q;
    visited[v] = 1;
    q.push(v);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        for (int i = 0; i < n; i++) {
            if (adjMatrix[node][i] == 1 && visited[i] == 0) {
                visited[i] = 1;
                q.push(i);
            }
        }
    }
}

void Graph::parallelBFS(int v) {
    queue<int> q;
    visited[v] = 1;
    q.push(v);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            if (adjMatrix[node][i] == 1 && visited[i] == 0) {
                #pragma omp critical
                {
                    if (visited[i] == 0) {
                        visited[i] = 1;
                        q.push(i);
                    }
                }
            }
        }
    }
}

int main() {
    Graph g;
    int choice;
    char cont;

    do {
        cout << "\nMenu\n1: Accept & Display\n2: DFS Comparison\n3: BFS Comparison\nEnter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            g.accept();
            g.display();
            break;
        case 2: {
            g.reset();
            cout << "\nNormal DFS from vertex 0:\n";
            auto start1 = high_resolution_clock::now();
            g.normalDFS(0);
            auto end1 = high_resolution_clock::now();
            duration<double> t1 = end1 - start1;

            g.reset();
            cout << "\n\nParallel DFS from vertex 0:\n";
            auto start2 = high_resolution_clock::now();
            g.parallelDFS(0);
            auto end2 = high_resolution_clock::now();
            duration<double> t2 = end2 - start2;

            cout << "\n\nTime Normal DFS: " << t1.count() << " sec";
            cout << "\nTime Parallel DFS: " << t2.count() << " sec\n";
            break;
        }
        case 3: {
            g.reset();
            cout << "\nNormal BFS from vertex 0:\n";
            auto start1 = high_resolution_clock::now();
            g.normalBFS(0);
            auto end1 = high_resolution_clock::now();
            duration<double> t1 = end1 - start1;

            g.reset();
            cout << "\n\nParallel BFS from vertex 0:\n";
            auto start2 = high_resolution_clock::now();
            g.parallelBFS(0);
            auto end2 = high_resolution_clock::now();
            duration<double> t2 = end2 - start2;

            cout << "\n\nTime Normal BFS: " << t1.count() << " sec";
            cout << "\nTime Parallel BFS: " << t2.count() << " sec\n";
            break;
        }
        default:
            cout << "Invalid choice!";
        }

        cout << "\nContinue? (y/n): ";
        cin >> cont;
    } while (cont == 'y' || cont == 'Y');

    return 0;
}
