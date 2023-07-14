#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

typedef vector<vector<int>> Board;
struct point {
    int x, y;
    point(int X = 0, int Y = 0) {
        x = X; y = Y;
    }
};

int nodeIdCounter = 0;

//LCRS w/additional parent pointer.
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
                h->children->ply = h->ply + 1;
                tryCount++;
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
public:
    SlidePuzzle() {
        M = 3;
    }
    typedef pair<int,link> pNode;
    void PFS(Board first, Board goal) {
        tryCount = 0;
        link start = new node(first, find(first, 0),nullptr, nullptr);
        priority_queue<pNode, vector<pNode>, greater<pNode>> pq;
        pq.push(make_pair(0, start));
        while (!pq.empty()) {
            link curr = pq.top().second;
            pq.pop();
            cout<<getBoardCost(curr->board, goal)<<" ";
            if (getBoardCost(curr->board, goal) == 0) {
                cout<<"Solution Found!\n";
                showSolution(curr);
                break;
            }
            curr->children = generateChildren(curr);
            for (link t = curr->children; t != nullptr; t = t->next) {
                   pq.push(make_pair(getBoardCost(t->board, goal), t));
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
                    {5, 8, 6},
                    {0, 7, 4}
                   };
    SlidePuzzle ep;
    ep.PFS(p1start, p1goal);
    ep.PFS(p2start, p2goal);
    return 0;
}