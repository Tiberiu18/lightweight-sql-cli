#include "../Headers/Menu.h"

Menu1::Menu1(Database& db) : db(db) {}

void Menu1::display() {
    std::cout << "\nTables are: " << std::endl;
    for (auto itr = db.tables.begin(); itr != db.tables.end(); ++itr) {
        std::cout << (*itr)->getTableName() << std::endl;
    }
    std::cout << std::endl;
}

void Menu1::displayMenuType() {
    std::cout << "\nMenu is: Menu1\n";
}

Menu2::Menu2(Database& db) : Menu1(db) {}

void Menu2::calc() {
    std::string table_name;
    int nr_rows = 0;
    for (auto itr = db.tables.begin(); itr != db.tables.end(); ++itr) {
        table_name = (*itr)->getTableName();
        output[table_name] = 0;
        for (int j = 0; j < (*itr)->getNrCols(); j++) {
            nr_rows = (*itr)->getColsArr()[j]->getRows();
            if (output.find(table_name) != output.end())
                output[table_name] += nr_rows;
        }
    }
}

void Menu2::display() {
    calc();
    std::cout << std::endl;
    for (auto itr = output.begin(); itr != output.end(); ++itr) {
        std::cout << "Table " << itr->first << " has " << itr->second << " rows." << std::endl;
    }
    if (output.size() == 0)
        std::cout << " Nu exista niciun tabel in sistem.\n";
}

void Menu2::displayMenuType() {
    std::cout << "\nMenu is: Menu2\n ";
}

Menu3::Menu3(Database& db) : Menu2(db) {}

void Menu3::calc() {
    total_MEM = 0;
    std::string table_name;
    for (auto itr = db.tables.begin(); itr != db.tables.end(); ++itr) {
        table_name = (*itr)->getTableName();
        output[table_name] = static_cast<int>((*itr)->calcSize());
        total_MEM += output[table_name];
    }
}

void Menu3::display() {
    calc();
    std::cout << std::endl;
    if (output.size() == 0) {
        std::cout << " Nu exista niciun tabel in sistem.\n";
    } else {
        for (auto itr = output.begin(); itr != output.end(); ++itr) {
            std::cout << "Table " << itr->first << " is " << itr->second << " bytes large." << std::endl;
        }
        std::cout << "Total memory occupied: " << total_MEM << std::endl << std::endl;
    }
}
