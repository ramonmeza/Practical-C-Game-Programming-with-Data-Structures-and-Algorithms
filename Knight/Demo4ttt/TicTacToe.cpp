#include <iostream>

const int BOARD_SIZE = 3;
char board[BOARD_SIZE][BOARD_SIZE] = { {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'} };
char currentPlayer = 'X';

// Function to draw the board
void drawBoard() {
    std::cout << "Tic-Tac-Toe Board:\n";
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            std::cout << board[i][j];
            if (j < BOARD_SIZE - 1) std::cout << " | ";
        }
        std::cout << std::endl;
        if (i < BOARD_SIZE - 1) std::cout << "--|---|--\n";
    }
}

// Function to check if a player has won
bool checkWin() {
    // Check rows and columns
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return true;
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return true;
    }
    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return true;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return true;

    return false;
}

// Function to check if the board is full
bool checkDraw() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                return false;
            }
        }
    }
    return true;
}

// Function to handle a player's move
bool makeMove(int move) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == '0' + move) {
                board[i][j] = currentPlayer;
                return true;
            }
        }
    }
    return false;
}

// Function to switch to the other player
void switchPlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

int main() {
    int move;
    bool gameRunning = true;

    while (gameRunning) {
        drawBoard();
        std::cout << "Player " << currentPlayer << ", enter your move (1-9): ";

        std::cin >> move;

        if (move < 1 || move > 9) {
            std::cout << "Invalid move. Try again.\n";
            continue;
        }

        if (!makeMove(move)) {
            std::cout << "This spot is already taken. Try again.\n";
            continue;
        }

        if (checkWin()) {
            drawBoard();
            std::cout << "Player " << currentPlayer << " wins!\n";
            gameRunning = false;
        }
        else if (checkDraw()) {
            drawBoard();
            std::cout << "It's a draw!\n";
            gameRunning = false;
        }
        else {
            switchPlayer();
        }
    }

    return 0;
}
