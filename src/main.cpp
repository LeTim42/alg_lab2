#include <fstream>
#include <iomanip>
#include <chrono>
#include <map>
#include "algs.hpp"

const size_t SOLVERS_COUNT = 3;
vector<int>(*SOLVERS[])(input&) = {&solve_brutforce, &solve_map, &solve_tree};
const char* SOLVERS_NAMES[] = {"brutforce", "rectangles map", "persistent segment tree"};

/////////////////////////// SETTINGS ///////////////////////////
const bool TEST = true;

// if TEST == true:
const int MAX_N = 1 << 8;
const int MAX_M = 1 << 15;
const int TESTS = 32;

// if TEST == false:
const char* INPUT = "input.txt";
const auto SOLVE = SOLVERS[2];
////////////////////////////////////////////////////////////////

uint64_t now() {
    using namespace chrono;
    return duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count();
}

const int PX = 998244353;
const int PY = 1000000007;

int mod = 20;

int mul(int a, int b) {
    return ((int64_t)a * b) % mod;
}

int pow(int a, int n) {
    int x = 1;
    for (; n; n >>= 1) {
        if (n & 1)
            x = mul(x,a);
        a = mul(a,a);
    }
    return x;
}

void test() {
    cout << fixed << setprecision(3);
    map<int,map<int,double[SOLVERS_COUNT][TESTS]>> time;
    for (int t = 0; t < TESTS; ++t) {
        for (int s = 0; s < SOLVERS_COUNT; ++s) {
            cout << "Test " << t+1 << '/' << TESTS << " | Solver " << s+1 << '/' << SOLVERS_COUNT << endl;
            for (int n = 1; n <= MAX_N; n <<= 1) {
                vector<rect> r(n);
                for (int i = 0; i < n; ++i)
                    r[i] = rect{point{10*i,10*i}, point{10*(2*n-i),10*(2*n-i)}};
                vector<point> p;
                mod = 20*n;
                int i = 0;
                int x = -1;
                for (int m = 1; m <= MAX_M; m <<= 1) {
                    if (x != -1 && p.back().x != x) {
                        cout << ":(" << endl;
                        exit(1);
                    }
                    for (; i < m; ++i)
                        p.emplace_back(point{pow(mul(PX,i+1),31), pow(mul(PY,i+1),31)});
                    x = p.back().x;
                    input inp{r,p};
                    auto start = now();
                    auto ans = SOLVERS[s](inp);
                    auto stop = now();
                    time[n][m][s][t] = stop - start;
                }
            }
        }
    }
    cout << endl;
    map<int,map<int,double[SOLVERS_COUNT]>> results;
    for (int n = 1; n <= MAX_N; n <<= 1) {
        for (int m = 1; m <= MAX_M; m <<= 1) {
            cout << ">> n = " << n << "; m = " << m << endl;
            for (int s = 0; s < SOLVERS_COUNT; ++s) {
                sort(time[n][m][s], time[n][m][s] + TESTS);
                results[n][m][s] = 1e-6 * (time[n][m][s][TESTS/2] + time[n][m][s][TESTS/2+TESTS%2-1]) / 2;
                cout << SOLVERS_NAMES[s] << ": " << results[n][m][s] << "ms" << endl;
            }
            cout << endl;
        }
    }
    ofstream fout("output.txt");
    class comma : public numpunct<char> {
    public:
        comma() : numpunct() {}
    protected:
        char do_decimal_point() const {
            return ',';
        }
    };
    fout.imbue(locale(fout.getloc(), new comma()));
    fout << fixed << setprecision(3);
    for (int s = 0; s < SOLVERS_COUNT; ++s) {
        fout << SOLVERS_NAMES[s] << '\n';
        fout << "n \\ m";
        for (int m = 1; m <= MAX_M; m <<= 1)
            fout << '\t' << m;
        fout << '\n';
        for (int n = 1; n <= MAX_N; n <<= 1) {
            fout << n;
            for (int m = 1; m <= MAX_M; m <<= 1)
                fout<< '\t' << results[n][m][s];
            fout << '\n';
        }
        fout << '\n';
    }
}

int main() {
    if (TEST) test();
    else {
        if (INPUT[0]) freopen(INPUT, "r", stdin);
        input inp;
        cin >> inp;
        auto start = now();
        auto ans = SOLVE(inp);
        auto stop = now();
        cout << ans << endl;
        cout << fixed << setprecision(3) << "\nWorking time: " << (stop - start) / 1e6 << "ms";
    }
    return 0;
}
