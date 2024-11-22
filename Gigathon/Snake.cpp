#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

int widthSize = 20; // Szerokość planszy.
int heightSize = 20; // Wysokość planszy.
int x, y, coinX, coinY, scoreValue; // Pozycje węża, monety oraz wynik.
vector<pair<int, int>> snake; // Wektor przechowujący pozycje węża.
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN }; // Kierunki ruchu węża.
atomic<Direction> directionVariable; // Zmienna atomowa przechowująca kierunek ruchu.
atomic<bool> stopInputThread; // Zmienna atomowa kontrolująca zatrzymanie wątku wejściowego.
bool animationsEnabled = true; // Flaga włączająca animacje.
bool rewardsEnabled = true; // Flaga włączająca nagrody.
int speedMultiplier = 1; // Mnożnik prędkości gry.
bool gameOver = false; // Flaga końca gry.

void clearScreen() {
#ifdef _WIN32
    system("cls"); // Dla systemu Windows.
#else
    system("clear"); // Dla systemu Unix/Linux.
#endif
}

void setupFunction() {
    srand(time(0)); // Inicjalizacja generatora liczb losowych.
    directionVariable = STOP; // Ustawienie początkowego kierunku na STOP.
    x = widthSize / 2; // Ustawienie początkowej pozycji węża na środku planszy.
    y = heightSize / 2; // Ustawienie początkowej pozycji węża na środku planszy.
    coinX = rand() % widthSize; // Losowanie pozycji monety.
    coinY = rand() % heightSize; // Losowanie pozycji monety.
    scoreValue = 0; // Ustawienie początkowego wyniku na 0.
    snake.clear(); // Czyszczenie wektora węża.
    snake.push_back({x, y}); // Dodanie początkowej pozycji węża do wektora.
    gameOver = false; // Ustawienie flagi końca gry na false.
    stopInputThread = false; // Ustawienie flagi zatrzymania wątku wejściowego na false.
}

void drawFunction() {
    clearScreen(); // Czyszczenie ekranu.
    for (int i = 0; i < widthSize + 2; i++) cout << "#"; // Rysowanie górnej krawędzi planszy.
    cout << endl;

    for (int i = 0; i < heightSize; i++) {
        for (int j = 0; j < widthSize; j++) {
            if (j == 0) cout << "#"; // Rysowanie lewej krawędzi planszy.
            if (i == y && j == x) {
                switch (directionVariable) {
                    case LEFT:
                        cout << "⭠"; // Rysowanie węża skierowanego w lewo.
                        break;
                    case RIGHT:
                        cout << "⭢"; // Rysowanie węża skierowanego w prawo.
                        break;
                    case UP:
                        cout << "⬆"; // Rysowanie węża skierowanego w górę.
                        break;
                    case DOWN:
                        cout << "⬇"; // Rysowanie węża skierowanego w dół.
                        break;
                    default:
                        cout << "O"; // Rysowanie węża w stanie spoczynku.
                        break;
                }
            } else if (i == coinY && j == coinX) {
                cout << "C"; // Rysowanie monety.
            } else {
                bool printVariable = false; // Flaga określająca, czy należy rysować ciało węża.
                for (int k = 0; k < snake.size(); k++) {
                    if (snake[k].first == j && snake[k].second == i) {
                        cout << "o"; // Rysowanie ciała węża.
                        printVariable = true;
                    }
                }
                if (!printVariable) cout << " "; // Rysowanie pustego pola.
            }
            if (j == widthSize - 1) cout << "#"; // Rysowanie prawej krawędzi planszy.
        }
        cout << endl;
    }

    for (int i = 0; i < widthSize + 2; i++) cout << "#"; // Rysowanie dolnej krawędzi planszy.
    cout << endl;
    cout << "Score: " << scoreValue << endl; // Wyświetlanie wyniku.
}

void inputFunction() {
    while (!gameOver && !stopInputThread) {
        char input; // Zmienna przechowująca wejście użytkownika.
        cin >> input;
        switch (input) {
            case 'a':
                if (directionVariable != RIGHT) directionVariable = LEFT; // Zmiana kierunku na lewo.
                break;
            case 'd':
                if (directionVariable != LEFT) directionVariable = RIGHT; // Zmiana kierunku na prawo.
                break;
            case 'w':
                if (directionVariable != DOWN) directionVariable = UP; // Zmiana kierunku na górę.
                break;
            case 's':
                if (directionVariable != UP) directionVariable = DOWN; // Zmiana kierunku na dół.
                break;
            case 'x':
                gameOver = true; // Ustawienie flagi końca gry na true.
                break;
        }
    }
}

void logicFunction() {
    int prevX = snake[0].first; // Poprzednia pozycja głowy węża (x).
    int prevY = snake[0].second; // Poprzednia pozycja głowy węża (y).
    int prev2X, prev2Y; // Zmienne pomocnicze do przechowywania poprzednich pozycji ciała węża.
    snake[0].first = x; // Aktualizacja pozycji głowy węża.
    snake[0].second = y; // Aktualizacja pozycji głowy węża.
    for (int i = 1; i < snake.size(); i++) {
        prev2X = snake[i].first;
        prev2Y = snake[i].second;
        snake[i].first = prevX;
        snake[i].second = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (directionVariable) {
        case LEFT:
            x--; // Ruch węża w lewo.
            break;
        case RIGHT:
            x++; // Ruch węża w prawo.
            break;
        case UP:
            y--; // Ruch węża w górę.
            break;
        case DOWN:
            y++; // Ruch węża w dół.
            break;
        default:
            break;
    }

    if (x >= widthSize) x = 0; else if (x < 0) x = widthSize - 1; // Przechodzenie przez krawędzie planszy (x).
    if (y >= heightSize) y = 0; else if (y < 0) y = heightSize - 1; // Przechodzenie przez krawędzie planszy (y).

    for (int i = 0; i < snake.size(); i++)
        if (snake[i].first == x && snake[i].second == y)
            gameOver = true; // Sprawdzenie kolizji węża z samym sobą.

    if (x == coinX && y == coinY) {
        scoreValue += 10; // Zwiększenie wyniku.
        coinX = rand() % widthSize; // Losowanie nowej pozycji monety.
        coinY = rand() % heightSize; // Losowanie nowej pozycji monety.
        snake.push_back({x, y}); // Dodanie nowego segmentu do węża.
    }
}

void gameOverAnimation() {
    int centerX = widthSize / 2; // Środek planszy (x).
    int centerY = heightSize / 2; // Środek planszy (y).
    int maxDistance = max(centerX, centerY); // Maksymalna odległość od środka planszy.

    for (int d = 0; d <= maxDistance; d++) {
        clearScreen(); // Czyszczenie ekranu.
        for (int i = 0; i < widthSize + 2; i++) cout << "#"; // Rysowanie górnej krawędzi planszy.
        cout << endl;

        for (int i = 0; i < heightSize; i++) {
            for (int j = 0; j < widthSize; j++) {
                if (j == 0) cout << "#"; // Rysowanie lewej krawędzi planszy.
                else if (j == widthSize - 1) cout << "#"; // Rysowanie prawej krawędzi planszy.
                else if (abs(centerX - j) <= d && abs(centerY - i) <= d) cout << "#"; // Rysowanie animacji końca gry.
                else cout << " "; // Rysowanie pustego pola.
            }
            cout << "#\n";
        }

        for (int i = 0; i < widthSize + 2; i++) cout << "#"; // Rysowanie dolnej krawędzi planszy.
        cout << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Opóźnienie animacji.
    }

    cout << "\nYou lost!" << endl; // Wyświetlanie komunikatu o przegranej.
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Opóźnienie przed zakończeniem gry.
}

void displaySettingsMenu() {
    clearScreen(); // Czyszczenie ekranu.
    cout << "+-------------------------+\n";
    cout << "| Width             |  " << widthSize << " |\n";
    cout << "+-------------------------+\n";
    cout << "| Height            |  " << heightSize << " |\n";
    cout << "+-------------------------+\n";
    cout << "| Speed Multiplier   |  " << speedMultiplier << " |\n";
    cout << "+-------------------------+\n";
    
    cout << "Change widthSize (Integer Number): ";
    cin >> widthSize; // Zmiana szerokości planszy.
    cout << "Change heightSize (Integer Number): ";
    cin >> heightSize; // Zmiana wysokości planszy.
    cout << "Change speedMultiplier (Integer Number): ";
    cin >> speedMultiplier; // Zmiana mnożnika prędkości.
    
    cout << "\nPress any key to return to the main menu...\n";
    cin.ignore();
    cin.get(); // Oczekiwanie na naciśnięcie klawisza.
}

void displayMainMenu() {
    clearScreen(); // Czyszczenie ekranu.
    cout << "        ██████████            \n";
    cout << "      ██          ██          \n";
    cout << "      ██            ██        \n";
    cout << "    ██  ██      ██  ██        \n";
    cout << "    ██  ██      ██    ██      \n";
    cout << "    ██░░░░      ░░░░  ██      \n";
    cout << "      ██            ██        \n";
    cout << "        ██████████████        \n";
    cout << "    ████    ██      ████      \n";
    cout << "  ██      ██        ██  ██    \n";
    cout << "██    ██          ██      ██  \n";
    cout << "██      ██████████      ██  ██\n";
    cout << "██                      ██  ██\n";
    cout << "  ██                  ████  ██\n";
    cout << "    ██████████████████    ██  \n";
    cout << "\n";
    cout << "(1) Start Game\n";
    cout << "(2) Settings\n";
    cout << "(3) Exit\n";
    cout << "Input: ";
}

int main() {
    while (true) {
        displayMainMenu(); // Wyświetlanie głównego menu.
        int choiceVariable; // Zmienna przechowująca wybór użytkownika.
        cin >> choiceVariable;
        if (choiceVariable == 1) {
            setupFunction(); // Inicjalizacja gry.
            directionVariable = RIGHT; // Ustawienie początkowego kierunku na prawo.
            thread inputThread(inputFunction); // Uruchomienie wątku wejściowego.
            inputThread.detach(); // Oddzielenie wątku wejściowego.
            while (!gameOver) {
                drawFunction(); // Rysowanie planszy.
                logicFunction(); // Logika gry.
                std::this_thread::sleep_for(std::chrono::milliseconds(100 / speedMultiplier)); // Opóźnienie gry.
            }
            stopInputThread = true; // Zatrzymanie wątku wejściowego.
            gameOverAnimation(); // Wyświetlanie animacji końca gry.
        } else if (choiceVariable == 2) {
            displaySettingsMenu(); // Wyświetlanie menu ustawień.
        } else if (choiceVariable == 3) {
            exit(0); // Zakończenie programu.
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return 0;
}
