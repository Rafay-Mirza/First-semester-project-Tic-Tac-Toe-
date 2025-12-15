#include <iostream>
#include <cctype>
#include <cstdlib>
#include <ctime>
using namespace std;

// ------------- COLORS -------------
#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

// ------------- PLAYER STRUCT -------------
struct Player
{
    string name;
    char symbol;
    int score = 0;
};

Player player1, player2;
bool vsComputer = false;
char board[3][3];
char currentTurn;
bool draw = false;

// ------------- RESET BOARD -------------
void ResetBoard()
{
    char num = '1';
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = num++;
    currentTurn = player1.symbol;
    draw = false;
}

// ------------- COLOR SYMBOLS -------------
string ColorSymbol(char sym)
{
    if (sym == player1.symbol)
        return RED + string(1, sym) + RESET;
    if (sym == player2.symbol)
        return GREEN + string(1, sym) + RESET;
    return string(1, sym);
}

// ------------- DISPLAY BOARD -------------
void DisplayBoard()
{
    system("cls");
    cout << "\n"
        << player1.name << " [" << ColorSymbol(player1.symbol) << "] | Score: " << player1.score
        << "\n"
        << player2.name << " [" << ColorSymbol(player2.symbol) << "] | Score: " << player2.score << "\n\n";

    for (int i = 0; i < 3; i++)
    {
        cout << " +---+---+---+\n | ";
        for (int j = 0; j < 3; j++)
        {
            cout << ColorSymbol(board[i][j]) << " | ";
        }
        cout << "\n";
    }
    cout << " +---+---+---+\n";
}

// ------------- CHECK WIN -------------
bool CheckWin(char sym)
{
    for (int i = 0; i < 3; i++)
        if ((board[i][0] == sym && board[i][1] == sym && board[i][2] == sym) ||
            (board[0][i] == sym && board[1][i] == sym && board[2][i] == sym))
            return true;
    if ((board[0][0] == sym && board[1][1] == sym && board[2][2] == sym) ||
        (board[0][2] == sym && board[1][1] == sym && board[2][0] == sym))
        return true;
    return false;
}

// ------------- AI HELPER -------------
bool CanWinNextMove(char sym, int& r, int& c)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] != player1.symbol && board[i][j] != player2.symbol)
            {
                char temp = board[i][j];
                board[i][j] = sym;
                if (CheckWin(sym))
                {
                    board[i][j] = temp;
                    r = i;
                    c = j;
                    return true;
                }
                board[i][j] = temp;
            }
        }
    }
    return false;
}

// ------------- COMPUTER MOVE -------------
void ComputerMove()
{
    int row, col;

    // 1. Win if possible
    if (CanWinNextMove(player2.symbol, row, col))
    {
        board[row][col] = player2.symbol;
        currentTurn = player1.symbol;
        DisplayBoard();
        return;
    }

    // 2. Block player if they can win
    if (CanWinNextMove(player1.symbol, row, col))
    {
        board[row][col] = player2.symbol;
        currentTurn = player1.symbol;
        DisplayBoard();
        return;
    }

    // 3. Take center
    if (board[1][1] != player1.symbol && board[1][1] != player2.symbol)
    {
        board[1][1] = player2.symbol;
        currentTurn = player1.symbol;
        DisplayBoard();
        return;
    }

    // 4. Take first empty cell
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != player1.symbol && board[i][j] != player2.symbol)
            {
                board[i][j] = player2.symbol;
                currentTurn = player1.symbol;
                DisplayBoard();
                return;
            }
}

// ------------- PLAYER MOVE -------------
void PlayerMove()
{
    if (vsComputer && currentTurn == player2.symbol)
    {
        cout << "\nComputer's turn...\n";
        ComputerMove();
        return;
    }

    int choice;
    int row, col;
    cout << "\n"
        << (currentTurn == player1.symbol ? player1.name : player2.name)
        << " [" << ColorSymbol(currentTurn) << "] turn: ";
    cin >> choice;

    // Map choice to row and column using if-else
    if (choice == 1)
    {
        row = 0;
        col = 0;
    }
    else if (choice == 2)
    {
        row = 0;
        col = 1;
    }
    else if (choice == 3)
    {
        row = 0;
        col = 2;
    }
    else if (choice == 4)
    {
        row = 1;
        col = 0;
    }
    else if (choice == 5)
    {
        row = 1;
        col = 1;
    }
    else if (choice == 6)
    {
        row = 1;
        col = 2;
    }
    else if (choice == 7)
    {
        row = 2;
        col = 0;
    }
    else if (choice == 8)
    {
        row = 2;
        col = 1;
    }
    else if (choice == 9)
    {
        row = 2;
        col = 2;
    }
    else
    {
        cout << "Invalid choice! Try again.\n";
        PlayerMove();
        return;
    }

    // Check if cell is empty
    if (board[row][col] == player1.symbol || board[row][col] == player2.symbol)
    {
        cout << "Cell already filled! Try again.\n";
        PlayerMove();
        return;
    }

    board[row][col] = currentTurn;
    currentTurn = (currentTurn == player1.symbol ? player2.symbol : player1.symbol);

    DisplayBoard();
}

// ------------- MAIN GAME -------------
int main()
{
    srand(time(0));

    cout << "Choose mode:\n1. Player vs Player\n2. Player vs Computer\nEnter choice: ";
    int mode;
    cin >> mode;
    vsComputer = (mode == 2);

    cout << "Enter Player 1 Name: ";
    cin >> player1.name;
    cout << "Choose symbol (x, o, #): ";
    cin >> player1.symbol;

    if (vsComputer)
    {
        player2.name = "Computer";
        player2.symbol = (player1.symbol == 'x' ? 'o' : 'x');
    }
    else
    {
        cout << "Enter Player 2 Name: ";
        cin >> player2.name;
        do
        {
            cout << "Choose symbol (x, o, #): ";
            cin >> player2.symbol;
        } while (player2.symbol == player1.symbol);
    }

    char playAgain = 'y';
    while (playAgain == 'y' || playAgain == 'Y')
    {
        ResetBoard();
        DisplayBoard();

        while (true)
        {
            PlayerMove();
            if (CheckWin(player1.symbol) || CheckWin(player2.symbol))
                break;

            bool full = true;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    if (board[i][j] != player1.symbol && board[i][j] != player2.symbol)
                        full = false;

            if (full)
            {
                draw = true;
                break;
            }
        }

        if (draw)
        {
            cout << "\n=== GAME DRAW! ===\n";
            draw = false;
        }
        else if (CheckWin(player1.symbol))
        {
            cout << "\n=== " << RED << player1.name << " [" << player1.symbol << "] WINS ===" << RESET << "\n";
            player1.score++;
        }
        else
        {
            cout << "\n=== " << GREEN << player2.name << " [" << player2.symbol << "] WINS ===" << RESET << "\n";
            player2.score++;
        }

        cout << "\nDo you want to play again? (y/n): ";
        cin >> playAgain;
    }

    cout << "\nFinal Scores:\n"
        << player1.name << " = " << player1.score
        << "\n"
        << player2.name << " = " << player2.score << "\nThanks for playing!\n";

    return 0;
}
