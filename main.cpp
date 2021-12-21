#include <bits/stdc++.h>
#include <list>
#include <iostream>

using namespace std;

typedef struct Vector2 {
    float x, y;

    inline Vector2( void ) {}
    inline Vector2( const float _x, const float _y) {
        x = _x;
        y = _y;
    }

    inline Vector2 operator + ( const Vector2& a ) const {
        return Vector2( x + a.x, y + a.y );
    }

    inline Vector2 operator * ( const float a ) const {
        return Vector2(x*a, y*a);
    }

    inline float Distance( const Vector2& a ) const {
        return sqrt(pow(x - a.x, 2) + pow(y - a.y, 2) * 1.0);
    }

    friend ostream& operator<<(ostream& os, const Vector2& a)
    {
        os << '(' << a.x << ", " << a.y << ')';
        return os;
    }
} Vector2;

typedef struct Vertex {
    public:
        Vector2 position;
        int profit;

        Vertex(float x, float y, int p) {
            position = Vector2(x, y);
            profit = p;
        }

        friend ostream& operator<<(ostream& os, const Vertex& a)
        {
            os << a.position << ' ' << a.profit;
            return os;
        }
} Vertex;


class Node {
    public:
        Node* parent;
        vector<Node*> children;
        int level;
        vector<int> domain;
        int value;

        Vertex& vertex;

        Node(Vertex& v): vertex(v) {
            value = 0;
            parent = NULL;
            level = -1;
        }

        Node(Vertex& v, Node& p): vertex(v) {
            parent = &p;
            level = p.level + 1;
            domain = p.domain;
            if (p.value != 0) {
                auto i = find(domain.begin(), domain.end(), p.value);
                if (i != domain.end())
                    domain.erase(i);
                else
                    throw invalid_argument("Parent's value doesn't belong to its domain.");
            }
        }

        int GetFirstNotInstValue() {
            for (int v: domain) {
                auto it = begin(parent->children);
                for (; it != end(parent->children); it++)
                    if ((*it)->value == v)
                        break;
                if (it == end(parent->children))
                    return v;
            }
            return -1;
        }

        float LessThanMaxTime(Vertex& start, Vertex& dest, float tmax) {
            vector<Node*> path;
            // Recorro el grafo hacia arriba hasta alcanzar la raíz, y armo el camino.
            for (Node* curr = this; curr->parent != NULL; curr = curr->parent)
                if (curr->value != 0)
                    path.push_back(curr);

            sort(path.begin(), path.end(), [](Node* n1, Node* n2) -> bool { 
                return (n1->value < n2->value);
            });
            cout << "Path: " << "[i]" << start.position << ' ';
            for (const auto &item: path)
                cout << *item << ' ';
            cout << "[f]" << dest.position;

            // Sumo la distancia euclediana entre los nodos del camino, en orden de visita.
            float timeSpent = 0;
            Vector2 prev = start.position;
            for (Node* node: path) {
                timeSpent += prev.Distance(node->vertex.position);
                if (tmax <= timeSpent)
                    return false;
                prev = node->vertex.position;
            }
            timeSpent += prev.Distance(dest.position);
            cout << " d" << timeSpent;
            return tmax >= timeSpent;
        }

        friend ostream& operator<<(ostream& os, const Node& a)
        {
            os << 'l' << a.level << " v" << a.value << ' ' << a.vertex;
            return os;
        }
};

vector<Vertex> readFile(string d, int* n, int* m, float* tmax) {
    string line;
    ifstream f(d);
    
    getline(f, line);
    sscanf(line.c_str(), "n %d", n);
    getline(f, line);
    sscanf(line.c_str(), "m %d", m);
    getline(f, line);
    sscanf(line.c_str(), "tmax %f", tmax);

    vector<Vertex> vertices;
    float x, y;
    int profit;
    while (getline(f, line)) {
        sscanf(line.c_str(), "%f %f %d", &x, &y, &profit);
        vertices.push_back(Vertex(x, y, profit));
    }
    f.close();

    return vertices;
}

int main() {
    int n, m;
    float tmax;
    vector<Vertex> vertices = readFile("./p2.2.a.txt", &n, &m, &tmax);
    
    Node root(vertices[1]);
    root.value = 0;
    for (int i = 0; i < vertices.size() - 2; i++)
        root.domain.push_back(i);

    Node* next;
    Node* curr = &root;
    int val;

    while (true) {
        next = new Node(vertices[curr->level + 2], *curr);
        val = next->GetFirstNotInstValue();
        if (val != -1) {
            next->value = val;
            curr->children.push_back(next);
            cout << *next << endl;
            if (!next->LessThanMaxTime(vertices.front(), vertices.back(), tmax))
                cout << " false" << endl;
            else {
                cout << " true" << endl;
                if (next->level < vertices.size() - 2 - 1)
                {
                    curr = next;
                    cout << "curr = next" << endl;
                }
                else
                    cout << "max level" << endl;
            }
        }
        else if (curr != &root)
            curr = curr->parent;
        else
            break;    
    }
    
    /*
    for (const auto &item : root.domain)
        cout << item << endl;
    */

    return 0;
};