#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define SIZE 8

char board[SIZE][SIZE];

// Inicializa o tabuleiro com as peças nas posições iniciais
void initializeBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((i + j) % 2 == 0 && i < 3)
                board[i][j] = 'B'; // Peças pretas
            else if ((i + j) % 2 == 0 && i > 4)
                board[i][j] = 'W'; // Peças brancas
            else
                board[i][j] = ' ';
        }
    }
}

// Exibe o tabuleiro atual
void displayBoard() {
    printf("   0 1 2 3 4 5 6 7\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < SIZE; j++)
            printf("|%c", board[i][j]);
        printf("|\n");
    }
}

// Verifica se uma posição está dentro dos limites do tabuleiro
bool isValidPosition(int row, int col) {
    return row >= 0 && row < SIZE && col >= 0 && col < SIZE;
}

// Verifica se uma peça pertence ao jogador atual
bool isPlayerPiece(char player, int row, int col) {
    if (isValidPosition(row, col)) {
        if (player == 'B' && (board[row][col] == 'B' || board[row][col] == 'K'))
            return true;
        else if (player == 'W' && (board[row][col] == 'W' || board[row][col] == 'Q'))
            return true;
    }
    return false;
}

// Verifica se uma posição está vazia
bool isEmpty(int row, int col) {
    return board[row][col] == ' ';
}

// Verifica se um movimento é válido para uma peça normal
bool isValidMove(char player, int row, int col, int newRow, int newCol) {
    if (!isValidPosition(newRow, newCol) || !isEmpty(newRow, newCol))
        return false;

    int rowDiff = newRow - row;
    int colDiff = abs(newCol - col);

    if (player == 'B' && rowDiff == 1 && colDiff == 1 && newCol > col)
        return true;
    else if (player == 'W' && rowDiff == -1 && colDiff == 1 && newCol > col)
        return true;
    else
        return false;
}

// Executa um movimento para uma peça normal
void makeMove(char player, int row, int col, int newRow, int newCol) {
    if (isValidMove(player, row, col, newRow, newCol)) {
        board[newRow][newCol] = board[row][col];
        board[row][col] = ' ';
    }
}

// Verifica se um movimento é uma captura válida
bool isValidCapture(char player, int row, int col, int newRow, int newCol) {
    if (!isValidPosition(newRow, newCol) || !isEmpty(newRow, newCol))
        return false;

    int rowDiff = newRow - row;
    int colDiff = abs(newCol - col);

    if (rowDiff == 2 && colDiff == 2) {
        int middleRow = (newRow + row) / 2;
        int middleCol = (newCol + col) / 2;

        if (isPlayerPiece(player, middleRow, middleCol) && !isPlayerPiece(player, row, col))
            return true;
    }
    return false;
}

// Executa uma captura
void makeCapture(char player, int row, int col, int newRow, int newCol) {
    if (isValidCapture(player, row, col, newRow, newCol)) {
        int middleRow = (newRow + row) / 2;
        int middleCol = (newCol + col) / 2;

        board[newRow][newCol] = board[row][col];
        board[row][col] = ' ';
        board[middleRow][middleCol] = ' ';
    }
}

// Verifica se uma peça pode fazer uma captura
bool canCapture(char player) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (isPlayerPiece(player, i, j)) {
                if (isValidCapture(player, i, j, i + 2, j + 2) || isValidCapture(player, i, j, i + 2, j - 2) ||
                    isValidCapture(player, i, j, i - 2, j + 2) || isValidCapture(player, i, j, i - 2, j - 2))
                    return true;
            }
        }
    }
    return false;
}

// Verifica se o jogador atual tem alguma peça restante
bool hasPiecesLeft(char player) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (isPlayerPiece(player, i, j))
                return true;
        }
    }
    return false;
}

// Verifica se o jogador atual venceu
bool hasWon(char player) {
    return !hasPiecesLeft(player == 'B' ? 'W' : 'B');
}

// Função principal do jogo
int main() {
    char currentPlayer = 'B'; // Começa com as peças pretas
    initializeBoard();

    while (true) {
        system("clear"); // Limpa o console (UNIX)
        displayBoard();

        printf("\nPlayer %c's turn\n", currentPlayer);
        printf("Enter move (e.g., '0 1 to 1 2'): ");

        int row, col, newRow, newCol;
        scanf("%d %d to %d %d", &row, &col, &newRow, &newCol);

        if (isValidPosition(row, col) && isValidPosition(newRow, newCol)) {
            if (isPlayerPiece(currentPlayer, row, col)) {
                if (isValidMove(currentPlayer, row, col, newRow, newCol))
                    makeMove(currentPlayer, row, col, newRow, newCol);
                else if (isValidCapture(currentPlayer, row, col, newRow, newCol))
                    makeCapture(currentPlayer, row, col, newRow, newCol);
                else
                    printf("Invalid move!\n");
            } else {
                printf("That's not your piece!\n");
            }
        } else {
            printf("Invalid position!\n");
        }

        if (!canCapture(currentPlayer) || !hasPiecesLeft(currentPlayer))
            break;

        currentPlayer = (currentPlayer == 'B') ? 'W' : 'B'; // Alternar jogadores
    }

    system("clear"); // Limpa o console (UNIX)
    displayBoard();

    if (hasWon('B'))
        printf("\nPlayer B wins!\n");
    else if (hasWon('W'))
        printf("\nPlayer W wins!\n");
    else
        printf("\nIt's a draw!\n");

    return 0;
}
