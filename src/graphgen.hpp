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
    std::map<std::string, double> data;

    std::string title;
    std::string xLabel;
    std::string yLabel;

    double width;
    double height;
    double* hlinePts;
    double* vlinePts;
    double* xLablePts;
    double* yLablePts;

    std::vector<double> axisValues;

public:
    BarGraph(const std::map<std::string, double> &data, std::string title, std::string xLable, std::string yLable)
    {
        this->data = data;
        this->height = 600;
        this->title = title;
        this->xLabel = xLable;
        this->yLabel = yLable;
        calcWidth();
        CalculateAxisValue();
        hlinePts = new double[4];
        vlinePts = new double[4];
        xLablePts = new double[2];
        yLablePts = new double[2];
    }
    BarGraph() = delete;
    ~BarGraph(){
        delete[] hlinePts;
        delete[] vlinePts;
        delete[] xLablePts;
        delete[] yLablePts;
    }

    double find_max_value(){
        double max = 0;
        for (const auto &data : this->data){
            if (data.second > max){
                max = data.second;
            }
        }
        return max;
    }

    void CalculateAxisValue(){
        std::vector<double> axisValues;
        double max = find_max_value();
        double step = max / 10;
        for (int i = 0; i < 11; i++){
            axisValues.push_back(step * i);
        }
        this->axisValues = axisValues;
    }

    std::vector<double> getAxisValues() const
    {
        return axisValues;
    }

    void calcWidth(){
        this->width = 60 * (data.size() + 1);
    }

    std::string getTitle() const
    {
        return title;
    }

    std::string getXLabel() const
    {
        return xLabel;
    }

    std::string getYLabel() const
    {
        return yLabel;
    }

    double getWidth() const
    {
        return width;
    }

    double getHeight() const
    {
        return height;
    }

    std::map<std::string, double> getData() const
    {
        return data;
    }

};

#endif