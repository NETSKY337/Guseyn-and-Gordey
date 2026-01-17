#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cstring>
using namespace std;

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

// Функции для работы с файлами
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

// Функции для работы с героями
// Добавление нового героя
void addHero() {
    system("cls");                       // Очищаем консоль
    cout << "ДОБАВЛЕНИЕ ГЕРОЯ\n\n";

    Hero h;
    h.id = nextHeroId++;                 // Присваиваем следующий доступный ID и увеличиваем счетчик

    // Ввод данных о герое
    cout << "Имя: ";
    cin >> h.name;

    cout << "Класс: ";
    cin >> h.class_type;

    cout << "Уровень: ";
    cin >> h.level;

    cout << "Здоровье: ";
    cin >> h.health;

    heroes.push_back(h);                 // Добавляем героя в вектор
    saveData();                          // Сохраняем данные в файл
    cout << "\nГерой добавлен!\n";
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

    int id;
    cout << "\nID для удаления: ";
    cin >> id;

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
    showHeroes();                                   // Сначала показываем список героев

    if (heroes.empty()) return;                     // Если нет героев, выходим

    cout << "\nВведите ID героев для удаления (через пробел): ";
    cin.ignore();                                   // Очищаем буфер ввода
    string input;
    getline(cin, input);                            // Читаем всю строку с ID

    stringstream ss(input);                         // Создаем поток для разбора строки
    vector<int> ids;                                // Вектор для хранения ID
    int id;

    // Разбираем строку на отдельные ID
    while (ss >> id) {
        ids.push_back(id);
    }

    int removed = 0;                                // Счетчик удаленных героев

    // Удаляем героев по списку ID
    for (int id : ids) {
        for (auto it = heroes.begin(); it != heroes.end(); ++it) {
            if (it->id == id) {
                heroes.erase(it);               // Удаляем героя
                removed++;                      // Увеличиваем счетчик
                break;                          // Прерываем внутренний цикл
            }
        }
    }

    saveData();                                         // Сохраняем изменения
    cout << "Удалено героев: " << removed << "\n";      // Выводим результат
}

// Функции для работы с предметами
// Добавление нового предмета
void addItem() {
    system("cls");
    cout << "ДОБАВЛЕНИЕ ПРЕДМЕТА\n\n";

    Item it;
    it.id = nextItemId++;                          // Присваиваем следующий доступный ID и увеличиваем счетчик

    // Ввод данных о предмете
    cout << "Название: ";
    cin >> it.name;

    cout << "Тип: ";
    cin >> it.type;

    cout << "Цена: ";
    cin >> it.price;

    items.push_back(it);                   // Добавляем предмет в вектор
    saveData();                            // Сохраняем данные в файл
    cout << "\nПредмет добавлен!\n";
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

    int id;
    cout << "\nID для удаления: ";
    cin >> id;

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
    showItems();                                  // Сначала показываем список предметов

    if (items.empty()) return;                    // Если нет предметов, выходим

    cout << "\nВведите ID предметов для удаления (через пробел): ";
    cin.ignore();                                 // Очищаем буфер ввода
    string input;
    getline(cin, input);                          // Читаем всю строку с ID

    stringstream ss(input);                    // Создаем поток для разбора строки
    vector<int> ids;                           // Вектор для хранения ID
    int id;

    // Разбираем строку на отдельные ID
    while (ss >> id) {
        ids.push_back(id);
    }

    int removed = 0;                          // Счетчик удаленных предметов

    // Удаляем предметы по списку ID
    for (int id : ids) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->id == id) {
                items.erase(it);               // Удаляем предмет
                removed++;                     // Увеличиваем счетчик
                break;                         // Прерываем внутренний цикл
            }
        }
    }

    saveData();                      // Сохраняем изменения
    cout << "Удалено предметов: " << removed << "\n";       // Выводим результат
}

// Функции поиска
// Поиск героев по различным критериям
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

    // Выполняем поиск в зависимости от выбранного критерия
    switch (choice) {
    case 1:                                     // Поиск по имени (частичному совпадению)
        cout << "Имя: ";
        cin >> text;
        for (const Hero& h : heroes) {
            if (strstr(h.name, text)) {         // Используем strstr для поиска подстроки
                cout << h.id << ". " << h.name << endl;
            }
        }
        break;
    case 2:                                                 // Поиск по точному совпадению класса
        cout << "Класс: ";
        cin >> text;
        for (const Hero& h : heroes) {
            if (strcmp(h.class_type, text) == 0) {          // Используем strcmp для точного сравнения
                cout << h.id << ". " << h.name << endl;
            }
        }
        break;
    case 3:                                // Поиск по минимальному уровню
        cout << "Мин. уровень: ";
        cin >> num;
        for (const Hero& h : heroes) {
            if (h.level >= num) {              // Уровень должен быть больше или равен
                cout << h.id << ". " << h.name << endl;
            }
        }
        break;
    }
}

// Поиск предметов по различным критериям
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

    // Выполняем поиск в зависимости от выбранного критерия
    switch (choice) {
    case 1:                            // Поиск по названию (частичному совпадению)
        cout << "Название: ";
        cin >> text;
        for (const Item& it : items) {
            if (strstr(it.name, text)) {                     // Используем strstr для поиска подстроки
                cout << it.id << ". " << it.name << endl;
            }
        }
        break;
    case 2:                      // Поиск по точному совпадению типа
        cout << "Тип: ";
        cin >> text;
        for (const Item& it : items) {
            if (strcmp(it.type, text) == 0) {                 // Используем strcmp для точного сравнения
                cout << it.id << ". " << it.name << endl;
            }
        }
        break;
    case 3:                           // Поиск по максимальной цене
        cout << "Макс. цена: ";
        cin >> num;
        for (const Item& it : items) {
            if (it.price <= num) {              // Цена должна быть меньше или равна
                cout << it.id << ". " << it.name << endl;
            }
        }
        break;
    }
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
        cout << "2. Средний уровень: " << (sum / heroes.size()) << endl;
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

// Функции сортировки
// Сортировка героев по выбранному критерию
void sortHeroes() {
    system("cls");
    cout << "СОРТИРОВКА ГЕРОЕВ\n\n";
    cout << "1. По имени (А-Я)\n";
    cout << "2. По уровню (высокий)\n";
    cout << "Выбор: ";

    int choice;
    cin >> choice;

    // Сортировка в зависимости от выбранного критерия
    if (choice == 1) {
        // Сортировка по имени в алфавитном порядке
        sort(heroes.begin(), heroes.end(), [](const Hero& a, const Hero& b) {
            return strcmp(a.name, b.name) < 0;  // Сравниваем имена
            });
    }
    else if (choice == 2) {
        // Сортировка по уровню в порядке убывания
        sort(heroes.begin(), heroes.end(), [](const Hero& a, const Hero& b) {
            return a.level > b.level;             // Более высокий уровень идет первым
            });
    }

    saveData();        // Сохраняем отсортированные данные
    cout << "Сортировка завершена!\n";
}

// Сортировка предметов по выбранному критерию
void sortItems() {
    system("cls");
    cout << "СОРТИРОВКА ПРЕДМЕТОВ\n\n";
    cout << "1. По названию (А-Я)\n";
    cout << "2. По цене (дорогой)\n";
    cout << "Выбор: ";

    int choice;
    cin >> choice;

    // Сортировка в зависимости от выбранного критерия
    if (choice == 1) {
        // Сортировка по названию в алфавитном порядке
        sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return strcmp(a.name, b.name) < 0;  // Сравниваем названия
            });
    }
    else if (choice == 2) {
        // Сортировка по цене в порядке убывания
        sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.price > b.price;  // Более дорогие предметы идут первыми
            });
    }

    saveData();  // Сохраняем отсортированные данные
    cout << "Сортировка завершена!\n";
}

// Вспомогательные функции
// Пауза с ожиданием нажатия Enter
void pause() {
    cout << "\nНажмите Enter для продолжения...";
    cin.ignore();          // Очищаем буфер ввода
    cin.get();          // Ждем нажатия Enter
}

int main() {
    setlocale(LC_ALL, "RU");
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

        int choice;
        cin >> choice;

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
            cout << "> ";
            cin >> choice;

            // Вызов соответствующих функций
            if (choice == 1) addHero();
            else if (choice == 2) showHeroes();
            else if (choice == 3) deleteHero();
            else if (choice == 4) deleteMultipleHeroes();
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
            cout << "> ";
            cin >> choice;

            // Вызов соответствующих функций
            if (choice == 1) addItem();
            else if (choice == 2) showItems();
            else if (choice == 3) deleteItem();
            else if (choice == 4) deleteMultipleItems();
            break;
        }
        case 3: { // Меню поиска
            system("cls");
            cout << "ПОИСК\n\n";
            // Выбор типа поиска
            cout << "1. Героев\n";
            cout << "2. Предметов\n";
            cout << "> ";
            cin >> choice;

            // Вызов соответствующих функций
            if (choice == 1) searchHero();
            else if (choice == 2) searchItem();
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
            cout << "> ";
            cin >> choice;

            // Вызов соответствующих функций
            if (choice == 1) sortHeroes();
            else if (choice == 2) sortItems();
            break;
        }
        }

        if (choice != 0) pause();             // Пауза после выполнения действия
    }

    saveData();                            // Сохраняем данные перед выходом
    cout << "\nСохранено. Выход.\n";
    return 0;
}