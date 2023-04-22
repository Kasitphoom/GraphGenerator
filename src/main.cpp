#include<iostream>
#include<string>
#include <vector>

struct data{
    std::string name;
    double value;
};

class Category{
    private:
        std::string name;
        double value;
};

class BarGraph{
    private:
        std::vector<Category> data;
};
