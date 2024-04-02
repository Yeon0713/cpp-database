#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "database.h"

Entry *create(Type type, std::string key, void *value){
    Entry *entry = new Entry;
    entry->type = type;
    entry->key = key;
    if (type == INT) {
        entry->value = new int(*(int*)value);
    } 
    else if (type == DOUBLE) {
        entry->value = new double(*(double*)value);
    } 
    else {
        // 문자열의 경우는 그대로 저장합니다.
        entry->value = value;
    }
    return entry;
}


void init(Database &database){
    database.entries = new Entry[10];
    database.capacity = 10;
    database.size = 0;
}

void list(const Database &database){
    for (int i = 0; i < database.size; ++i) {
        std::cout << database.entries[i].key << ": ";
        if (database.entries[i].type == INT)
            std::cout << *((int *)database.entries[i].value) << "\n";
        else if (database.entries[i].type == DOUBLE)
            std::cout << *((double *)database.entries[i].value) << "\n";
        else if (database.entries[i].type == STRING)
            std::cout << (char *)database.entries[i].value << "\n";
        else if (database.entries[i].type == ARRAY) {
            Array *array = (Array *)database.entries[i].value;
            for (int j = 0; j < array->size; ++j) {
                if (array->type == INT)
                    std::cout << *((int *)(array->items) + j) << " ";
                else if (array->type == DOUBLE)
                    std::cout << *((double *)(array->items) + j) << " ";
                else if (array->type == STRING)
                    std::cout << *((char **)(array->items) + j) << " ";
            }
            std::cout << "\n";
        }
    }
}


void add(Database &database, Entry *entry){
    if (database.size >= database.capacity){
        Entry *temp = new Entry[database.capacity * 2];
        std::memcpy(temp, database.entries, sizeof(Entry) * database.capacity);
        delete[] database.entries;
        database.entries = temp;
        database.capacity *= 2;
    }
    database.entries[database.size++] = *entry;
}

Entry *get(Database &database, std::string key){
    for (int i = 0; i< database.size; ++i){
        if (database.entries[i].key == key) {
            return &database.entries[i];
        }
    }
    return nullptr;
}

void remove(Database &database, std::string key){
    for (int i = 0; i < database.size; ++i){
        if (database.entries[i].key == key){
            for (int j = i; j < database.size - 1; ++j){
                database.entries[j] = database.entries[j + 1];
            }
            --database.size;
            break;
        }
    }
}

void destroy(Database &database){
    delete[] database.entries;
}

int main() {
    Database mk_database;
    init(mk_database);

    std::string command;

    while (true) {
        std::string key = "";
        std::string type1 = "";
        std::string value_str = "";

        std::cout << "command (list, add, get, del, exit): ";
        std::cin >> command;

        if (command == "list") {
            list(mk_database);
        } 
        else if (command == "add") {
            std::cout << "key: ";
            std::cin >> key;

            std::cout << "type (int, double, string, array): ";
            std::cin >> type1;

            Type type;

            if (type1 == "int") {
                type = INT;
            } else if (type1 == "double") {
                type = DOUBLE;
            } else if (type1 == "string") {
                type = STRING;
            } else if (type1 == "array") {
                type = ARRAY;
            } else {
                std::cout << "Invalid input type.\n";
                continue;
            }

            std::cout << "value: ";
            std::cin.ignore();  // Clear the input buffer
            std::getline(std::cin, value_str);

            char *value_copy = new char[value_str.size() + 1]; // 문자열의 길이 + 널 종료 문자('\0')를 위한 공간 할당
            std::strcpy(value_copy, value_str.c_str()); // 문자열 복사
            Entry *entry = create(type, key, value_copy);
            add(mk_database, entry);

        } else if (command == "get") {
            std::cout << "key: ";
            std::cin >> key;
            Entry *entry = get(mk_database, key.c_str());
            if (entry != nullptr) {
                std::cout << entry->key << ": ";
                if (entry->type == INT)
                    std::cout << *((int *)entry->value) << "\n";
                else if (entry->type == DOUBLE)
                    std::cout << *((double *)entry->value) << "\n";
                else if (entry->type == STRING)
                    std::cout << (char *)entry->value << "\n";
                else if (entry->type == ARRAY) {
                    Array *array = (Array *)entry->value;
                    for (int j = 0; j < array->size; ++j) {
                        if (array->type == INT)
                            std::cout << *((int *)(array->items) + j) << " ";
                        else if (array->type == DOUBLE)
                            std::cout << *((double *)(array->items) + j) << " ";
                        else if (array->type == STRING)
                            std::cout << *((char **)(array->items) + j) << " ";
                    }
                    std::cout << "\n";
                }
            } 

        } else if (command == "del") {
            std::cout << "key: ";
            std::cin >> key;
            remove(mk_database, key.c_str());
        } else if (command == "exit") {
            break;
        } else {
            std::cout << "Invalid command.\n";
        }
    }

    destroy(mk_database);

    return 0;
}