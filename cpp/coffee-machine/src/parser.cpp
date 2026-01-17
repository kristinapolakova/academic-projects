#include "parser.h"

void TextRecipeParser::parse(std::string filename, std::vector<Recipe> &r)
{
    std::ifstream file;
    file.open(filename);

    std::string line;

    while (std::getline(file, line))
    {
        Recipe recipe;
        std::string name;
        std::map<std::string, unsigned> ingredients;

        for (auto li : line)
        {
            if (li == ':')
                break;
            name += li;
        }
        recipe.setName(name);

        line = line.substr(line.find(':') + 1, line.size());
        std::string name_ing;
        std::string count_ing;
        std::map<std::string, unsigned> ing;
        int count = std::count(line.begin(), line.end(), ',');
        for (int i = 0; i < count + 1; i++)
        {
            name_ing = line.substr(0, line.find('('));
            line = line.substr(line.find('(') + 1, line.size());
            count_ing = line.substr(0, line.find(')'));
            int n = std::stoi(count_ing);
            ing[name_ing] = n;
            if (i < count)
            {
                line = line.substr(line.find(')') + 2, line.size());
            }
        }
        recipe.setIngredients(ing);
        r.push_back(recipe);
    }
    file.close();
}