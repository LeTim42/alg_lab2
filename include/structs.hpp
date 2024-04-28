#ifndef STRUCTS_HPP
#define STRUCTS_HPP
#include <iostream>
#include <vector>

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
#endif
