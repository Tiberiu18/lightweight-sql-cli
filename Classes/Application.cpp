#include "../Headers/Application.h"
#include "../utils/utils.h"
#include <fstream>
#include <iostream>

Application::Application()
    : db(1), fileHandler(db)
{
    menu1 = std::make_unique<Menu1>(db);
    menu2 = std::make_unique<Menu2>(db);
    menu3 = std::make_unique<Menu3>(db);
    fileHandler.createExistingTables();
    if (db.getNrTables() > 0)
        std::cout << db << std::endl;
}

std::string Application::readCommand(const std::string& prompt)
{
    std::string cmd;
    std::cout << prompt;
    std::getline(std::cin, cmd);
    Utility::toUpper(cmd);
    return cmd;
}

void Application::executeCommand(const std::string& input)
{
    std::string firstWord = input.substr(0, input.find(" "));
    getCmdType g1(input, "HELPER");
    if (firstWord == "CREATE") {
        if (g1.verifyCreateTable()) {
            std::string tableName = "TABEL_" + g1.res2[4];
            std::string path = "binaryfiles/";
            if (!Utility::file_exists(path + tableName)) {
                try {
                    Table* p = g1.createCols();
                    db.addTable(p);
                    std::fstream f(path + tableName, std::ios::out);
                    fileHandler.writeToFile(f, db.getNrTables() - 1);
                    f.close();
                    std::cout << db << std::endl;
                } catch (std::exception& ex) {
                    std::cout << ex.what();
                }
            } else {
                std::cout << " Tabelul deja exista!\n";
            }
        } else {
            std::cout << " Sintaxa incorecta pentru CREATE TABLE.\n";
        }
    } else if (firstWord == "DISPLAY") {
        DisplayHandling d(db);
        g1.displayTable(db);
        d.writeToFile(g1.res2[4]);
    } else if (firstWord == "DROP") {
        DropHandling d(g1);
        g1.dropTable(db);
        d.deleteTable();
    } else if (firstWord == "INSERT" || firstWord == "IMPORT") {
        InsertHandling i1(db);
        if (firstWord == "INSERT") {
            bool ok = Utility::checkParenthesisInsert(input);
            bool ok2 = g1.InsertVerify(db);
            if (ok && ok2) {
                std::string copy = input;
                std::string res = Utility::formStringInsert(copy);
                std::string* arr = Utility::regexInsert(res);
                g1.insertInto(db, arr);
                int t_POS = g1.return_pos;
                if (t_POS >= 0) {
                    std::fstream f("TABEL_" + g1.res2[4], std::ios::out);
                    i1.writeToFile(f, g1.res2[4]);
                    f.close();
                }
                delete[] arr;
            } else {
                std::cout << "Sintaxa incorecta pentru INSERT INTO." << std::endl;
            }
        } else {
            std::string name_tbl = g1.res2[2];
            std::string file_name = g1.res2[4];
            bool modifiedTable = fileHandler.ImportFrom(file_name, name_tbl);
            if (modifiedTable) {
                std::fstream f("TABEL_" + g1.res2[2], std::ios::out);
                i1.writeToFile(f, g1.res2[2]);
                f.close();
            }
        }
    } else if (firstWord == "SELECT") {
        DisplayHandling d(db);
        g1.selectALL(db);
        d.writeToFile(g1.res2[6]);
    } else if (firstWord == "DELETE") {
        InsertHandling i1(db);
        g1.deleteFrom(db);
        int t_POS = g1.return_pos;
        if (t_POS >= 0) {
            std::fstream f("TABEL_" + g1.res2[4], std::ios::out);
            i1.writeToFile(f, g1.res2[4]);
            f.close();
        }
    } else if (firstWord == "UPDATE") {
        InsertHandling i1(db);
        bool updated = g1.Update(db);
        if (updated) {
            std::fstream f("TABEL_" + g1.res2[2], std::ios::out);
            i1.writeToFile(f, g1.res2[2]);
            f.close();
        }
    } else if (firstWord == "MENU") {
        std::string inp;
        do {
            std::cout << "1) List all table names from the database.\n2)List all rows existing in the database\n3)List size of tables\n4)Exit\n";
            std::cin >> inp;
            std::cin.ignore();
            if (Table::isNumber(inp)) {
                int x = std::stof(inp);
                switch (x) {
                case 1:
                    menu1->display();
                    break;
                case 2:
                    menu2->display();
                    break;
                case 3:
                    menu3->display();
                    break;
                default:
                    inp = "";
                    break;
                }
            }
        } while (inp == "MENU");
    } else if (firstWord == "--HELP") {
        std::ifstream f("ReadMe.md");
        if (f.is_open())
            std::cout << f.rdbuf();
    } else if (firstWord == "--CLEAR") {
        for (int i = 0; i < 100; ++i)
            std::cout << std::endl;
    } else if (firstWord == "--SHOWDATABASE") {
        std::cout << db;
    } else {
        std::cout << "Comanda nerecunoscuta. Tastati --help pentru ajutor.\n";
    }
}

void Application::run()
{
    std::string input;
    while (true) {
        input = readCommand("");
        if (input == "EXIT")
            break;
        executeCommand(input);
    }
}

