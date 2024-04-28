#include "pst.hpp"

int& persist_seg_tree::val(int node) {
    return nodes[node].val;
}

int& persist_seg_tree::left(int node) {
    return nodes[node].left;
}

int& persist_seg_tree::right(int node) {
    return nodes[node].right;
}

int persist_seg_tree::add(int& prev, int& curr, int tl, int tr, int i, int d) {
    if (tl >= tr) return 0;
    if (tl > i || i >= tr) return val(curr = prev);
    curr = nodes.size();
    nodes.emplace_back(node{val(prev)});
    if (tl + 1 == tr) return val(curr) += d;
    int tm = (tl + tr) / 2;
    return val(curr) = add(left(prev),  left(curr),  tl, tm, i, d) +
                       add(right(prev), right(curr), tm, tr, i, d);
}

int persist_seg_tree::add(int i, int d) {
    int x = roots.size();
    roots.emplace_back(0);
    auto max_size = nodes.size() + h + 1;
    if (max_size > nodes.capacity())
        nodes.reserve(max(max_size, nodes.capacity() * 2));
    add(roots[x-1], roots[x], 0, n, i, d);
    return x;
}

int persist_seg_tree::get(int curr, int tl, int tr, int l, int r) {
    if (l >= r || !curr) return 0;
    if (l == tl && r == tr) return val(curr);
    int tm = (tl + tr) / 2;
    return get(left(curr),  tl, tm, l, min(r,tm)) +
           get(right(curr), tm, tr, max(l,tm), r);
}

persist_seg_tree::persist_seg_tree(int n) : n(n) {
    for (h = 0; n; n >>= 1) h++;
    nodes.emplace_back(node{});
    roots.emplace_back(0);
}

int persist_seg_tree::add(int l, int r, int d) {
    add(l, d);
    return add(r, -d);
}

int persist_seg_tree::get(int x, int i) {
    return get(roots[x], 0, n, 0, i+1);
}
