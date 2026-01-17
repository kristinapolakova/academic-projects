#include "menu.h"

int Menu::numberName()
{
    int input;
    std::cout << "Введите номер напитка: ";
    std::cin >> input;
    return input;
}

std::string Menu::searchName(std::vector<Recipe> &r, int &input)
{
    return r[input - 1].getName();
}

void Print::printMap(std::map<std::string, unsigned> ingredients)
{
    for (auto ingredient : ingredients)
    {
        std::cout << ingredient.first << " " << ingredient.second << ", ";
    }
    std::cout << std::endl;
}

void Print::printVector(std::vector<Recipe> r)
{
    for (size_t i = 0; i < r.size(); i++)
    {
        std::cout << i + 1 << ". " << r[i].getName() << std::endl;
    }
}

void Menu::start(std::vector<Recipe> r, CoffeeMachine &cm)
{
    while (true)
    {
        std::cout << "+--------------+" << std::endl;
        for (size_t i = 0; i < r.size(); i++)
        {
            std::cout << "| " << i + 1 << ". " << r[i].getName() << std::endl;
        };
        std::cout << "+--------------+" << std::endl;

        std::cout << "Выберете напиток (q для выхода): ";
        std::string userInput;
        std::cin >> userInput;

        if (userInput == "q" || userInput == "Q")
        {
            std::cout << "Машина выключена." << std::endl;
            break;
        }

        int input;
        try
        {
            input = std::stoi(userInput);
        }
        catch (...)
        {
            std::cout << "Ошибка ввода!" << std::endl;
            continue;
        }

        if (input < 1 || input > static_cast<int>(r.size()))
        {
            std::cout << "Неверный номер!" << std::endl;
            continue;
        }

        std::string name = searchName(r, input);
        std::cout << "+------------------------+" << std::endl;
        std::cout << "| Вы выбрали: " << name << std::endl;

        int additionalTime = 0;
        while (true)
        {
            std::cout << "Добавить добавки? (корица - 1, сухой сироп - 2, бадьян - 3, 0 - пропустить, q - выход): ";
            std::string stroka;
            std::cin >> stroka;

            if (stroka == "0")
            {
                break;
            }
            else if (stroka == "q" || stroka == "Q")
            {
                std::cout << "Машина выключена." << std::endl;
                return;
            }

            additionalTime = cm.AddDobavkiCoffee(stroka);
            break;
        }

        int totalTime = cm.prepareCoffee(r[input - 1]) + additionalTime;
        std::cout << "| Время изготовления: " << totalTime << " сек" << std::endl;
        std::cout << "+------------------------+" << std::endl;

        std::string choice;
        while (true)
        {
            std::cout << "Продолжить (1 - да, q - выход)? ";
            std::cin >> choice;

            if (choice == "1" || choice == "q" || choice == "Q")
            {
                break;
            }
            else
            {
                std::cout << "Неверный ввод! Введите 1 или q." << std::endl;
            }
        }

        if (choice == "q" || choice == "Q")
        {
            std::cout << "Машина выключена." << std::endl;
            break;
        }
    }
}