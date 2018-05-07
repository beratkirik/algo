#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include <functional>

using namespace std;

struct T
{
    string k;
    string v;

    bool operator == (T const & that) const
    {
        return k == that.k && v == that.v;
    }
};

struct H
{
    size_t operator() (T const & o) const
    {
        size_t v = 0;
        v ^= hash<string>{}(o.k);
        v ^= hash<string>{}(o.v);
        return v;
    }
};

template <typename T = int>
struct Board : vector<vector<T>>
{
    using Base = vector<vector<T>>;

    Board(int m = 3, int n = 3) : m{m},n{n}
    {
        resize(m,n);
    }

    Board(vector<vector<int>> v)
    {
        m = v.size();
        if (!v.empty()) {
            auto min = v[0].size();
            for (auto const & e : v) {
                if (e.size() < min) {
                    min = e.size();
                }
            }
            n = min;
        }
        resize(m,n);
        for (auto i = 0; i < m; i++) {
        for (auto j = 0; j < n; j++) {
            (*this)[i][j] = v[i][j];
        }
        }
    }

    auto resize(int m, int n) -> void
    {
        this->Base::resize(m);
        for (auto & row : *this) {
            row.resize(n);
        }
    }

    int m = 0;
    int n = 0;

    friend auto operator << (ostream & s, Board const & b) -> ostream &
    {
        for (auto i = 0; i < b.m; i++) {
        for (auto j = 0; j < b.n; j++) {
            s << b[i][j] << " ";
        }
        s << "\n";
        }
        return s;
    }
};

template<typename T = int>
struct BoardHash
{
    auto operator () (Board<T> const & b) const -> size_t
    {
        size_t v = 0;
        for (auto const & r : b) {
        for (auto const & e : r) {
            v ^= hash<T>{}(e);
        }
        }
        return v;
    }
};

int main()
{
    unordered_set<T,H> s;
    s.emplace(T{"Hello","Beijing"});
    s.emplace(T{"Bye","Beijing"});
    s.emplace(T{"Hello","HongKong"});
    assert(s.find(T{"Hello","Shanghai"}) == s.end());
    assert(s.find(T{"Bye","Beijing"}) != s.end());

    Board<int> board;
    cout << board.m << board.n << endl;
    cout << board.size();
    for (auto row : board) cout << row.size();
    cout << endl;
    cout << board;

    Board<int> board_3x2(3,2);
    Board<int> board_2x3(2,3);
    cout << board_3x2;
    cout << board_2x3;

    cout << (board_3x2 == board_2x3) << endl;
    cout << (board == Board(board)) << endl;

    Board<int> puzzle({{1,2,3},{4,5,6},{7,8,0}});
    cout << puzzle;

    unordered_set<Board<int>,BoardHash<int>> boards;
    boards.insert(puzzle);
    for (auto const & e : boards) {
        cout << e;
    }
    assert(boards.find(puzzle) != boards.end());
    BoardHash<int> h;
    cout << h(puzzle) << endl;
}

