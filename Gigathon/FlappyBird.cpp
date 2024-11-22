#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

int widthSize = 20; // Szerokość obszaru gry.
int heightSize = 20; // Wysokość obszaru gry.
int birdX, birdY; // Pozycja ptaka.
int scoreValue; // Wartość punktów.
vector<pair<int, int>> obstacles; // Wektor przechowujący pozycje przeszkód.
atomic<bool> stopInputThread; // Zmienna atomowa do kontrolowania zatrzymania wątku wejściowego.
bool gameOver = false; // Flaga końca gry.
bool gameStarted = false; // Flaga rozpoczęcia gry.
int gravity = 1; // Efekt grawitacji.
int jumpStrength = -3; // Siła skoku.
int birdVelocity = 0; // Prędkość ptaka.
int obstacleSpeed = 1; // Prędkość przeszkód.
int speedMultiplier = 1; // Mnożnik prędkości gry.

void clearScreen() {
#ifdef _WIN32
    system("cls"); // Dla Windows.
#else
    system("clear"); // Dla Unix/Linux.
#endif
}

void setupFunction() {
    srand(time(0)); // Inicjalizacja generatora liczb losowych.
    birdX = widthSize / 4; // Ustawienie początkowej pozycji ptaka.
    birdY = heightSize / 2;
    scoreValue = 0; // Ustawienie początkowej wartości punktów na 0.
    obstacles.clear(); // Wyczyść wektor przeszkód.
    for (int i = 0; i < 5; i++) {
        int obstacleX = widthSize + i * 10;
        int obstacleY = rand() % (heightSize - 5) + 2;
        obstacles.push_back({obstacleX, obstacleY});
    }
    gameOver = false; // Ustawienie flagi końca gry na false.
    stopInputThread = false; // Ustawienie flagi zatrzymania wątku wejściowego na false.
    birdVelocity = 0; // Ustawienie początkowej prędkości ptaka na 0.
    gameStarted = false; // Ustawienie flagi rozpoczęcia gry na false.
}

void drawFunction() {
    clearScreen(); // Wyczyść ekran.
    for (int i = 0; i < widthSize + 2; i++) cout << "#"; // Rysowanie górnej granicy.
    cout << endl;

    for (int i = 0; i < heightSize; i++) {
        for (int j = 0; j < widthSize; j++) {
            if (j == 0) cout << "#"; // Rysowanie lewej granicy.
            if (i == birdY && j == birdX) {
                cout << "B"; // Rysowanie ptaka.
            } else {
                bool isObstacle = false;
                for (auto &obstacle : obstacles) {
                    if (j == obstacle.first && (i == obstacle.second || i == obstacle.second + 1 || i == obstacle.second - 1)) {
                        cout << "#"; // Rysowanie przeszkody.
                        isObstacle = true;
                        break;
                    }
                }
                if (!isObstacle) cout << " "; // Rysowanie pustej przestrzeni.
            }
            if (j == widthSize - 1) cout << "#"; // Rysowanie prawej granicy.
        }
        cout << endl;
    }

    for (int i = 0; i < widthSize + 2; i++) cout << "#"; // Rysowanie dolnej granicy.
    cout << endl;
    cout << "Punkty: " << scoreValue << endl; // Wyświetlanie punktów.
}

void inputFunction() {
    while (!gameOver && !stopInputThread) {
        char input; // Zmienna do przechowywania wejścia użytkownika.
        cin >> input;
        if (input == 'J' || input == 'j') {
            birdVelocity = jumpStrength; // Spraw, aby ptak skoczył.
            gameStarted = true; // Rozpocznij grę.
        }
    }
}

void logicFunction() {
    if (!gameStarted) return; // Nie aktualizuj stanu gry, jeśli gra się nie rozpoczęła.

    birdVelocity += gravity; // Zastosuj grawitację do prędkości ptaka.
    birdY += birdVelocity; // Aktualizuj pozycję ptaka.

    if (birdY >= heightSize || birdY < 0) {
        gameOver = true; // Zakończ grę, jeśli ptak uderzy w ziemię lub poleci za wysoko.
    }

    for (auto &obstacle : obstacles) {
        obstacle.first -= obstacleSpeed; // Przesuń przeszkody w lewo.
        if (obstacle.first < 0) {
            obstacle.first = widthSize;
            obstacle.second = rand() % (heightSize - 5) + 2;
            scoreValue += 1; // Zwiększ punkty, gdy przeszkoda zostanie minięta.
        }
        if (birdX == obstacle.first && (birdY == obstacle.second || birdY == obstacle.second + 1 || birdY == obstacle.second - 1)) {
            gameOver = true; // Zakończ grę, jeśli ptak zderzy się z przeszkodą.
        }
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
    clearScreen(); // Wyczyść ekran.
    cout << "+-------------------------+\n";
    cout << "| Szerokość         |  " << widthSize << " |\n";
    cout << "+-------------------------+\n";
    cout << "| Wysokość          |  " << heightSize << " |\n";
    cout << "+-------------------------+\n";
    cout << "| Mnożnik prędkości |  " << speedMultiplier << " |\n";
    cout << "+-------------------------+\n";
    cout << "| Grawitacja        |  " << gravity << " |\n";
    cout << "+-------------------------+\n";
    cout << "| Siła skoku        |  " << jumpStrength << " |\n";
    cout << "+-------------------------+\n";
    cout << "| Prędkość przeszkód|  " << obstacleSpeed << " |\n";
    cout << "+-------------------------+\n";
    
    cout << "Zmień szerokość (Liczba całkowita): ";
    cin >> widthSize; // Zmień szerokość obszaru gry.
    cout << "Zmień wysokość (Liczba całkowita): ";
    cin >> heightSize; // Zmień wysokość obszaru gry.
    cout << "Zmień mnożnik prędkości (Liczba całkowita): ";
    cin >> speedMultiplier; // Zmień mnożnik prędkości.
    cout << "Zmień grawitację (Liczba całkowita): ";
    cin >> gravity; // Zmień grawitację.
    cout << "Zmień siłę skoku (Liczba całkowita): ";
    cin >> jumpStrength; // Zmień siłę skoku.
    cout << "Zmień prędkość przeszkód (Liczba całkowita): ";
    cin >> obstacleSpeed; // Zmień prędkość przeszkód.
    
    cout << "\nNaciśnij dowolny klawisz, aby wrócić do głównego menu...\n";
    cin.ignore();
    cin.get(); // Czekaj na naciśnięcie klawisza.
}

void displayMainMenu() {
    clearScreen(); // Wyczyść ekran.
    cout << "           ▄██████████████▄\n";
    cout << "       ▄████░░░░░░░░█▀    █▄\n";
    cout << "      ██░░░░░░░░░░░█▀      █▄\n";
    cout << "     ██░░░░░░░░░░░█▀        █▄\n";
    cout << "    ██░░░░░░░░░░░░█          ██\n";
    cout << "   ██░░░░░░░░░░░░░█      ██  ██\n";
    cout << "  ██░░░░░░░░░░░░░░█▄     ██  ██\n";
    cout << " ████████████░░░░░░██        ██\n";
    cout << "██░░░░░░░░░░░██░░░░░█████████████\n";
    cout << "██░░░░░░░░░░░██░░░░█▓▓▓▓▓▓▓▓▓▓▓▓▓█\n";
    cout << "██░░░░░░░░░░░██░░░█▓▓▓▓▓▓▓▓▓▓▓▓▓▓█\n";
    cout << " ▀███████████▒▒▒▒█▓▓▓███████████▀\n";
    cout << "    ██▒▒▒▒▒▒▒▒▒▒▒▒█▓▓▓▓▓▓▓▓▓▓▓▓█\n";
    cout << "     ██▒▒▒▒▒▒▒▒▒▒▒▒██▓▓▓▓▓▓▓▓▓▓█\n";
    cout << "      █████▒▒▒▒▒▒▒▒▒▒██████████\n";
    cout << "         ▀███████████▀\n";
    cout << "\n";
    cout << "(1) Start Game\n";
    cout << "(2) Settings\n";
    cout << "(3) Exit\n";
    cout << "Input: ";
}

int main() {
    while (true) {
        displayMainMenu(); // Wyświetl główne menu.
        int choiceVariable; // Zmienna do przechowywania wyboru użytkownika.
        cin >> choiceVariable;
        if (choiceVariable == 1) {
            setupFunction(); // Inicjalizacja gry.
            thread inputThread(inputFunction); // Uruchom wątek wejściowy.
            inputThread.detach(); // Odłącz wątek wejściowy.
            while (!gameOver) {
                drawFunction(); // Rysowanie obszaru gry.
                logicFunction(); // Zastosowanie logiki gry.
                std::this_thread::sleep_for(std::chrono::milliseconds(100 / speedMultiplier)); // Opóźnienie pętli gry.
            }
            stopInputThread = true; // Zatrzymaj wątek wejściowy.
            gameOverAnimation(); // Wyświetl animację końca gry.
        } else if (choiceVariable == 2) {
            displaySettingsMenu(); // Wyświetl menu ustawień.
        } else if (choiceVariable == 3) {
            exit(0); // Zakończ program.
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return 0;
}
