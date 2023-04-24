#include "graphgen.hpp"

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
    Classified_Info classified_info(lines);
    std::cout << "--------------------Testing Object--------------------" << std::endl;
    std::cout << "getName():" << classified_info.getName() << std::endl;
    std::cout << "getXAxis()" << classified_info.getXAxis() << std::endl;
    std::cout << "getYAxis()" << classified_info.getYAxis() << std::endl;
    std::vector<Data> datas = classified_info.getDatas();
    std::cout << "getDatas():" << std::endl;
    for (const auto& data : datas) {
        std::cout << data.name << " " << data.value << std::endl;
    }
    std::cout << "getDatasMap():" << std::endl;
    std::map<std::string, double> datasMap = classified_info.getDatasMap();
    for (const auto& data : datasMap) {
        std::cout << data.first << " " << data.second << std::endl;
    }

    return 0;
}