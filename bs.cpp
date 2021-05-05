#include <iostream>
#include <vector>

using namespace std;
class Vertex {
public:
    int value;
    vector<int> edges;
    
    Vertex(int _value): value(_value) {}
};

class Edge {
public:
    int v;
    int w;
    
    Edge(int _v, int _w): v(_v), w(_w) {}
};

class Graph {
public:
    vector<Vertex> vertices;
    vector<Edge> edges;

    Vertex find_vertex(int _value) {
        Vertex ans = Vertex(-1);
        for (auto v : vertices)
            if (v.value == _value) return v;
        return ans;
    }
    
    bool find_edge(int _v, int _w) {
        Vertex v = find_vertex(_v);
        for (auto c : v.edges)
            if (c == _w) return 1;
        return 0;
    }
    
    void add_edge(int _v, int _w) {
        int has_edge = find_edge(_v, _w);
        if (!has_edge) {
            Edge e = Edge(_v, _w);
            Vertex v = find_vertex(_v);
            Vertex w = find_vertex(_w);
            v.edges.push_back(_w);
            w.edges.push_back(_v);
            edges.push_back(e);
        }
    }
    
    void remove_edge(int _v, int _w) {
        int has_edge = find_edge(_v, _w);
        if (has_edge) {
            Vertex v = find_vertex(_v);
            Vertex w = find_vertex(_w);
            for (auto it = v.edges.begin(); it < v.edges.end(); ++it)
                if (*it == _w) v.edges.erase(it);
            for (auto it = w.edges.begin(); it < w.edges.end(); ++it)
                if (*it == _v) w.edges.erase(it);
            for (auto it = edges.begin(); it < edges.end(); ++it){
                if (((*it).v == _v && (*it).w == _w) || ((*it).v == _w && (*it).w == _v))
                    edges.erase(it);
            }
                    
        }
    }
    
    void add_vertex(int _value) {
        Vertex v = Vertex(_value);
        vertices.push_back(v);
    }
    
    void remove_vertex(int _value) {
        for (auto it = vertices.begin(); it < vertices.end(); ++it) {
            if ((*it).value == _value)
                vertices.erase(it);
        }
    }
    
    Graph clone() {
        Graph g = Graph();
        for (auto v : vertices)
            g.add_vertex(v.value);
        for (auto e : edges)
            g.add_edge(e.v, e.w);
        return g;
    }
};

class Matching: public Graph {
public:
    void augment_path(vector<Edge> path) {
        for(auto e : path){
            Vertex v = find_vertex(e.v);
            Vertex w = find_vertex(e.w);
            bool has_edge = find_edge(e.v, e.w);
            if (has_edge)
                remove_edge(e.v, e.w);
            else
                add_edge(e.v, e.w);
        }
    }
    
    int get_matched(int _value){
        Vertex v = find_vertex(_value);
        return v.edges[0];
    }
    
    Matching from_graph(Graph source){
        Matching m;
        for (auto v : source.vertices) {
            m.add_vertex(v.value);
        }
        return m;
    }
};

class Tree {
public:
    int value;
    vector<Tree> children;
    Tree* parent;
    
    Tree(int _value, vector<Tree> _children = vector<Tree> (), Tree* par = nullptr): value(_value), children(_children), parent(par) {}
    
    void add_child(int value) {
        Tree c = Tree(value, vector<Tree> (), this);
        children.push_back(c);
    }
    
    vector<Tree> subnodes(){
        vector<Tree> ans;
        for (auto c : children)
            ans.insert(ans.end(), c.subnodes().begin(), c.subnodes().end());
        return ans;
    }
    
    int height(){
        if (parent == nullptr) return 0;
        return 1 + parent -> height();
    }
    
    Tree* find(Tree* root, int _value){
        if (root == nullptr) return nullptr;
        if (root -> value == value) return root;
        for (auto c : root->children)
            find(&c, _value);
        return nullptr;
    }
    
    Tree* root(Tree* cur){
        if(cur->parent == nullptr)
            return parent;
        return root(cur->parent);
    }
};

class Blossom {
public:
    int index = 0;
    int v, w;
    Tree* t;
    vector<Tree*> path;
    Blossom(int v, int w, Tree* t){
        v = v;
        w = w;
        t = t;
        precompute();
    }
    
    void precompute(){
        if(path.size() != 0)
            return;
        Tree* v_tree = t -> find(t, v);
        Tree* w_tree = t -> find(t, w);
        int v_height = v_tree -> height();
        int w_height = w_tree -> height();
        vector<Tree*> v_path;
        vector<Tree*> w_path;
        Tree* v_it = v_tree;
        Tree* w_it = v_tree;
        for(int i = 0; i < v_height - w_height; ++i){
            v_path.push_back(v_it);
            v_it = v_it -> parent;
        }
        for(int i = 0; i < v_height - w_height; ++i){
            w_path.push_back(w_it);
            w_it = v_it -> parent;
        }
        reverse(w_path.begin(), w_path.end());
        path = v_path;
        for (auto c : w_path)
            path.push_back(c);
    }
};

vector<Edge> find_augmenting_path(Graph g, Matching m){
    vector<Edge> forest;
    for (auto v : m.edges){
        forest.push_back(v);
    }
    return forest;
}

Matching find_maximum_matching(Graph g, Matching m) {
    vector<Edge> path = find_augmenting_path(g, m);
    if (path.size()){
        m.augment_path(path);
        return find_maximum_matching(g, m);
    }
    return m;
}
