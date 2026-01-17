#ifndef COFFEE_MACHINE_H
#define COFFEE_MACHINE_H

#include "ingredients.h"
#include "recipes.h"
#include "parser.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>

// class Recipe;

// Класс кофемолка
class CoffeeGrinder
{
public:
    void grind(CoffeeBeans &c); // помол
};

// Класс "группа"
class Group
{
public:
    void boil(CoffeeBeans &c); // варкa
};

// Класс бойлер
class Boiler
{
public:
    void heat(Water &w); // нагрев
};

// Класс форсунок
class SteamNozzle
{
public:
    void steam(WetIngredient &ingredient);        // нагрев
    void mix(DryIngredient &ingredient);          // размешать
    void add(Ingredient &ingredient, int amount); // добавить
};

// Класс кофемашины целиком
class CoffeeMachine
{
private:
    CoffeeGrinder grinder;
    Group group;
    Boiler boiler;
    SteamNozzle nozzle;

public:
    int prepareCoffee(Recipe &r);              // "приготовить кофе" (узнать время)
    int AddDobavkiCoffee(std::string &stroka); // добавка к кофе
};

#endif // COFFEE_MACHINE_H
