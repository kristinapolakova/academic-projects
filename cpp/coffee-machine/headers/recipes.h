#ifndef RECIPES_H
#define RECIPES_H

#include <map>
#include <string>

// Класс рецепта
class Recipe
{
private:
    std::string name;
    std::map<std::string, unsigned> ingredients;

public:
    void setName(std::string n);
    std::string getName() const;
    void setIngredients(std::map<std::string, unsigned> &ing);
    std::map<std::string, unsigned> getIngredients() const;
};

#endif // RECIPES_H
