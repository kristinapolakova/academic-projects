#include <iostream>
#include "coffee_machine.h"
#include "ingredients.h"
#include "recipes.h"
#include "parser.h"
#include "menu.h"

using namespace std;

int main()
{
    Menu menu;
    TextRecipeParser parser;
    CoffeeMachine cm;
    std::vector<Recipe> r;

    parser.parse("recipes.txt", r);
    menu.start(r, cm);

    return 0;
}
