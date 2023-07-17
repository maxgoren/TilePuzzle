#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

typedef vector<vector<int>> Board;

enum PuzzleType {
    EIGHT,
    FIFTEEN
};


struct point {
    int x, y;
    point(int X = 0, int Y = 0) {
        x = X; y = Y;
    }
};

struct node {
    int ply;
    Board board;
    point blank;
    node* parent;
    node* next;
    node* children;
    node(Board info, point begin, node* parent, node* next) {
            this->ply = 0;
            this->blank.x = begin.x;
            this->blank.y = begin.y;
            this->board = info;
            this->parent = parent;
            this->next = next;
            this->children = nullptr;
    }
};

typedef node* link;

class SlidePuzzle {
    int M;
    int tryCount = 0;
    vector<point> dirs = {{1, 0}, {-1, 0}, {0,1},{0,-1}};
    void printBoard(Board& board) {
        for (vector<int> row : board) {
            for (int col : row) {
                cout<<col<<" ";
            }
            cout<<endl;
        }
    }
    void showSolution(link h) {
        link x = h;
        link solution;
        int moves = 0;
        while (x != nullptr) {
            printBoard(x->board);
            cout<<"------\n";
            x = x->parent;
            moves++;
        }
        cout<<"Puzzle completed in "<<moves<<" moves after trying "<<tryCount<<" board configurations.\n";
    }
    bool isSafe(int x, int y) {
        return x >= 0 && x < M && y >= 0 && y < M;
    }
    link generateChildren(link h) {
        for (point d : dirs) {
            point next = {h->blank.x+d.x, h->blank.y+d.y};
            if (isSafe(next.x, next.y)) {
                Board board = h->board;
                swap(board[next.x][next.y], board[h->blank.x][h->blank.y]);
                h->children = new node(board, next, h, h->children);
                if (hasSeen(h->children)) {
                    node* t = h->children;
                    h->children = h->children->next;
                    delete t;
                } else {
                    h->children->ply = h->ply + 1;
                    tryCount++;
                }
            }
        }
        return h->children;
    }
    point find(Board board, int x) {
        for (int i = 0; i < M; i++) {
            for (int j= 0; j < M; j++) {
                if (x == board[i][j])
                    return point(i, j);
            }
        }
        return point(-1,-1);
    }
    bool compareBoards(Board a, Board b) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < M; j++) {
                if (a[i][j] != b[i][j])
                    return false;
            }
        }
        return true;
    }
    int getBoardCost(Board a, Board b) {
        int cost = 0;
        int oop = 0;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < M; j++) {
                if (a[i][j] != b[i][j]) {
                   point p = find(b, a[i][j]);
                   cost += std::abs(i - p.x) + std::abs(j - p.y);
                   oop += 1;
                }
            }
        }       
        return oop;
    }
    void cleanUp(link h) {
        if (h != nullptr) {
            cleanUp(h->children);
            cleanUp(h->next);
            delete h;
        }
    }
    int countInversions(vector<int>& a, vector<int>& aux, int l, int r, int inv) {
    if (r - l <= 1)
        return inv;
    int m = (l+r) / 2;
    inv = countInversions(a, aux, l, m, inv);
    inv = countInversions(a, aux, m, r, inv);
    
    for (int k = l; k < r; k++) aux[k] = a[k];
    int i = l, j = m, k = l;
    while (i < m  && j < r) {
        if (aux[i] < aux[j]) {
            a[k++] = aux[i++];
        } else {
            //inversion!
            a[k++] = aux[j++];
            if (aux[i] && aux[j])
                inv++;
        }
    }
    while (i < m) a[k++] = aux[i++];
    while (j < r) a[k++] = aux[j++];
    return inv;
}
bool hasSeen(link h) {
        if (h == nullptr || h->parent == nullptr)
            return false;
        node* x = h->parent;
        while (x != nullptr) {
            if (compareBoards(x->board, h->board)) {
                return true;
            }
            x = x->parent;
        }
        return false;
    }
    bool checkBoardIsSolvable(Board b) {
        vector<int> ordered;
        for (auto row : b)
            for (auto col : row)
                ordered.push_back(col);
        vector<int> aux; 
        aux.reserve(ordered.size());
        int inversions = countInversions(ordered,aux, 0, ordered.size(), 0);

        if (b[0].size() % 2 != 0 && inversions % 2 == 0)
            return true;
        if (b[0].size() % 2 == 0 && (inversions + (find(b, 0).y+1)) % 2 ==0)
            return true;
        return false;
    }
public:
    SlidePuzzle() {
        M = 0;
    }
    Board generatePuzzle(PuzzleType size) {
        Board ret;
        vector<int> nums;
        switch (size) {
            case EIGHT:
                ret = vector<vector<int>>(3, vector<int>(3));
                nums = vector<int>{1,2,3,4,5,6,7,8,0};
                break;
            case FIFTEEN:
                ret = vector<vector<int>>(4, vector<int>(4));
                nums = vector<int>{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};
                break;
            default:
                break;
        }
        do {
            std::random_shuffle(nums.begin(), nums.end());
            int p = 0;
            for (int i = 0; i < ret[0].size(); i++) {
                for (int j = 0; j < ret[0].size(); j++) {
                    ret[i][j] = nums[p++];
                }
            }
        } while (!checkBoardIsSolvable(ret));
        return ret;
    }
    typedef pair<int,link> pNode;
    void aStar(Board first, Board goal) {
        if (!checkBoardIsSolvable(first)) {
            cout<<"Starting configuration is unsolvable.\n";
            printBoard(first);
            return;
        }
        tryCount = 0;
        M = first[0].size();
        link start = new node(first, find(first, 0),nullptr, nullptr);
        priority_queue<pNode, vector<pNode>, greater<pNode>> pq;
        pq.push(make_pair(0, start));
        while (!pq.empty()) {
            link curr = pq.top().second;
            pq.pop();
            cout<<"\r"<<tryCount<<std::flush;
            if (getBoardCost(curr->board, goal) == 0) {
                cout<<" Solution Found!\n";
                showSolution(curr);
                break;
            }
            curr->children = generateChildren(curr);
            for (link t = curr->children; t != nullptr; t = t->next) {
                   if (!compareBoards(t->board, curr->board)) {
                        pq.push(make_pair(getBoardCost(t->board, goal), t));
                   } else {
                        cout<<"No dupes!\n";
                   }
            }
        }
        cleanUp(start);
    }
};

int main() {
   Board p1start = {
                   {2,8,3},
                   {1,6,4},
                   {7,0,5}
                 };

    Board p1goal = { 
                   {1,2,3},
                   {8,0,4},
                   {7,6,5}
                 };

    Board p2start = {
                    {1, 2, 3},
                    {5, 6, 0},
                    {7, 8, 4}
                    };
 
    Board p2goal = {
                    {1, 2, 3},
                    {4, 5, 6},
                    {7, 8, 0}
                   }; 
       Board p15start = {
                   {1,6,2,7},
                   {0,5,4,11},
                   {10,15,14,3},
                   {9,13,12,8}
                 };
        Board p15goal = { {1,2,3,4},
                          {5,6,7,8},
                          {9,10,11,12},
                          {13,14,15,0}};

   
    SlidePuzzle ep;
    ep.aStar(p15start, p15goal);
    return 0;
}