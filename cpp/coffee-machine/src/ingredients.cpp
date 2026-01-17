#include "ingredients.h"

void Ingredient::setTemperature(int t)
{
    temperature = t;
}

int Ingredient::getTemperature() const
{
    return temperature;
}

void Ingredient::setAmount(int a)
{
    amount = a;
}

int Ingredient::getAmount() const
{
    return amount;
}

void Ingredient::setTime(int t)
{
    time = t;
}

int Ingredient::getTime() const
{
    return time; // Возвращаем время, которое требуется ингредиенту
}

void CoffeeBeans::setStatus(bool s)
{
    is_grinded = s;
}

bool CoffeeBeans::getStatus() const
{
    return is_grinded;
}

int CoffeeBeans::getTime() const
{
    return time; // Возвращаем время, необходимое для кофейных зерен
}

// Реализации getTime для других классов (WetIngredient и DryIngredient)
int WetIngredient::getTime() const
{
    return time;
}

int DryIngredient::getTime() const
{
    return time; // Аналогично
}