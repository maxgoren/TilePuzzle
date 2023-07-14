#include <iostream>
#include <vector>
#include <queue>
using namespace std;
int ncg = 0;
typedef vector<vector<int>> Board;

struct point {
    int x, y;
    point(int X = 0, int Y = 0) {
        x = X; y = Y;
    }
};

//LCRS w/additional parent pointer.
struct node {
    Board board;
    point blank;
    node* parent;
    node* next;
    node* children;
    node(Board info, int x, int y, node* parent, node* next) {
            this->blank.x = x;
            this->blank.y = y;
            this->board = info;
            this->next = next;
            this->parent = parent;
            this->children = nullptr;
    }
    ~node() {
        cout<<"adios from "<<ncg<<"\n";
        ncg++;
    }
};

typedef node* link;


class EightPuzzle {
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
        return x >= 0 && x < 3 && y >= 0 && y < 3;
    }
    link generateChildren(link h) {
        link children = nullptr;;
        for (point d : dirs) {
            point next = {h->blank.x+d.x, h->blank.y+d.y};
            if (isSafe(next.x, next.y)) {
                Board board = h->board;
                swap(board[next.x][next.y], board[h->blank.x][h->blank.y]);
                children = new node(board, next.x, next.y, h, children);
                tryCount++;
            }
        }
        return children;
    }
    bool compareBoards(Board a, Board b) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (a[i][j] != b[i][j])
                   return false;
        return true;
    }
    void cleanup(link h) {
        if (h != nullptr) {
            cleanup(h->children);
            cleanup(h->next);
            delete h;
        }
    }
public:
    EightPuzzle() {

    }
    void BFS(link start, Board goal) {
        queue<link> fq;
        fq.push(start);
        while (!fq.empty()) {
            link h = fq.front();
            fq.pop();
            if (compareBoards(h->board, goal)) {
                cout<<"Solution Found!\n";
                showSolution(h);
                break;
            }
            h->children = generateChildren(h);
            for (link t = h->children; t != nullptr; t = t->next)
                fq.push(t);
        }
        cleanup(start);
    }
};

int main() {
    Board start = {
                    {2,8,3},
                    {1,6,4},
                    {7,0,5} 
                    };

    Board goal = { 
                   {1,2,3},
                   {8,0,4},
                   {7,6,5}
                    };
    EightPuzzle ep;
    ep.BFS(new node(start, 2,1,nullptr, nullptr), goal);
    return 0;
}