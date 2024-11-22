#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;

// Flaga do włączania/wyłączania animacji.
bool animationsEnabled = true;

// Funkcja do czyszczenia ekranu.
void clearScreen() {
#ifdef _WIN32
    system("cls"); // Komenda dla systemu Windows.
#else
    system("clear"); // Komenda dla systemów Unix/Linux.
#endif
}

// Funkcja do wyświetlania planszy gry/
void displayBoard(const vector<vector<char>>& board, const vector<vector<bool>>& revealed = {}, int animRow = -1, int animCol = -1, char animChar = ' ') {
    clearScreen(); // Czyszczenie ekranu przed wyświetleniem planszy.
    cout << "    1   2   3\n"; // Wyświetlanie nagłówka kolumn.
    for (int i = 0; i < 3; ++i) {
        if (i > 0) {
            cout << "   ---+---+---\n"; // Wyświetlanie separatora między wierszami.
        }
        cout << i + 1 << "   "; // Wyświetlanie numeru wiersza.
        for (int j = 0; j < 3; ++j) {
            if (!revealed.empty() && revealed[i][j]) {
                if (i == animRow && j == animCol) {
                    cout << animChar; // Wyświetlanie animowanego znaku.
                } else {
                    cout << board[i][j]; // Wyświetlanie znaku na planszy.
                }
            } else {
                cout << (board[i][j] == ' ' ? ' ' : board[i][j]); // Wyświetlanie znaku lub pustego miejsca.
            }
            if (j < 2) cout << " | "; // Wyświetlanie separatora między kolumnami.
        }
        cout << endl;
    }
}

// Funkcja do płynnego wyświetlania animacji odkrywania pola.
void smoothRevealAnimation(int row, int col, vector<vector<char>>& board, char player) {
    if (!animationsEnabled) {
        board[row][col] = player; // Jeśli animacje są wyłączone, bezpośrednio ustaw znak gracza.
        return;
    }
    
    char steps[] = {'#', '-', '/', '|', '\\', '?', player}; // Kroki animacji.
    int totalSteps = sizeof(steps) / sizeof(steps[0]);
    vector<vector<bool>> revealed(3, vector<bool>(3, false)); // Macierz do śledzenia odkrytych pól.
    for (int i = 0; i < totalSteps; ++i) {
        revealed[row][col] = true; // Oznacz pole jako odkryte.
        displayBoard(board, revealed, row, col, steps[i]); // Wyświetl planszę z animacją.
        this_thread::sleep_for(chrono::milliseconds(100)); // Opóźnienie między krokami animacji.
    }
    board[row][col] = player; // Ustaw znak gracza na planszy po zakończeniu animacji.
}

// Funkcja do sprawdzania, czy dany gracz wygrał.
bool checkWin(const vector<vector<char>>& board, char player) {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return true; // Sprawdzenie wierszy.
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return true; // Sprawdzenie kolumn.
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return true; // Sprawdzenie przekątnej.
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return true; // Sprawdzenie przekątnej.
    return false;
}

// Funkcja do sprawdzania, czy gra zakończyła się remisem.
bool checkDraw(const vector<vector<char>>& board) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') return false; // Jeśli jest puste pole, gra nie jest remisem.
        }
    }
    return true; // Jeśli nie ma pustych pól, gra jest remisem.
}

// Funkcja do znalezienia ruchu wygrywającego dla danego gracza.
pair<int, int> getWinningMove(const vector<vector<char>>& board, char player) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                vector<vector<char>> tempBoard = board;
                tempBoard[i][j] = player; // Symulacja ruchu.
                if (checkWin(tempBoard, player)) {
                    return {i, j}; // Jeśli ruch wygrywa, zwróć jego pozycję.
                }
            }
        }
    }
    return {-1, -1}; // Jeśli nie ma ruchu wygrywającego, zwróć (-1, -1).
}

// Funkcja do znalezienia losowego ruchu.
pair<int, int> getRandomMove(const vector<vector<char>>& board) {
    vector<pair<int, int>> emptyCells;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                emptyCells.emplace_back(i, j); // Dodaj puste pole do listy.
            }
        }
    }
    if (!emptyCells.empty()) {
        return emptyCells[rand() % emptyCells.size()]; // Zwróć losowe puste pole.
    }
    return {-1, -1}; // Jeśli nie ma pustych pól, zwróć (-1, -1).
}

// Funkcja do znalezienia ruchu dla bota
pair<int, int> getBotMove(const vector<vector<char>>& board, char bot, char opponent) {
    pair<int, int> move = getWinningMove(board, bot);
    if (move.first != -1) return move; // Jeśli bot ma ruch wygrywający, zwróć go.
    
    move = getWinningMove(board, opponent);
    if (move.first != -1) return move; // Jeśli przeciwnik ma ruch wygrywający, zablokuj go.
    
    return getRandomMove(board); // W przeciwnym razie, wykonaj losowy ruch.
}

// Funkcja do rozgrywania gry.
void playGame(bool vsAI = false) {
    vector<vector<char>> board(3, vector<char>(3, ' ')); // Inicjalizacja pustej planszy.
    char currentPlayer = 'X';
    srand(time(0)); // Inicjalizacja generatora liczb losowych.

    while (true) {
        displayBoard(board); // Wyświetlanie planszy.
        int row, col;

        if (vsAI && currentPlayer == 'O') {
            pair<int, int> move = getBotMove(board, 'O', 'X'); // Ruch bota.
            row = move.first;
            col = move.second;
            cout << "Computer (O) plays at: " << row + 1 << ", " << col + 1 << endl;
            this_thread::sleep_for(chrono::milliseconds(500)); // Opóźnienie dla ruchu bota.
        } else {
            cout << "Player " << currentPlayer << ", enter your move (row and column): ";
            cin >> row >> col;
            --row; --col;
            if (row < 0 || row >= 3 || col < 0 || col >= 3 || board[row][col] != ' ') {
                cout << "Invalid move. Try again." << endl;
                continue; // Jeśli ruch jest nieprawidłowy, poproś o ponowne wprowadzenie.
            }
        }

        smoothRevealAnimation(row, col, board, currentPlayer); // Animacja odkrywania pola.

        if (checkWin(board, currentPlayer)) {
            displayBoard(board);
            cout << "Player " << currentPlayer << " wins!" << endl;
            break; // Jeśli gracz wygrał, zakończ grę.
        }
        if (checkDraw(board)) {
            displayBoard(board);
            cout << "It's a draw!" << endl;
            break; // Jeśli jest remis, zakończ grę.
        }
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X'; // Zmiana gracza.
    }
}

// Funkcja do wyświetlania menu ustawień.
void displaySettingsMenu() {
    clearScreen();
    cout << "+-------------------------+\n";
    cout << "| Animations  |  " << (animationsEnabled ? "Enabled " : "Disabled") << " |\n";
    cout << "+-------------------------+\n";
    cout << "Enable animations during card reveals? (1 for Yes, 0 for No): ";
    cin >> animationsEnabled; // Ustawienie flagi animacji.
}

// Funkcja do wyświetlania głównego menu.
void displayMainMenu() {
    clearScreen();
    cout << " _______ _        _______           _______ \n"
            "|__   __(_)      |__   __|         |__   __| \n"
            "   | |   _  ___     | | __ _  ___     | | ___   ___ \n"
            "   | |  | |/ __|    | |/ _` |/ __|    | |/ _ \\ / _ \\ \n"
            "   | |  | | (__     | | (_| | (__     | | (_) |  __/ \n"
            "   |_|  |_|\\___|    |_|\\__,_|\\___|    |_|\\___/ \\___| \n\n\n";
    cout << "(1) Play Game\n";
    cout << "(2) Settings\n";
    cout << "(3) Vs. Computer\n";
    cout << "(4) Exit\n";
    cout << "Input: ";
}

// Funkcja do obsługi głównego menu.
void mainMenu() {
    while (true) {
        displayMainMenu();
        int choice;
        cin >> choice;
        switch (choice) {
            case 1:
                playGame(); // Rozpocznij grę dla dwóch graczy.
                break;
            case 2:
                displaySettingsMenu(); // Wyświetl menu ustawień.
                break;
            case 3:
                playGame(true); // Rozpocznij grę przeciwko komputerowi.
                break;
            case 4:
                return; // Zakończ program.
            default:
                cout << "Invalid choice. Try again." << endl; // Obsługa nieprawidłowego wyboru.
                break;
        }
    }
}

// Funkcja główna programu.
int main() {
    mainMenu(); // Wywołanie głównego menu.
    return 0;
}
