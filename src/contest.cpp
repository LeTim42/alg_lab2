#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>

using namespace std;

template<class T>
istream& operator>>(istream& in, vector<T>& a) {
    for (T& x : a)
        in >> x;
    return in;
}

template<class T>
ostream& operator<<(ostream& out, vector<T>& a) {
    for (T& x : a)
        out << x << ' ';
    return out;
}

struct point {
    int x, y;

    friend istream& operator>>(istream& in, point& p) {
        return in >> p.x >> p.y;
    }
};

struct rect {
    point a, b;

    friend istream& operator>>(istream& in, rect& r) {
        return in >> r.a >> r.b;
    }
};

struct input {
    vector<rect> r;
    vector<point> p;

    friend istream& operator>>(istream& in, input& inp) {
        int n;
        cin >> n;
        inp.r.resize(n);
        in >> inp.r;
        int m;
        cin >> m;
        inp.p.resize(m);
        in >> inp.p;
        return in;
    }
};

struct border {
    int x, y1, y2, d;
};

struct node {
    int val, left = 0, right = 0;
};

class persist_seg_tree {
    vector<node> nodes;
    vector<int> roots;
    int n, h;

    int& val(int node) {
        return nodes[node].val;
    }

    int& left(int node) {
        return nodes[node].left;
    }

    int& right(int node) {
        return nodes[node].right;
    }

    int add(int& prev, int& curr, int tl, int tr, int i, int d) {
        if (tl >= tr) return 0;
        if (tl > i || i >= tr) return val(curr = prev);
        curr = nodes.size();
        nodes.emplace_back(node{val(prev)});
        if (tl + 1 == tr) return val(curr) += d;
        int tm = (tl + tr) / 2;
        return val(curr) = add(left(prev),  left(curr),  tl, tm, i, d) +
                        add(right(prev), right(curr), tm, tr, i, d);
    }

    int add(int i, int d) {
        int x = roots.size();
        roots.emplace_back(0);
        auto max_size = nodes.size() + h + 1;
        if (max_size > nodes.capacity())
            nodes.reserve(max(max_size, nodes.capacity() * 2));
        add(roots[x-1], roots[x], 0, n, i, d);
        return x;
    }

    int get(int curr, int tl, int tr, int l, int r) {
        if (l >= r || !curr) return 0;
        if (l == tl && r == tr) return val(curr);
        int tm = (tl + tr) / 2;
        return get(left(curr),  tl, tm, l, min(r,tm)) +
            get(right(curr), tm, tr, max(l,tm), r);
    }
public:
    persist_seg_tree(int n) : n(n) {
        for (h = 0; n; n >>= 1) h++;
        nodes.emplace_back(node{});
        roots.emplace_back(0);
    }

    int add(int l, int r, int d) {
        add(l, d);
        return add(r, -d);
    }

    int get(int x, int i) {
        return get(roots[x], 0, n, 0, i+1);
    }
};

vector<int> squeeze_coords(vector<int*>& coords_ptrs) {
    sort(coords_ptrs.begin(), coords_ptrs.end(),
        [](const int* a, const int* b) { return *a < *b; });
    vector<int> coords;
    for (int* coord_ptr : coords_ptrs) {
        if (coords.empty() || coords.back() != *coord_ptr)
            coords.emplace_back(*coord_ptr);
        *coord_ptr = coords.size() - 1;
    }
    return coords;
}

tuple<vector<int>, vector<int>> squeeze_coords(input& inp) {
    vector<int*> x_ptrs, y_ptrs;
    x_ptrs.reserve(inp.r.size()*2);
    y_ptrs.reserve(inp.r.size()*2);
    for (rect& r : inp.r) {
        x_ptrs.emplace_back(&r.a.x);
        x_ptrs.emplace_back(&r.b.x);
        y_ptrs.emplace_back(&r.a.y);
        y_ptrs.emplace_back(&r.b.y);
    }
    return {squeeze_coords(x_ptrs), squeeze_coords(y_ptrs)};
}

point find_point(const vector<int>& x_coords, const vector<int>& y_coords, const point& p) {
    return point{(int)(upper_bound(x_coords.begin(), x_coords.end(), p.x) - x_coords.begin()),
                 (int)(upper_bound(y_coords.begin(), y_coords.end(), p.y) - y_coords.begin())};
}

vector<int> solve(input& inp) {
    vector<int> x_coords, y_coords;
    tie(x_coords, y_coords) = squeeze_coords(inp);

    vector<border> borders;
    borders.reserve(inp.r.size()*2);
    for (const rect& r : inp.r) {
        borders.emplace_back(border{r.a.x, r.a.y, r.b.y, 1});
        borders.emplace_back(border{r.b.x, r.a.y, r.b.y, -1});
    }
    sort(borders.begin(), borders.end(),
        [](const border& a, const border& b) { return a.x < b.x; });
    
    persist_seg_tree pst(y_coords.size()+1);
    vector<int> x_states(x_coords.size()+1);
    for (const border& b : borders)
        x_states[b.x+1] = pst.add(b.y1+1, b.y2+1, b.d);
    
    int n = inp.p.size();
    vector<int> ans(n);
    for (int i = 0; i < n; ++i) {
        point p = find_point(x_coords, y_coords, inp.p[i]);
        ans[i] = pst.get(x_states[p.x], p.y);
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    #ifdef LOCAL
    freopen("input.txt", "r", stdin);
    #endif
    input inp;
    cin >> inp;
    auto ans = solve(inp);
    cout << ans;
    return 0;
}
