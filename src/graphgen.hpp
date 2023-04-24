#ifndef GRAPHGEN_MAIN_HPP
#define GRAPHGEN_MAIN_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

struct Data
{
    std::string name;
    double value;

    Data(const std::string &name, double value) : name(name), value(value) {}
    bool operator==(const Data &other) const
    {
        return name == other.name && value == other.value;
    }
};

class Category
{
};

class Classified_Info
{
private:
    std::string name;
    std::string x_axis;
    std::string y_axis;
    std::vector<Data> datas;

public:
    Classified_Info(const std::vector<std::string> &csvData)
    {
        // get the header and split it into name, x_axis, and y_axis
        std::string header = csvData[0];
        name = header;
        std::string xy_axis = csvData[1];
        std::istringstream XY_axisStream(xy_axis);

        std::vector<std::string> xy_axisVector;

        std::string temp;
        while(std::getline(XY_axisStream, temp, ',')){
            temp = temp[0] == ' ' ? temp.substr(1) : temp;
            xy_axisVector.push_back(temp);
        }
        temp.clear();

        x_axis = xy_axisVector[0];
        y_axis = xy_axisVector[1];

        // loop over the data rows and create Data objects for each row
        for (size_t i = 2; i < csvData.size(); i++)
        {
            std::istringstream rowStream(csvData[i]);
            std::string name;
            double value;
            std::getline(rowStream, name, ',');
            rowStream >> value;
            datas.push_back(Data{name, value});
        }
    }

    // getter methods for the private member variables
    std::string getName() const
    {
        return name;
    }

    std::string getXAxis() const
    {
        return x_axis;
    }

    std::string getYAxis() const
    {
        return y_axis;
    }

    std::vector<Data> getDatas() const
    {
        return datas;
    }

    std::map<std::string, double> getDatasMap() const
    {
        std::map<std::string, double> datasMap;
        for (const auto &data : datas)
        {
            if (datasMap.find(data.name) != datasMap.end())
            {
                datasMap[data.name] += data.value;
            }
            else
            {
                datasMap[data.name] = data.value;
            }
        }
        return datasMap;
    }
};

class BarGraph
{
private:
    std::vector<Category> data;
};

#endif