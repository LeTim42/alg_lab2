#ifndef ALGS_HPP
#define ALGS_HPP
#include "structs.hpp"
#include "pst.hpp"
#include <algorithm>
#include <tuple>

vector<int> solve_brutforce(input& inp); // Time: O(N*M)          | Space: O(1)
vector<int> solve_map(input& inp);       // Time: O(N^3+M)        | Space: O(N^2)
vector<int> solve_tree(input& inp);      // Time: O((N+M)*log(N)) | Space: O(N*log(N))
#endif
