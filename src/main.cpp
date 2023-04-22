#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>


struct Data{
    std::string name;
    std::string value;
};

class Category{
    private:
        std::string name;
        std::vector<Data> datas;
        
    public:
        Category(std::string name,std::vector<Data> datas): name(name), datas(datas){};
        

        
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

    return 0;
}