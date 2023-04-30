#include "../src/graphgen.hpp"

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

int main(){
    std::vector<std::string> lines = readDataFromCSV("../data.csv");
    Classified_Info classified_info(lines);
    BarGraph bargraph(classified_info.getDatasMap(), classified_info.getName(), classified_info.getXAxis(), classified_info.getYAxis());
    GenHTML(bargraph, "test.html");
    return 0;
}