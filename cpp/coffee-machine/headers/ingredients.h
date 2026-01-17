#ifndef INGREDIENTS_H
#define INGREDIENTS_H

class Ingredient
{
private:
    int temperature; // температура
    int amount;      // количество
public:
    Ingredient() : temperature(24), amount(0), time(0) {}
    int time; // время, которое они требуют
    void setTemperature(int t);
    int getTemperature() const;
    void setAmount(int a);
    int getAmount() const;
    void setTime(int t);
    virtual int getTime() const;
};

class WetIngredient : public Ingredient
{
public:
    virtual int getTime() const override; // Переопределите метод
};

class DryIngredient : public Ingredient
{
public:
    virtual int getTime() const override; // Переопределите метод
};

class CoffeeBeans : public DryIngredient
{
private:
    bool is_grinded;

public:
    CoffeeBeans() : is_grinded(false) {}
    void setStatus(bool s);
    bool getStatus() const;
    int getTime() const override; // Переопределите метод
};

class Sugar : public DryIngredient
{
public:
    int getTime() const override
    {
        return 5;
    } // Переопределите метод
};

class Koriza : public DryIngredient
{
public:
    int getTime() const override
    {
        return 5;
    } // Переопределите метод
};

class Sirop : public DryIngredient
{
public:
    int getTime() const override
    {
        return 7;
    } // Переопределите метод
};

class Badyan : public DryIngredient
{
public:
    int getTime() const override
    {
        return 10;
    } // Переопределите метод
};

class Milk : public WetIngredient
{
public:
    int getTime() const override
    {
        return 10;
    } // Переопределите метод
};

class Water : public WetIngredient
{
public:
    int getTime() const override
    {
        return 10;
    }
};

#endif // INGREDIENTS_H