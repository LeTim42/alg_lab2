#ifndef PST_HPP
#define PST_HPP
#include <vector>

using namespace std;

struct border {
    int x, y1, y2, d;
};

struct node {
    int val, left = 0, right = 0;
};

class persist_seg_tree {
private:
    vector<node> nodes;
    vector<int> roots;
    int n, h;

    int& val(int node);
    int& left(int node);
    int& right(int node);
    int add(int& prev, int& curr, int tl, int tr, int i, int d);
    int add(int i, int d);
    int get(int curr, int tl, int tr, int l, int r);
public:
    persist_seg_tree(int n);
    int add(int l, int r, int d);
    int get(int x, int i);
};
#endif
