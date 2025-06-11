#ifndef APPLICATION_H
#define APPLICATION_H

#include "Database.h"
#include "Phase2.h"
#include "Menu.h"
#include "GetCmdType.h"
#include <memory>
#include <string>

class Application {
public:
    Application();
    void run();
private:
    Database db;
    FileHandling fileHandler;
    std::unique_ptr<Menu1> menu1;
    std::unique_ptr<Menu2> menu2;
    std::unique_ptr<Menu3> menu3;

    void executeCommand(const std::string& input);
    std::string readCommand(const std::string& prompt);
};

#endif // APPLICATION_H
