#include "coffee_machine.h"

void CoffeeGrinder::grind(CoffeeBeans &c)
{
    int time = 14 + rand() % (19 - 14 + 1);
    c.setTime(c.getTime() + time);
    c.setStatus(true);
}

void Group::boil(CoffeeBeans &c)
{
    if (c.getStatus())
    {
        int time = 19 + rand() % (24 - 19 + 1);
        c.setTime(c.getTime() + time);
    }
    else
    {
        return;
    }
}

void Boiler::heat(Water &w)
{
    int toAdd = 100 - w.getTemperature();
    w.setTemperature(w.getTemperature() + toAdd);
    w.setTime(w.getTime() + toAdd);
}

void SteamNozzle::steam(WetIngredient &wi)
{
    int toAdd = (65 + rand() % (70 - 65 + 1)) - wi.getTemperature();
    wi.setTemperature(wi.getTemperature() + toAdd);
    wi.setTime(wi.getTime() + toAdd);
}

void SteamNozzle::mix(DryIngredient &di)
{
    di.setTime(di.getTime() + 10);
}

void SteamNozzle::add(Ingredient &ingredient, int amount)
{
    //
}

int CoffeeMachine::prepareCoffee(Recipe &r)
{
    r.getIngredients();
    int allTime = 0;
    for (auto ingredient : r.getIngredients())
    {
        if (ingredient.first == "молоко" || ingredient.first == "сливки")
        {
            srand(time(NULL));
            for (int i = 0; i < ingredient.second; i++)
            {
                Milk milk;
                nozzle.steam(milk);
                allTime += milk.getTime();
            }
        }
        else if (ingredient.first == "кофе")
        {
            srand(time(NULL));
            for (int i = 0; i < ingredient.second; i++)
            {
                CoffeeBeans cb;
                grinder.grind(cb);
                group.boil(cb);
                allTime += cb.getTime();
            }
        }
        else if (ingredient.first == "вода")
        {
            for (int i = 0; i < ingredient.second; i++)
            {
                Water w;
                boiler.heat(w);
                allTime += w.getTime();
            }
        }
    }
    return allTime;
}

int CoffeeMachine::AddDobavkiCoffee(std::string &stroka)
{
    std::istringstream iss(stroka);
    std::string ingredient;
    std::vector<DryIngredient *> ingredients; // Вектор для хранения добавок
    int totalTime = 0;

    // Разделяем строку по ингредиентам
    while (iss >> ingredient)
    {
        if (ingredient == "1")
        { // Корица
            ingredients.push_back(new Koriza());
        }
        else if (ingredient == "2")
        { // Сухой сироп
            ingredients.push_back(new Sirop());
        }
        else if (ingredient == "3")
        { // Бадьян
            ingredients.push_back(new Badyan());
        }
    }

    // Считаем общее время добавок
    for (const auto &ing : ingredients)
    {
        totalTime += ing->getTime();
        delete ing; // Освобождаем память после использования
    }

    return totalTime;
}