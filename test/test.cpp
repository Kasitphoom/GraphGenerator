#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "..\lib\doctest\doctest.h"
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

TEST_CASE("Testing the readDataFromCSV function") {
    std::vector<std::string> lines = readDataFromCSV("testdata.csv");
    CHECK(lines == std::vector<std::string> {"HEADER", "month, money spent", "jan, 150", "jan, 200"});

    Classified_Info classified_info(lines);
    CHECK(classified_info.getName() == "HEADER");
    CHECK(classified_info.getXAxis() == "month");
    CHECK(classified_info.getYAxis() == "money spent");
    CHECK(classified_info.getDatas() == std::vector<Data> {Data{"jan", 150}, Data{"jan", 200}});
    CHECK(classified_info.getDatasMap() == std::map<std::string, double> {{"jan", 350}});
}

TEST_CASE("Checking Bargraph Correctness"){
    std::vector<std::string> lines = readDataFromCSV("testdata.csv");
    Classified_Info classified_info(lines);
    BarGraph bargraph(classified_info.getDatasMap(), classified_info.getName(), classified_info.getXAxis(), classified_info.getYAxis());

    CHECK(bargraph.getData() == std::map<std::string, double> {{"jan", 350}});
    CHECK(bargraph.getTitle() == "HEADER");
    CHECK(bargraph.getXLabel() == "month");
    CHECK(bargraph.getYLabel() == "money spent");
    CHECK(bargraph.getWidth() == 120);
    CHECK(bargraph.getAxisValues() == std::vector<double> {0, 35, 70, 105, 140, 175, 210, 245, 280, 315, 350});
}
