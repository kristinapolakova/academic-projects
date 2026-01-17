#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <stdexcept>

#include "recipes.h"
#include "coffee_machine.h"

class Print
{
public:
    void printMap(std::map<std::string, unsigned> ingredients);
    void printVector(std::vector<Recipe> r);
};

class Menu
{
public:
    int numberName();
    std::string searchName(std::vector<Recipe> &r, int &input);
    void start(std::vector<Recipe> r, CoffeeMachine &cm);
};

#endif