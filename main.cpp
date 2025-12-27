#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

const int MAX_LEN = 1000;
const int MAX_WORDS = 50;
const int WORD_LEN = 1000;
const int ALPH_SIZE = 256;

// Загрузка из файла (часть задания 1: пользователь сам может выбрать способ ввода)
bool loadFromFile(char* text) {
    const char* filePath = "/Users/maksimkokotkin/Documents/laba_four/input.txt";

    ifstream fin(filePath);
    if (!fin) {
        cerr << "Ошибка открытия файла!" << endl;
        return false;
    }

    fin.getline(text, MAX_LEN);
    cout << "Файл прочитан" << endl;
    fin.close();
    return true;
}

// Задание 1: С клавиатуры или с файла (*) вводится последовательность
void getInput(char* text) {
    int choice;
    cout << "Как хотите ввести строку?\n1) из терминала\n2) из файла\n";
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        cout << "Введите строку (заканчивается точкой): ";
        cin.getline(text, MAX_LEN);
    } else {
        if (!loadFromFile(text)) {
            cout << "Используем ввод с клавиатуры: ";
            cin.getline(text, MAX_LEN);
        }
    }
}

int strLen(const char* str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

bool isPunct(char ch) {
    return ch == '.' || ch == ',' || ch == '!' || ch == ':' ||
           ch == ';' || ch == '?' || ch == '-';
}

// Задание 2: - удалить лишние пробелы;
void removeExtraSpaces(char* text) {
    int dst = 0;
    bool spaceFlag = false;
    int i = 0;

    while (text[i] == ' ') i++;

    while (text[i] != '\0') {
        if (text[i] != ' ') {
            text[dst++] = text[i];
            spaceFlag = false;
        } else if (!spaceFlag) {
            text[dst++] = ' ';
            spaceFlag = true;
        }
        i++;
    }

    if (dst > 0 && text[dst-1] == ' ') dst--;
    text[dst] = '\0';
}

// Задание 2: - удалить лишние знаки препинания
void removeExtraPunctuation(char* text) {
    int dst = 0;
    int i = 0;

    while (text[i] != '\0') {
        if (!isPunct(text[i])) {
            text[dst++] = text[i];
        } else {
            // Проверка на корректное использование "..."
            if (text[i] == '.' && text[i+1] == '.' && text[i+2] == '.' &&
                (text[i+3] == ' ' || text[i+3] == '\0')) {
                text[dst++] = '.';
                text[dst++] = '.';
                text[dst++] = '.';
                i += 2;
            }
            // Оставляем последнюю точку
            else if (text[i] == '.' && text[i+1] == '\0') {
                text[dst++] = '.';
            }
            // Убираем лишние знаки
            else {
                text[dst++] = text[i];
                while (isPunct(text[i+1])) i++;
            }
        }
        i++;
    }
    text[dst] = '\0';
}

// Задание 2: - исправить регистр букв, если это требуется
void fixRegister(char* text) {
    bool inWord = false;

    for (int i = 0; text[i] != '\0'; i++) {
        if (!inWord && text[i] != ' ') {
            inWord = true;
            // Первая буква слова делается заглавной
            if (text[i] >= 'a' && text[i] <= 'z') {
                text[i] = text[i] - 32;
            }
        }
        else if (inWord) {
            if (text[i] == ' ') {
                inWord = false;
            }
            // Буквы внутри слова делаются строчными
            else if (text[i] >= 'A' && text[i] <= 'Z') {
                text[i] = text[i] + 32;
            }
        }
    }
}

// Вспомогательная функция для разбиения текста на слова
void splitIntoWords(char* text, char words[][WORD_LEN], int& wordCount) {
    wordCount = 0;
    int idx = 0;
    bool inWord = false;
    int len = strLen(text);

    for (int i = 0; i <= len && wordCount < MAX_WORDS; i++) {
        if (text[i] != ' ' && text[i] != '.' && text[i] != '\0') {
            if (!inWord) {
                inWord = true;
                idx = 0;
            }
            if (idx < WORD_LEN - 1) {
                words[wordCount][idx++] = text[i];
            }
        }
        else {
            if (inWord) {
                words[wordCount][idx] = '\0';
                wordCount++;
                inWord = false;
            }
        }
    }
}

// Задание 3.1: Вывести на экран слова последовательности в обратном порядке
void printWordsReverse(char* text) {
    cout << "\nСлова в обратном порядке:\n";
    char words[MAX_WORDS][WORD_LEN];
    int count = 0;
    splitIntoWords(text, words, count);

    if (count == 0) {
        cout << "Слова не найдены" << endl;
        return;
    }

    for (int i = count - 1; i >= 0; i--) {
        cout << words[i];
        if (i > 0) cout << " ";
    }
    cout << endl;
}

// Задание 4.1: Вывести на экран ту же последовательность, заменив во всех словах первую букву соответствующей прописной буквой
void capitalizeFirstLetters(char* text) {
    cout << "\nЗамена первой буквы на прописную:\n";
    char result[MAX_LEN];
    strcpy(result, text);

    bool newWord = true;
    for (int i = 0; result[i] != '\0'; i++) {
        if (result[i] == ' ') {
            newWord = true;
        } else if (result[i] >= 'a' && result[i] <= 'z' && newWord) {
            result[i] = result[i] - 32;
            newWord = false;
        } else if (result[i] != ' ') {
            newWord = false;
        }
    }

    cout << result << endl;
}

// Задание 5: Реализовать алгоритм линейного поиска (найти все вхождения)
void findAllLinear(const char* text, const char* pattern) {
    int n = strLen(text);
    int m = strLen(pattern);

    if (m == 0 || m > n) {
        cout << "подстрока не найдена" << endl;
        return;
    }

    cout << "Линейный поиск: ";
    bool found = false;

    for (int i = 0; i <= n - m; i++) {
        bool match = true;
        for (int j = 0; j < m; j++) {
            if (text[i+j] != pattern[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            if (found) cout << ", ";
            cout << i;
            found = true;
        }
    }

    if (!found) cout << "не найдено";
    cout << endl;
}

// Задание 5: Реализовать алгоритм Бойера-Мура (найти все вхождения)
void findAllBoyerMoore(const char* text, const char* pattern) {
    int n = strLen(text);
    int m = strLen(pattern);

    if (m == 0 || m > n) {
        cout << "подстрока не найдена" << endl;
        return;
    }

    int shift[ALPH_SIZE];
    for (int i = 0; i < ALPH_SIZE; i++) shift[i] = m;
    for (int i = 0; i < m - 1; i++) {
        shift[(unsigned char)pattern[i]] = m - 1 - i;
    }

    cout << "Бойер-Мур: ";
    bool found = false;
    int pos = 0;

    while (pos <= n - m) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[pos + j]) j--;

        if (j < 0) {
            if (found) cout << ", ";
            cout << pos;
            found = true;
            pos++;
        } else {
            int s = shift[(unsigned char)text[pos + j]] - (m - 1 - j);
            pos += (s > 0) ? s : 1;
        }
    }

    if (!found) cout << "не найдено";
    cout << endl;
}

int main() {
    char text[MAX_LEN];

    // Задание 1: Ввод последовательности
    getInput(text);
    cout << "\nИсходная строка: " << text << endl;

    // Задание 2: Редактирование текста
    removeExtraSpaces(text);
    cout << "Без лишних пробелов: " << text << endl;

    removeExtraPunctuation(text);
    cout << "Без лишних знаков: " << text << endl;

    fixRegister(text);
    cout << "С исправленным регистром: " << text << endl;

    // Задание 3.1: Слова в обратном порядке
    printWordsReverse(text);

    // Задание 4.1: Замена первой буквы на прописную
    capitalizeFirstLetters(text);

    // Задание 5: Поиск подстрок (два алгоритма)
    char pattern[100];
    cout << "\nВведите подстроку для поиска: ";
    cin.getline(pattern, 100);

    findAllLinear(text, pattern);
    findAllBoyerMoore(text, pattern);

    return 0;
}