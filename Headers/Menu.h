#pragma once
#include "Database.h"
#include <map>
#include <string>
#include <iostream>

class Menu1 {
protected:
    Database& db;
public:
    explicit Menu1(Database& db);
    virtual ~Menu1() = default;
    virtual void display();
    virtual void displayMenuType();
};

class Menu2 : public Menu1 {
protected:
    std::map<std::string, int> output;
    virtual void calc();
public:
    explicit Menu2(Database& db);
    void display() override;
    void displayMenuType() override;
};

class Menu3 : public Menu2 {
    int total_MEM = 0;
    void calc() override;
public:
    explicit Menu3(Database& db);
    void display() override;
};
