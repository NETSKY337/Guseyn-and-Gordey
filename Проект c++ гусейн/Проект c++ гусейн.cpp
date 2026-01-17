#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

// Структуры данных
struct Hero {
    int id;
    char name[20];
    char class_type[15];
    int level;
    int health;
};

struct Item {
    int id;
    char name[30];
    char type[15];
    int price;
};

// Глобальные переменные
Hero heroes[100];
Item items[100];
int heroCount = 0;
int itemCount = 0;
const char* HERO_FILE = "heroes.bin";
const char* ITEM_FILE = "items.bin";

// Функции для файлов
void loadData() {
    // Загрузка героев
    ifstream f1(HERO_FILE, ios::binary);
    if (f1) {
        heroCount = 0;
        while (f1.read((char*)&heroes[heroCount], sizeof(Hero)) && heroCount < 100) {
            heroCount++;
        }
        f1.close();
    }

    // Загрузка предметов
    ifstream f2(ITEM_FILE, ios::binary);
    if (f2) {
        itemCount = 0;
        while (f2.read((char*)&items[itemCount], sizeof(Item)) && itemCount < 100) {
            itemCount++;
        }
        f2.close();
    }
}

void saveData() {
    // Сохранение героев
    ofstream f1(HERO_FILE, ios::binary);
    for (int i = 0; i < heroCount; i++) {
        f1.write((char*)&heroes[i], sizeof(Hero));
    }
    f1.close();

    // Сохранение предметов
    ofstream f2(ITEM_FILE, ios::binary);
    for (int i = 0; i < itemCount; i++) {
        f2.write((char*)&items[i], sizeof(Item));
    }
    f2.close();
}

// Функции для героев
void addHero() {
    system("cls");
    cout << "ДОБАВЛЕНИЕ ГЕРОЯ\n\n";

    if (heroCount >= 100) {
        cout << "Лимит героев!\n";
        return;
    }

    heroes[heroCount].id = heroCount + 1;

    cout << "Имя: ";
    cin >> heroes[heroCount].name;

    cout << "Класс (Воин/Маг/Лучник): ";
    cin >> heroes[heroCount].class_type;

    cout << "Уровень: ";
    cin >> heroes[heroCount].level;

    cout << "Здоровье: ";
    cin >> heroes[heroCount].health;

    heroCount++;
    saveData();
    cout << "\nГерой добавлен!\n";
}

void showHeroes() {
    system("cls");
    cout << "СПИСОК ГЕРОЕВ\n\n";

    if (heroCount == 0) {
        cout << "Героев нет\n";
        return;
    }

    for (int i = 0; i < heroCount; i++) {
        cout << heroes[i].id << ". " << heroes[i].name
            << " (" << heroes[i].class_type
            << ") Ур." << heroes[i].level
            << " HP:" << heroes[i].health << endl;
    }
}

void deleteHero() {
    system("cls");
    showHeroes();

    if (heroCount == 0) return;

    int id;
    cout << "\nID для удаления: ";
    cin >> id;

    for (int i = 0; i < heroCount; i++) {
        if (heroes[i].id == id) {
            for (int j = i; j < heroCount - 1; j++) {
                heroes[j] = heroes[j + 1];
            }
            heroCount--;
            saveData();
            cout << "Удалено!\n";
            return;
        }
    }
    cout << "Не найден!\n";
}

// Функции для предметов
void addItem() {
    system("cls");
    cout << "ДОБАВЛЕНИЕ ПРЕДМЕТА\n\n";

    if (itemCount >= 100) {
        cout << "Лимит предметов!\n";
        return;
    }

    items[itemCount].id = itemCount + 1;

    cout << "Название: ";
    cin >> items[itemCount].name;

    cout << "Тип (Оружие/Броня/Зелье): ";
    cin >> items[itemCount].type;

    cout << "Цена: ";
    cin >> items[itemCount].price;

    itemCount++;
    saveData();
    cout << "\nПредмет добавлен!\n";
}

void showItems() {
    system("cls");
    cout << "СПИСОК ПРЕДМЕТОВ\n\n";

    if (itemCount == 0) {
        cout << "Предметов нет\n";
        return;
    }

    for (int i = 0; i < itemCount; i++) {
        cout << items[i].id << ". " << items[i].name
            << " (" << items[i].type
            << ") Цена: " << items[i].price << endl;
    }
}

void deleteItem() {
    system("cls");
    showItems();

    if (itemCount == 0) return;

    int id;
    cout << "\nID для удаления: ";
    cin >> id;

    for (int i = 0; i < itemCount; i++) {
        if (items[i].id == id) {
            for (int j = i; j < itemCount - 1; j++) {
                items[j] = items[j + 1];
            }
            itemCount--;
            saveData();
            cout << "Удалено!\n";
            return;
        }
    }
    cout << "Не найден!\n";
}

// Функции поиска
void searchHero() {
    system("cls");
    cout << "ПОИСК ГЕРОЯ\n\n";
    cout << "1. По имени\n";
    cout << "2. По классу\n";
    cout << "3. По уровню (мин.)\n";
    cout << "Выбор: ";

    int choice;
    cin >> choice;

    char text[20];
    int num;

    system("cls");
    cout << "РЕЗУЛЬТАТЫ:\n\n";

    switch (choice) {
    case 1:
        cout << "Имя: ";
        cin >> text;
        for (int i = 0; i < heroCount; i++) {
            if (strstr(heroes[i].name, text)) {
                cout << heroes[i].id << ". " << heroes[i].name << endl;
            }
        }
        break;
    case 2:
        cout << "Класс: ";
        cin >> text;
        for (int i = 0; i < heroCount; i++) {
            if (strcmp(heroes[i].class_type, text) == 0) {
                cout << heroes[i].id << ". " << heroes[i].name << endl;
            }
        }
        break;
    case 3:
        cout << "Мин. уровень: ";
        cin >> num;
        for (int i = 0; i < heroCount; i++) {
            if (heroes[i].level >= num) {
                cout << heroes[i].id << ". " << heroes[i].name << endl;
            }
        }
        break;
    }
}

void searchItem() {
    system("cls");
    cout << "ПОИСК ПРЕДМЕТА\n\n";
    cout << "1. По названию\n";
    cout << "2. По типу\n";
    cout << "3. По цене (макс.)\n";
    cout << "Выбор: ";

    int choice;
    cin >> choice;

    char text[20];
    int num;

    system("cls");
    cout << "РЕЗУЛЬТАТЫ:\n\n";

    switch (choice) {
    case 1:
        cout << "Название: ";
        cin >> text;
        for (int i = 0; i < itemCount; i++) {
            if (strstr(items[i].name, text)) {
                cout << items[i].id << ". " << items[i].name << endl;
            }
        }
        break;
    case 2:
        cout << "Тип: ";
        cin >> text;
        for (int i = 0; i < itemCount; i++) {
            if (strcmp(items[i].type, text) == 0) {
                cout << items[i].id << ". " << items[i].name << endl;
            }
        }
        break;
    case 3:
        cout << "Макс. цена: ";
        cin >> num;
        for (int i = 0; i < itemCount; i++) {
            if (items[i].price <= num) {
                cout << items[i].id << ". " << items[i].name << endl;
            }
        }
        break;
    }
}

// Функции статистики
void showStats() {
    system("cls");
    cout << "СТАТИСТИКА\n\n";

    // 1. Общее количество
    cout << "1. Всего записей: " << (heroCount + itemCount) << endl;
    cout << "   - Героев: " << heroCount << endl;
    cout << "   - Предметов: " << itemCount << endl;

    // 2. Средний уровень
    if (heroCount > 0) {
        int sum = 0;
        for (int i = 0; i < heroCount; i++) {
            sum += heroes[i].level;
        }
        cout << "2. Средний уровень: " << (sum / heroCount) << endl;
    }

    // 3. Общая цена
    if (itemCount > 0) {
        int sum = 0;
        for (int i = 0; i < itemCount; i++) {
            sum += items[i].price;
        }
        cout << "3. Общая цена: " << sum << endl;
    }
}

// Функции сортировки
void sortHeroes() {
    system("cls");
    cout << "СОРТИРОВКА ГЕРОЕВ\n\n";
    cout << "1. По имени (А-Я)\n";
    cout << "2. По уровню (высокий)\n";
    cout << "Выбор: ";

    int choice;
    cin >> choice;

    // Простая сортировка пузырьком
    for (int i = 0; i < heroCount - 1; i++) {
        for (int j = 0; j < heroCount - i - 1; j++) {
            bool swap = false;

            if (choice == 1 && strcmp(heroes[j].name, heroes[j + 1].name) > 0) {
                swap = true;
            }
            if (choice == 2 && heroes[j].level < heroes[j + 1].level) {
                swap = true;
            }

            if (swap) {
                Hero temp = heroes[j];
                heroes[j] = heroes[j + 1];
                heroes[j + 1] = temp;
            }
        }
    }

    saveData();
    cout << "Сортировка завершена!\n";
}

void sortItems() {
    system("cls");
    cout << "СОРТИРОВКА ПРЕДМЕТОВ\n\n";
    cout << "1. По названию (А-Я)\n";
    cout << "2. По цене (дорогой)\n";
    cout << "Выбор: ";

    int choice;
    cin >> choice;

    // Простая сортировка пузырьком
    for (int i = 0; i < itemCount - 1; i++) {
        for (int j = 0; j < itemCount - i - 1; j++) {
            bool swap = false;

            if (choice == 1 && strcmp(items[j].name, items[j + 1].name) > 0) {
                swap = true;
            }
            if (choice == 2 && items[j].price < items[j + 1].price) {
                swap = true;
            }

            if (swap) {
                Item temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
            }
        }
    }

    saveData();
    cout << "Сортировка завершена!\n";
}

// Главное меню
int main() {
    setlocale(LC_ALL, "RU");
    loadData();

    int choice;

    while (true) {
        system("cls");
        cout << "### АРСЕНАЛ ГЕРОЕВ ###\n\n";
        cout << "1. Герои\n";
        cout << "2. Предметы\n";
        cout << "3. Поиск\n";
        cout << "4. Статистика\n";
        cout << "5. Сортировка\n";
        cout << "0. Выход\n";
        cout << "> ";
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
        case 1: { // ГероиZ
            system("cls");
            cout << "ГЕРОИ\n\n";
            cout << "1. Добавить\n";
            cout << "2. Показать\n";
            cout << "3. Удалить\n";
            cout << "> ";
            cin >> choice;

            if (choice == 1) addHero();
            else if (choice == 2) showHeroes();
            else if (choice == 3) deleteHero();
            break;
        }
        case 2: { // Предметы
            system("cls");
            cout << "ПРЕДМЕТЫ\n\n";
            cout << "1. Добавить\n";
            cout << "2. Показать\n";
            cout << "3. Удалить\n";
            cout << "> ";
            cin >> choice;

            if (choice == 1) addItem();
            else if (choice == 2) showItems();
            else if (choice == 3) deleteItem();
            break;
        }
        case 3: { // Поиск
            system("cls");
            cout << "ПОИСК\n\n";
            cout << "1. Героев\n";
            cout << "2. Предметов\n";
            cout << "> ";
            cin >> choice;

            if (choice == 1) searchHero();
            else if (choice == 2) searchItem();
            break;
        }
        case 4: // Статистика
            showStats();
            break;
        case 5: { // Сортировка 
            system("cls");
            cout << "СОРТИРОВКА\n\n";
            cout << "1. Героев\n";
            cout << "2. Предметов\n";
            cout << "> ";
            cin >> choice;

            if (choice == 1) sortHeroes();
            else if (choice == 2) sortItems();
            break;
        }
        }

        if (choice != 0) {
            cout << "\nНажмите Enter...";
            cin.ignore();
            cin.get();
        }
    }

    saveData();
    cout << "\nСохранено. Выход.\n";
    return 0;
}