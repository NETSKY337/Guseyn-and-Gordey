#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <limits>
#include <cwchar>
#include <locale>
#include <cwctype>
using namespace std;

// Прототипы функций
void showHeroes();
void showItems();
void chooseHeroClass(char* class_type, int size);
int safeMenuChoice();
void chooseItemType(char* type, int size);
void safeHeroNameInput(char* buffer, int size);
void safeItemNameInput(char* buffer, int size);
void sortHeroesMenu();
void sortItemsMenu();
void pause(); // Добавлен прототип

struct Hero {
    int id;                 // Уникальный идентификатор героя
    char name[20];          // Имя героя
    char class_type[15];    // Класс героя (воин, маг и т.д.)
    int level;              // Уровень героя
    int health;             // Здоровье героя
};

struct Item {
    int id;
    char name[30];
    char type[15];
    int price;
};

// Глобальные переменные
vector<Hero> heroes;                            // Дин массив для хранения героев
vector<Item> items;                             // Дин массив для хранения предметов
int nextHeroId = 1;                             // Следующий доступный ID для героя
int nextItemId = 1;                             // Следующий доступный ID для предмета
const char* HERO_FILE = "heroes.bin";           // Имя файла для хранения героев
const char* ITEM_FILE = "items.bin";            // Имя файла для хранения предметов

// Преобразование символа CP1251 в нижний регистр - ИСПРАВЛЕНО
char toLowerCP1251(char c) {
    unsigned char uc = static_cast<unsigned char>(c); // Явное преобразование
    if (uc >= 'A' && uc <= 'Z') return static_cast<char>(uc + 32);   // латиница
    if (uc >= 192 && uc <= 223) return static_cast<char>(uc + 32);   // кириллица А-Я -> а-я
    if (uc == 168) return static_cast<char>(184);                   // Ё -> ё
    return c;
}

// Сравнение строк без учёта регистра
bool equalsIgnoreCase(const char* a, const char* b) {
    while (*a && *b) {
        if (toLowerCP1251(*a) != toLowerCP1251(*b)) return false;
        a++; b++;
    }
    return *a == *b && *b == '\0';
}

// Проверка, содержится ли подстрока без учёта регистра
bool containsSubstringIgnoreCase(const char* str, const char* substr) {
    if (!str || !substr) return false;

    int lenSub = static_cast<int>(strlen(substr));
    for (int i = 0; str[i]; i++) {
        bool match = true;
        for (int j = 0; j < lenSub; j++) {
            if (toLowerCP1251(str[i + j]) != toLowerCP1251(substr[j])) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

// Проверка, что строка содержит только буквы
bool isLettersOnly(const char* str) {
    if (!str) return false;
    for (int i = 0; str[i]; i++) {
        unsigned char c = static_cast<unsigned char>(str[i]); // Явное преобразование
        // латиница A-Z a-z
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) continue;
        // кириллица А-Я а-я Ё ё (CP1251)
        if ((c >= 192 && c <= 223) || (c >= 224 && c <= 255) || c == 168 || c == 184)
            continue;
        return false; // всё остальное запрещено
    }
    return strlen(str) > 0;
}

void safeHeroNameInput(char* buffer, int size) {
    while (true) {
        cout << "Имя героя (только буквы): ";
        cin.getline(buffer, size);

        if (!isLettersOnly(buffer)) {
            cout << "Ошибка: имя не должно содержать цифры или спецсимволы!\n";
            continue;
        }
        break;
    }
}

void safeItemNameInput(char* buffer, int size) {
    while (true) {
        cout << "Название предмета (только буквы): ";
        cin.getline(buffer, size);

        if (!isLettersOnly(buffer)) {
            cout << "Ошибка: название не должно содержать цифры или спецсимволы!\n";
            continue;
        }
        break;
    }
}

// Загрузка данных из файлов в память
void loadData() {
    heroes.clear();                         // Очищаем вектор героев
    items.clear();                          // Очищаем вектор предметов

    // Загрузка героев из бинарного файла
    ifstream f1(HERO_FILE, ios::binary);
    if (f1) {
        Hero h;
        // Читаем героев пока не достигнем конца файла
        while (f1.read((char*)&h, sizeof(Hero))) {
            heroes.push_back(h);                                // Добавляем героя в вектор
            if (h.id >= nextHeroId) nextHeroId = h.id + 1;      // Обновляем следующий доступный ID
        }
        f1.close();
    }

    ifstream f2(ITEM_FILE, ios::binary);
    if (f2) {
        Item it;
        while (f2.read((char*)&it, sizeof(Item))) {
            items.push_back(it);
            if (it.id >= nextItemId) nextItemId = it.id + 1;
        }
        f2.close();
    }
}

// Сохранение данных из памяти в файлы
void saveData() {
    // Сохранение героев в бинарный файл
    ofstream f1(HERO_FILE, ios::binary);
    for (const Hero& h : heroes) {
        f1.write((char*)&h, sizeof(Hero));         // Записываем каждого героя в файл
    }
    f1.close();

    // Сохранение предметов в бинарный файл
    ofstream f2(ITEM_FILE, ios::binary);
    for (const Item& it : items) {
        f2.write((char*)&it, sizeof(Item));
    }
    f2.close();
}

// Функция для безопасного ввода числа с проверкой
int safeIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (cin.fail()) {
            cin.clear(); // Сбрасываем флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очищаем буфер
            cout << "Ошибка! Введите целое число.\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очищаем лишние символы
            return value;
        }
    }
}

// Функция для безопасного ввода строки
void safeStringInput(char* buffer, int size, const string& prompt) {
    cout << prompt;
    cin.getline(buffer, size);
    while (strlen(buffer) == 0) {
        cout << "Поле не может быть пустым. " << prompt;
        cin.getline(buffer, size);
    }
}

// Функции для работы с героями
// Добавление нового героя
void addHero() {
    system("cls");
    cout << "ДОБАВЛЕНИЕ ГЕРОЯ\n\n";

    Hero h;
    h.id = nextHeroId++;

    // Ввод имени героя без цифр
    safeHeroNameInput(h.name, sizeof(h.name));

    // Выбор класса
    chooseHeroClass(h.class_type, sizeof(h.class_type));

    // Ввод уровня и здоровья
    h.level = safeIntInput("Уровень: ");
    h.health = safeIntInput("Здоровье: ");

    heroes.push_back(h);
    saveData();

    cout << "\nГерой добавлен!\n";
    showHeroes();
}

// Показать всех героев
void showHeroes() {
    system("cls");                       // Очищаем консоль
    cout << "СПИСОК ГЕРОЕВ\n\n";

    if (heroes.empty()) {                 // Если вектор пустой
        cout << "Героев нет\n";
        return;
    }

    // Выводим информацию о каждом герое
    for (const Hero& h : heroes) {
        cout << h.id << ". " << h.name
            << " (" << h.class_type
            << ") Ур." << h.level
            << " HP:" << h.health << endl;
    }
    cout << "Всего: " << heroes.size() << " героев\n";      // Выводим общее количество
}

// Удаление одного героя по ID
void deleteHero() {
    showHeroes();                      // Сначала показываем список героев

    if (heroes.empty()) return;        // Если нет героев, выходим

    int id = safeIntInput("\nID для удаления: ");

    // Ищем героя с указанным ID
    for (auto it = heroes.begin(); it != heroes.end(); ++it) {
        if (it->id == id) {
            heroes.erase(it);                   // Удаляем героя из вектора
            saveData();                         // Сохраняем изменения
            cout << "Удалено!\n";
            return;
        }
    }
    cout << "Не найден!\n";                     // Если герой не найден
}

// Удаление нескольких героев по списку ID
void deleteMultipleHeroes() {
    showHeroes();
    if (heroes.empty()) return;

    cout << "\nВведите ID героев для удаления (через пробел): ";
    string input;
    getline(cin, input); // теперь корректно считывается вся строка

    stringstream ss(input);
    vector<int> ids;
    int id;

    while (ss >> id) {
        ids.push_back(id);
    }

    if (ids.empty()) {
        cout << "Нет введённых ID!\n";
        return;
    }

    size_t before = heroes.size();
    heroes.erase(
        remove_if(heroes.begin(), heroes.end(),
            [&](const Hero& h) {
                return find(ids.begin(), ids.end(), h.id) != ids.end();
            }),
        heroes.end()
    );
    size_t removed = before - heroes.size();

    saveData();

    cout << "Удалено героев: " << static_cast<int>(removed) << "\n"; // ИСПРАВЛЕНО
}

void chooseItemType(char* type, int size) {
    int choice;
    while (true) {
        cout << "\nВыберите тип предмета:\n";
        cout << "1. Оружие\n";
        cout << "2. Броня\n";
        cout << "3. Зелье\n";
        cout << "> ";
        choice = safeMenuChoice();

        if (choice == 1) strcpy_s(type, size, "Оружие");
        else if (choice == 2) strcpy_s(type, size, "Броня");
        else if (choice == 3) strcpy_s(type, size, "Зелье");
        else {
            cout << "Неправильный выбор. Попробуйте ещё раз.\n";
            continue;
        }
        break;
    }
}

// Функции для работы с предметами
// Добавление нового предмета
void addItem() {
    system("cls");
    cout << "ДОБАВЛЕНИЕ ПРЕДМЕТА\n\n";

    Item it;
    it.id = nextItemId++;

    // Ввод названия предмета без цифр
    safeItemNameInput(it.name, sizeof(it.name));

    // Выбор типа предмета из трёх вариантов
    chooseItemType(it.type, sizeof(it.type));

    // Ввод цены
    it.price = safeIntInput("Цена: ");

    items.push_back(it);
    saveData();

    cout << "\nПредмет добавлен!\n";
    showItems();
}

// Показать все предметы
void showItems() {
    system("cls");
    cout << "СПИСОК ПРЕДМЕТОВ\n\n";

    if (items.empty()) {                   // Если вектор пустой
        cout << "Предметов нет\n";
        return;
    }

    // Выводим информацию о каждом предмете
    for (const Item& it : items) {
        cout << it.id << ". " << it.name
            << " (" << it.type
            << ") Цена: " << it.price << endl;
    }
    cout << "Всего: " << items.size() << " предметов\n";       // Выводим общее количество
}

// Удаление одного предмета по ID
void deleteItem() {
    showItems();                               // Сначала показываем список предметов

    if (items.empty()) return;                 // Если нет предметов, выходим

    int id = safeIntInput("\nID для удаления: ");

    // Ищем предмет с указанным ID
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->id == id) {
            items.erase(it);                       // Удаляем предмет из вектора
            saveData();                            // Сохраняем изменения
            cout << "Удалено!\n";
            return;
        }
    }
    cout << "Не найден!\n";                        // Если предмет не найден
}

// Удаление нескольких предметов по списку ID
void deleteMultipleItems() {
    showItems();  // Сначала показываем список предметов
    if (items.empty()) return;

    cout << "\nВведите ID предметов для удаления (через пробел): ";
    string input;
    getline(cin, input);  // считываем всю строку с ID

    stringstream ss(input);
    vector<int> ids;
    int id;

    // Разбираем строку на отдельные ID
    while (ss >> id) {
        ids.push_back(id);
    }

    if (ids.empty()) {
        cout << "Нет введённых ID!\n";
        return;
    }

    size_t before = items.size();
    items.erase(
        remove_if(items.begin(), items.end(),
            [&](const Item& it) {
                return find(ids.begin(), ids.end(), it.id) != ids.end();
            }),
        items.end()
    );
    size_t removed = before - items.size();

    saveData();  // Сохраняем изменения

    cout << "Удалено предметов: " << static_cast<int>(removed) << "\n"; // ИСПРАВЛЕНО
}

// Функции поиска
// Поиск героев
void searchHero() {
    if (heroes.empty()) {
        cout << "Героев нет!\n";
        return;
    }

    system("cls");
    cout << "ПОИСК ГЕРОЕВ\n\n";
    cout << "Выберите критерий поиска:\n";
    cout << "1. По имени\n";
    cout << "2. По классу\n";
    cout << "3. По уровню\n";
    cout << "0. Назад\n";
    cout << "> ";

    int choice = safeMenuChoice();
    bool found = false;

    if (choice == 0) return; // Назад

    system("cls"); // Очищаем меню выбора критерия

    switch (choice) {
    case 1: { // По имени
        char text[20];
        cout << "Введите имя или часть имени: ";
        cin.getline(text, sizeof(text));

        for (const Hero& h : heroes) {
            if (containsSubstringIgnoreCase(h.name, text)) {
                cout << h.id << ". " << h.name
                    << " (" << h.class_type
                    << ") Ур." << h.level
                    << " HP:" << h.health << endl;
                found = true;
            }
        }
        break;
    }

    case 2: { // По классу
        vector<string> classes;
        for (const Hero& h : heroes) {
            if (find(classes.begin(), classes.end(), h.class_type) == classes.end())
                classes.push_back(h.class_type);
        }

        if (classes.empty()) break;

        cout << "Доступные классы:\n";
        for (size_t i = 0; i < classes.size(); i++)
            cout << static_cast<int>(i + 1) << ". " << classes[i] << endl; // ИСПРАВЛЕНО

        int cid;
        cout << "Выберите класс (номер): ";
        cid = safeMenuChoice();

        if (cid < 1 || cid > static_cast<int>(classes.size())) { // ИСПРАВЛЕНО
            cout << "Неверный выбор!\n";
            return;
        }

        string sel = classes[cid - 1];
        for (const Hero& h : heroes) {
            if (equalsIgnoreCase(h.class_type, sel.c_str())) {
                cout << h.id << ". " << h.name
                    << " (" << h.class_type
                    << ") Ур." << h.level
                    << " HP:" << h.health << endl;
                found = true;
            }
        }
        break;
    }

    case 3: { // По уровню
        int minLvl = safeIntInput("Введите минимальный уровень: ");
        for (const Hero& h : heroes) {
            if (h.level >= minLvl) {
                cout << h.id << ". " << h.name
                    << " (" << h.class_type
                    << ") Ур." << h.level
                    << " HP:" << h.health << endl;
                found = true;
            }
        }
        break;
    }

    default:
        cout << "Неверный выбор!\n";
        return;
    }

    if (!found) cout << "Ничего не найдено!\n";
}


// Поиск предметов
void searchItem() {
    if (items.empty()) {
        cout << "Предметов нет!\n";
        return;
    }

    system("cls");
    cout << "ПОИСК ПРЕДМЕТОВ\n\n";
    cout << "Выберите критерий поиска:\n";
    cout << "1. По названию\n";
    cout << "2. По типу\n";
    cout << "3. По цене\n";
    cout << "0. Назад\n";
    cout << "> ";

    int choice = safeMenuChoice();
    bool found = false;

    if (choice == 0) return; // Назад

    system("cls"); // Очищаем меню выбора критерия

    switch (choice) {
    case 1: { // По названию
        char text[30];
        cout << "Введите название или часть названия: ";
        cin.getline(text, sizeof(text));

        for (const Item& it : items) {
            if (containsSubstringIgnoreCase(it.name, text)) {
                cout << it.id << ". " << it.name
                    << " (" << it.type
                    << ") Цена: " << it.price << endl;
                found = true;
            }
        }
        break;
    }

    case 2: { // По типу
        vector<string> types;
        for (const Item& it : items) {
            if (find(types.begin(), types.end(), it.type) == types.end())
                types.push_back(it.type);
        }

        if (types.empty()) break;

        cout << "Доступные типы:\n";
        for (size_t i = 0; i < types.size(); i++)
            cout << static_cast<int>(i + 1) << ". " << types[i] << endl; // ИСПРАВЛЕНО

        int tid;
        cout << "Выберите тип (номер): ";
        tid = safeMenuChoice();

        if (tid < 1 || tid > static_cast<int>(types.size())) { // ИСПРАВЛЕНО
            cout << "Неверный выбор!\n";
            return;
        }

        string sel = types[tid - 1];
        for (const Item& it : items) {
            if (equalsIgnoreCase(it.type, sel.c_str())) {
                cout << it.id << ". " << it.name
                    << " (" << it.type
                    << ") Цена: " << it.price << endl;
                found = true;
            }
        }
        break;
    }

    case 3: { // По цене
        int maxPrice = safeIntInput("Введите максимальную цену: ");
        for (const Item& it : items) {
            if (it.price <= maxPrice) {
                cout << it.id << ". " << it.name
                    << " (" << it.type
                    << ") Цена: " << it.price << endl;
                found = true;
            }
        }
        break;
    }

    default:
        cout << "Неверный выбор!\n";
        return;
    }

    if (!found) cout << "Ничего не найдено!\n";
}

// Функции статистики
// Вывод статистики по данным
void showStats() {
    system("cls");
    cout << "СТАТИСТИКА\n\n";

    // 1. Общее количество записей
    cout << "1. Всего записей: " << (heroes.size() + items.size()) << endl;
    cout << "   - Героев: " << heroes.size() << endl;
    cout << "   - Предметов: " << items.size() << endl;

    // 2. Средний уровень героев (если есть герои)
    if (!heroes.empty()) {
        int sum = 0;
        for (const Hero& h : heroes) {
            sum += h.level;                   // Суммируем уровни всех героев
        }
        cout << "2. Средний уровень: " << (sum / static_cast<int>(heroes.size())) << endl; // ИСПРАВЛЕНО
    }

    // 3. Общая стоимость всех предметов (если есть предметы)
    if (!items.empty()) {
        int sum = 0;
        for (const Item& it : items) {
            sum += it.price;                  // Суммируем цены всех предметов
        }
        cout << "3. Общая цена: " << sum << endl;
    }
}

// Функции сортировки с улучшенной структурой меню

// Подменю сортировки героев по имени
void sortHeroesByNameMenu() {
    system("cls");
    cout << "СОРТИРОВКА ГЕРОЕВ ПО ИМЕНИ\n\n";
    cout << "1. От А до Я (A-Z)\n";
    cout << "2. От Я до А (Z-A)\n";
    cout << "0. Назад\n";
    cout << "Выбор: ";

    int choice = safeIntInput("");

    if (choice == 0) return;

    switch (choice) {
    case 1:
        sort(heroes.begin(), heroes.end(), [](const Hero& a, const Hero& b) {
            return strcmp(a.name, b.name) < 0;
            });
        cout << "Герои отсортированы по имени от А до Я!\n";
        break;
    case 2:
        sort(heroes.begin(), heroes.end(), [](const Hero& a, const Hero& b) {
            return strcmp(a.name, b.name) > 0;
            });
        cout << "Герои отсортированы по имени от Я до А!\n";
        break;
    default:
        cout << "Неверный выбор!\n";
        return;
    }

    saveData();
    showHeroes();
}

// Подменю сортировки героев по уровню
void sortHeroesByLevelMenu() {
    system("cls");
    cout << "СОРТИРОВКА ГЕРОЕВ ПО УРОВНЮ\n\n";
    cout << "1. По возрастанию\n";
    cout << "2. По убыванию\n";
    cout << "0. Назад\n";
    cout << "Выбор: ";

    int choice = safeIntInput("");

    if (choice == 0) return;

    switch (choice) {
    case 1:
        sort(heroes.begin(), heroes.end(), [](const Hero& a, const Hero& b) {
            return a.level < b.level;
            });
        cout << "Герои отсортированы по уровню по возрастанию!\n";
        break;
    case 2:
        sort(heroes.begin(), heroes.end(), [](const Hero& a, const Hero& b) {
            return a.level > b.level;
            });
        cout << "Герои отсортированы по уровню по убыванию!\n";
        break;
    default:
        cout << "Неверный выбор!\n";
        return;
    }

    saveData();
    showHeroes();
}

// Подменю сортировки героев по здоровью
void sortHeroesByHealthMenu() {
    system("cls");
    cout << "СОРТИРОВКА ГЕРОЕВ ПО ЗДОРОВЬЮ\n\n";
    cout << "1. По возрастанию\n";
    cout << "2. По убыванию\n";
    cout << "0. Назад\n";
    cout << "Выбор: ";

    int choice = safeIntInput("");

    if (choice == 0) return;

    switch (choice) {
    case 1:
        sort(heroes.begin(), heroes.end(), [](const Hero& a, const Hero& b) {
            return a.health < b.health;
            });
        cout << "Герои отсортированы по здоровью по возрастанию!\n";
        break;
    case 2:
        sort(heroes.begin(), heroes.end(), [](const Hero& a, const Hero& b) {
            return a.health > b.health;
            });
        cout << "Герои отсортированы по здоровью по убыванию!\n";
        break;
    default:
        cout << "Неверный выбор!\n";
        return;
    }

    saveData();
    showHeroes();
}

// Главное меню сортировки героев
void sortHeroesMenu() {
    while (true) {
        system("cls");
        cout << "СОРТИРОВКА ГЕРОЕВ\n\n";
        cout << "1. По имени\n";
        cout << "2. По уровню\n";
        cout << "3. По здоровью\n";
        cout << "0. Назад\n";
        cout << "Выбор: ";

        int choice = safeIntInput("");

        if (choice == 0) return;

        switch (choice) {
        case 1:
            sortHeroesByNameMenu();
            break;
        case 2:
            sortHeroesByLevelMenu();
            break;
        case 3:
            sortHeroesByHealthMenu();
            break;
        default:
            cout << "Неверный выбор!\n";
            break;
        }

        if (choice >= 1 && choice <= 3) {
            pause();
        }
    }
}

// Подменю сортировки предметов по названию
void sortItemsByNameMenu() {
    system("cls");
    cout << "СОРТИРОВКА ПРЕДМЕТОВ ПО НАЗВАНИЮ\n\n";
    cout << "1. От А до Я (A-Z)\n";
    cout << "2. От Я до А (Z-A)\n";
    cout << "0. Назад\n";
    cout << "Выбор: ";

    int choice = safeIntInput("");

    if (choice == 0) return;

    switch (choice) {
    case 1:
        sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return strcmp(a.name, b.name) < 0;
            });
        cout << "Предметы отсортированы по названию от А до Я!\n";
        break;
    case 2:
        sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return strcmp(a.name, b.name) > 0;
            });
        cout << "Предметы отсортированы по названию от Я до А!\n";
        break;
    default:
        cout << "Неверный выбор!\n";
        return;
    }

    saveData();
    showItems();
}

// Подменю сортировки предметов по цене
void sortItemsByPriceMenu() {
    system("cls");
    cout << "СОРТИРОВКА ПРЕДМЕТОВ ПО ЦЕНЕ\n\n";
    cout << "1. По возрастанию (дешевые -> дорогие)\n";
    cout << "2. По убыванию (дорогие -> дешевые)\n";
    cout << "0. Назад\n";
    cout << "Выбор: ";

    int choice = safeIntInput("");

    if (choice == 0) return;

    switch (choice) {
    case 1:
        sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.price < b.price;
            });
        cout << "Предметы отсортированы по цене по возрастанию!\n";
        break;
    case 2:
        sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.price > b.price;
            });
        cout << "Предметы отсортированы по цене по убыванию!\n";
        break;
    default:
        cout << "Неверный выбор!\n";
        return;
    }

    saveData();
    showItems();
}

// Главное меню сортировки предметов
void sortItemsMenu() {
    while (true) {
        system("cls");
        cout << "СОРТИРОВКА ПРЕДМЕТОВ\n\n";
        cout << "1. По названию\n";
        cout << "2. По цене\n";
        cout << "0. Назад\n";
        cout << "Выбор: ";

        int choice = safeIntInput("");

        if (choice == 0) return;

        switch (choice) {
        case 1:
            sortItemsByNameMenu();
            break;
        case 2:
            sortItemsByPriceMenu();
            break;
        default:
            cout << "Неверный выбор!\n";
            break;
        }

        if (choice >= 1 && choice <= 2) {
            pause();
        }
    }
}

// Вспомогательные функции
// Пауза с ожиданием нажатия Enter
void pause() {
    cout << "\nНажмите Enter для продолжения...";
    cin.clear();          // сброс ошибок на всякий случай
    cin.get();            // ждём Enter
}

// Безопасный выбор меню
int safeMenuChoice() {
    int choice;
    while (true) {
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка! Введите число: ";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
    }
}

void chooseHeroClass(char* class_type, int size) {
    int choice;

    while (true) {
        cout << "\nВыберите класс:\n";
        cout << "1. Лучник\n";
        cout << "2. Воин\n";
        cout << "3. Маг\n";
        cout << "> ";

        choice = safeMenuChoice();

        if (choice == 1) {
            strcpy_s(class_type, size, "Лучник");
            break;
        }
        else if (choice == 2) {
            strcpy_s(class_type, size, "Воин");
            break;
        }
        else if (choice == 3) {
            strcpy_s(class_type, size, "Маг");
            break;
        }
        else {
            cout << "Неправильный выбор. Попробуйте еще раз.\n";
        }
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    system("chcp 1251");
    loadData();                    // Загружаем данные при запуске программы

    // Главный цикл программы
    while (true) {
        system("cls");
        cout << "::::::АРСЕНАЛ ГЕРОЕВ::::::\n\n";
        cout << "1. Герои\n";
        cout << "2. Предметы\n";
        cout << "3. Поиск\n";
        cout << "4. Статистика\n";
        cout << "5. Сортировка\n";
        cout << "0. Выход\n";
        cout << "> ";

        int choice = safeMenuChoice();

        if (choice == 0) break;  // Выход из программы

        // Обработка выбора пользователя
        switch (choice) {
        case 1: {                  // Меню героев
            system("cls");
            cout << "ГЕРОИ\n\n";
            cout << "1. Добавить\n";
            cout << "2. Показать\n";
            cout << "3. Удалить одного\n";
            cout << "4. Удалить несколько\n";
            cout << "0. Назад\n";
            cout << "> ";
            choice = safeMenuChoice();

            // Вызов соответствующих функций
            if (choice == 0) break;
            else if (choice == 1) addHero();
            else if (choice == 2) showHeroes();
            else if (choice == 3) deleteHero();
            else if (choice == 4) deleteMultipleHeroes();
            else cout << "Неверный выбор!\n";
            break;
        }
        case 2: { // Меню предметов
            system("cls");
            cout << "ПРЕДМЕТЫ\n\n";
            // Подменю для работы с предметами
            cout << "1. Добавить\n";
            cout << "2. Показать\n";
            cout << "3. Удалить одного\n";
            cout << "4. Удалить несколько\n";
            cout << "0. Назад\n";
            cout << "> ";
            choice = safeMenuChoice();

            // Вызов соответствующих функций
            if (choice == 0) break;
            else if (choice == 1) addItem();
            else if (choice == 2) showItems();
            else if (choice == 3) deleteItem();
            else if (choice == 4) deleteMultipleItems();
            else cout << "Неверный выбор!\n";
            break;
        }
        case 3: { // Меню поиска
            system("cls");
            cout << "ПОИСК\n\n";
            // Выбор типа поиска
            cout << "1. Героев\n";
            cout << "2. Предметов\n";
            cout << "0. Назад\n";
            cout << "> ";
            choice = safeMenuChoice();

            // Вызов соответствующих функций
            if (choice == 0) break;
            else if (choice == 1) searchHero();
            else if (choice == 2) searchItem();
            else cout << "Неверный выбор!\n";
            break;
        }
        case 4: // Статистика
            showStats();
            break;
        case 5: { // Меню сортировки
            system("cls");
            cout << "СОРТИРОВКА\n\n";
            cout << "1. Героев\n";
            cout << "2. Предметов\n";
            cout << "0. Назад\n";
            cout << "> ";
            choice = safeMenuChoice();

            // Вызов соответствующих функций
            if (choice == 0) break;
            else if (choice == 1) sortHeroesMenu();
            else if (choice == 2) sortItemsMenu();
            else cout << "Неверный выбор!\n";
            break;
        }
        default:
            cout << "Неверный выбор!\n";
            break;
        }

        if (choice != 0) pause();             // Пауза после выполнения действия
    }

    saveData();                            // Сохраняем данные перед выходом
    cout << "\nСохранено. Выход.\n";
    return 0;
}