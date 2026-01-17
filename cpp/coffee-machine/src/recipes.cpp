#include "recipes.h"

void Recipe::setName(std::string n)
{
    name = n;
}

std::string Recipe::getName() const
{
    return name;
}

void Recipe::setIngredients(std::map<std::string, unsigned> &ing)
{
    ingredients = ing;
}

std::map<std::string, unsigned> Recipe::getIngredients() const
{
    return ingredients;
}
