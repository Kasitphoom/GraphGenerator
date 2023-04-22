#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>


struct Data{
    std::string name;
    double value;
};

class Category {
private:
    std::string name;
    std::string x_axis;
    std::string y_axis;
    std::vector<Data> datas;

public:
    Category(const std::vector<std::string>& csvData) {
        // get the header and split it into name, x_axis, and y_axis
        std::string header = csvData[0];
        name = header;
        std::string xy_axis = csvData[1];
        std::istringstream XY_axisStream(xy_axis);
        std::string col1, col2;
        std::getline(XY_axisStream, col1, ',');
        XY_axisStream >> col2;
        x_axis = col1;
        y_axis = col2;
        
        // loop over the data rows and create Data objects for each row
        for (size_t i = 2; i < csvData.size(); i++) {
            std::istringstream rowStream(csvData[i]);
            std::string name;
            double value;
            std::getline(rowStream, name, ',');
            rowStream >> value;
            datas.push_back(Data{name, value});
        }
    }

    // getter methods for the private member variables
    std::string getName() const {
        return name;
    }

    std::string getXAxis() const {
        return x_axis;
    }

    std::string getYAxis() const {
        return y_axis;
    }

    std::vector<Data> getDatas() const {
        return datas;
    }
};

class BarGraph{
    private:
        std::vector<Category> data;
};

//functions


std::vector<std::string> readDataFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;

    // loop over each line of the file and add it to the vector
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}



int main(int argc, char** argv) {

    char* path = argv[1];

    std::vector<std::string> lines = readDataFromCSV(path);

    // print out the lines
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }

    std::cout << "======================Earth's_test_chamber======================" << std::endl;
    Category category(lines);
    std::cout << "--------------------Testing Object--------------------" << std::endl;
    std::cout << "getName():" << category.getName() << std::endl;
    std::cout << "getXAxis()" << category.getXAxis() << std::endl;
    std::cout << "getYAxis()" << category.getYAxis() << std::endl;
    std::vector<Data> datas = category.getDatas();
    std::cout << "getDatas():" << std::endl;
    for (const auto& data : datas) {
        std::cout << data.name << " " << data.value << std::endl;
    }

    return 0;
}