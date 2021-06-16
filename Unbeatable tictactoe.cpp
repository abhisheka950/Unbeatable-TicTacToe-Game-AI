#include <bits/stdc++.h>
using namespace std;

class Board;
class Game;

int minmax(Board *b, int depth, bool isMax, char player, char oppo);

class Board
{
private:
    char **arr = nullptr;
    char winner = ' ';
    int n, size = 0;
public:
    int br = 0;

    Board(int n) : arr(new char *[n]), n(n)
    {
        for (int i = 0; i < n; ++i)
        {
            arr[i] = new char[n];
        }
        resetBoard();
    }

    void resetBoard()
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                arr[i][j] = char(48 + i * n + j);
            }
        }

        winner = ' ';
        size = 0;
        br = 0;
    }

    void printBoard()
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                cout << (arr[i][j] == 'O' ? '@' : arr[i][j]) << "   ";
            }
            cout << "\n\n\n";
        }
    }

    void set(int k, char player)
    {
        arr[k / n][k % n] = player;
        ++size;
    }

    void remove(int k)
    {
        arr[k / n][k % n] = ' ';
        --size;
    }

    char get(int k)
    {
        return arr[k / n][k % n];
    }

    bool isBoardFull()
    {
        return size == n * n;
    }

    int checkWinner(int flag = false, char player = ' ')
    {
        bool f = false;

        for (int i = 0; i < n; ++i)
        {
            f = true;

            for (int j = 0; j < n - 1; ++j)
            {
                if (arr[i][j] != arr[i][j + 1])
                {
                    f = false;
                    break;
                }
            }

            if (f)
            {
                if (flag)
                {
                    if (player == arr[i][0])
                        return 10;
                    else
                        return -10;
                }
                winner = arr[i][0];
                return f;
            }
        }

        for (int i = 0; i < n; ++i)
        {
            f = true;

            for (int j = 0; j < n - 1; ++j)
            {
                if (arr[j][i] != arr[j + 1][i])
                {
                    f = false;
                    break;
                }
            }

            if (f)
            {
                if (flag)
                {
                    if (player == arr[0][i])
                        return 10;
                    else
                        return -10;
                }
                winner = arr[0][i];
                return f;
            }
        }

        f = true;

        for (int i = 0; i < n - 1; ++i)
        {
            if (arr[i][i] != arr[i + 1][i + 1])
            {
                f = false;
                break;
            }
        }

        if (f)
        {
            if (flag)
            {
                if (player == arr[0][0])
                    return 10;
                else
                    return -10;
            }
            winner = arr[0][0];
            return f;
        }

        f = true;

        for (int i = 0; i < n - 1; ++i)
        {
            if (arr[i][n - 1 - i] != arr[i + 1][n - 1 - i - 1])
            {
                f = false;
                break;
            }
        }

        if (f)
        {
            if (flag)
            {
                if (player == arr[0][n - 1])
                    return 10;
                else
                    return -10;
            }
            winner = arr[0][n - 1];
            return f;
        }

        if (flag)
            return 0;
        return f;
    }

    bool isMovesLeft()
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (arr[i][j] >= 48 and arr[i][j] <= 57)
                    return true;
        return false;
    }

    char getWinner()
    {
        return winner;
    }

    void resetWinner()
    {
        winner = ' ';
    }

    int getSize()
    {
        return n;
    }

    bool checkIfFilled(int k)
    {
        return arr[k / n][k % n] != 'X' and arr[k / n][k % n] != 'O';
    }
};

class Player
{
public:
    char sign;
public:
    Player(char sign) : sign(sign){};
    virtual int makeMove(Board *b) = 0;
};

class Human : public Player
{
public:
    Human(char sign) : Player(sign){};

    int makeMove(Board *b)
    {
        int temp;
        char c;

        cout << "Enter Position: ";
        cin >> c;

        if (c == 'q')
        {
            b->br = 1;
            return 0;
        }
        else if (c == 'r')
        {
            b->br = 2;
            return 0;
        }

        if (!b->checkIfFilled(c - 48))
        {
            cout << "Position already occupied...\n";
            return makeMove(b);
        }

        return c - 48;
    }
};

class Easy : public Player
{
public:
    Easy(char sign) : Player(sign){};

    int makeMove(Board *b)
    {

        int temp = rand() % (b->getSize() * b->getSize()), a;

        while (!b->checkIfFilled(temp))
        {
            temp = rand() % (b->getSize() * b->getSize());
        }

        return temp;
    }
};

class Hard : public Player
{
public:
    Hard(char sign) : Player(sign){};

    int makeMove(Board *b)
    {
        int bestVal = -1000;
        pair<int, int> bestMove;
        bestMove.first = -1;
        bestMove.second = -1;

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (b->get(3 * i + j) != 'X' and b->get(3 * i + j) != 'O')
                {
                    char temp = b->get(3 * i + j);

                    b->set(3 * i + j, sign);

                    int moveVal = minmax(b, 0, false, sign, sign == 'X' ? 'O' : 'X');

                    cout << moveVal << '\n';

                    b->set(3 * i + j, temp);

                    if (moveVal > bestVal)
                    {
                        bestMove.first = i;
                        bestMove.second = j;
                        bestVal = moveVal;
                    }
                }
            }
        }

        return bestMove.first * 3 + bestMove.second;
    }
};

int minmax(Board *b, int depth, bool isMax, char player, char oppo)
{
    int score = b->checkWinner(true, player);

    if (score == 10)
    {
        return 10 - depth;
    }

    if (score == -10)
    {
        return -10 + depth;
    }

    if (!b->isMovesLeft())
    {
        return 0;
    }

    if (isMax)
    {
        int best = -1000;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (b->get(3 * i + j) != 'X' and b->get(3 * i + j) != 'O')
                {
                    char temp = b->get(3 * i + j);

                    b->set(3 * i + j, player);

                    best = max(best,
                               minmax(b, depth + 1, !isMax, player, oppo));

                    b->set(3 * i + j, temp);
                }
            }
        }
        return best;
    }
    else
    {
        int best = 1000;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (b->get(3 * i + j) != 'X' and b->get(3 * i + j) != 'O')
                {
                    char temp = b->get(3 * i + j);

                    b->set(3 * i + j, oppo);

                    best = min(best,
                               minmax(b, depth + 1, !isMax, player, oppo));

                    b->set(3 * i + j, temp);
                }
            }
        }
        return best;
    }
}

class Game
{
private:
    Board *board = nullptr;
    Player *p1 = nullptr, *p2 = nullptr;
    bool player1 = true;
public:
    Game(int n = 3) : board(new Board(n)){};

    void initPlayers(string s1, string s2)
    {
        if (s1 == "human")
        {
            p1 = new Human('X');
        }
        else if (s1 == "easy")
        {
            p1 = new Easy('X');
        }
        else if (s1 == "hard")
        {
            p1 = new Hard('X');
        }

        if (s2 == "human")
        {
            p2 = new Human('O');
        }
        else if (s2 == "easy")
        {
            p2 = new Easy('O');
        }
        else if (s2 == "hard")
        {
            p2 = new Hard('O');
        }
    }

    void Play()
    {
        int temp;
        char c;

        system("cls");

        while (true)
        {
            board->printBoard();

            if (player1)
            {
                temp = p1->makeMove(board);
                if (!board->br)
                    board->set(temp, p1->sign);
            }
            else
            {
                temp = p2->makeMove(board);
                if (!board->br)
                    board->set(temp, p2->sign);
            }

            if (board->br == 1)
            {
                break;
            }
            else if (board->br == 2)
            {
                startGame();
            }
            else if (board->checkWinner())
            {
                system("cls");
                cout << "WINNER --> " << board->getWinner() << "\nPress Enter to continue\n";
                cin.ignore();
                while(cin.get() != '\n') {
                    cin >> c;
                }
                break;
            }

            if(!board->isMovesLeft()) {
                system("cls");
                cout << "---- DRAW ----"<< "\nPress Enter to continue\n";
                cin.ignore();
                while(cin.get() != '\n') {
                    cin >> c;
                }
                break;
            }

            player1 = !player1;
            system("cls");
        }
        startGame();
    }

    void startGame()
    {
        system("cls");
        char temp;
        cout << string("To Select -- > Human vs Easy Type --> 1\n") +
                    "To Select -- > Easy vs Human Type --> 2\n" +
                    "To Select -- > Human vs Hard Type --> 3\n" +
                    "To Select -- > Hard vs Human Type --> 4\n" +
                    "To Select -- > Human vs Human Type --> 5\n" +
                    "To exit type 'q' Or to restart in game press 'r'\n";
        cin >> temp;
        if (temp == 'q')
        {
            return;
        }
        else if (temp == '1')
        {
            initPlayers("human", "easy");
        }
        else if (temp == '2')
        {
            initPlayers("easy", "human");
        }
        else if (temp == '3')
        {
            initPlayers("human", "hard");
        }
        else if (temp == '4')
        {
            initPlayers("hard", "human");
        }
        else if (temp == '5')
        {
            initPlayers("human", "human");
        }
        board->resetBoard();
        return Play();
    }
};

int main()
{
    Game *g = new Game();
    g->startGame();
}