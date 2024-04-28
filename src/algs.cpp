#include "algs.hpp"

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

vector<int> solve_brutforce(input& inp) {
    int n = inp.p.size();
    vector<int> ans(n);
    for (int i = 0; i < n; ++i) {
        const point& p = inp.p[i];
        for (const rect& r : inp.r)
            ans[i] += r.a.x <= p.x && p.x < r.b.x && r.a.y <= p.y && p.y < r.b.y;
    }
    return ans;
}

vector<int> solve_map(input& inp) {
    vector<int> x_coords, y_coords;
    tie(x_coords, y_coords) = squeeze_coords(inp);

    vector<vector<int>> map(x_coords.size()+1, vector<int>(y_coords.size()+1));
    for (const rect& r : inp.r)
        for (int i = r.a.x; i < r.b.x; ++i)
            for (int j = r.a.y; j < r.b.y; ++j)
                map[i+1][j+1]++;
    
    int n = inp.p.size();
    vector<int> ans(n);
    for (int i = 0; i < n; ++i) {
        point p = find_point(x_coords, y_coords, inp.p[i]);
        ans[i] = map[p.x][p.y];
    }
    return ans;
}

vector<int> solve_tree(input& inp) {
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
