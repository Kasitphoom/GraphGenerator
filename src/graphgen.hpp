#ifndef GRAPHGEN_MAIN_HPP
#define GRAPHGEN_MAIN_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include "nse/html.hpp"
#include "nse/ptr.hpp"
#include "nse/html.cpp"

class Stream_writer : public nse::html::Writer
{
public:
    void write(std::string_view msg) const override;
    void write(const Element &e, int lv = 0) const override;

    Stream_writer(std::ostream &o) : os(o) {}

private:
    std::ostream &os;
};

void Stream_writer::write(std::string_view msg) const
{
    os << msg;
}

void Stream_writer::write(const Element &e, int lv) const
{
    e.write_to(*this, lv);
}

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

    const double X_GRAPH_PADDING = 60;
    const double Y_GRAPH_PADDING = 50;
    const double BAR_MARGIN = 10;

    std::map<std::string, double> data;
    std::vector<std::vector<double>> barPts;
    std::vector<std::vector<double>> yaxisvlauePts;
    std::vector<std::vector<double>> xaxisvlauePts;
    std::vector<std::vector<double>> realValuePts;

    std::string title;
    std::string xLabel;
    std::string yLabel;

    double width;
    double height;
    double yheight;
    double xwidth;

    std::vector<double> hlinePts;
    std::vector<double> vlinePts;
    std::vector<double> xLablePts;
    std::vector<double> yLablePts;

    std::vector<double> yaxisValues;
    std::vector<double> realValues;
    std::vector<std::string> xaxisValues;

public:
    BarGraph(const std::map<std::string, double> &data, std::string title, std::string xLable, std::string yLable)
    {
        this->data = data;
        this->height = 600;
        this->title = title;
        this->xLabel = xLable;
        this->yLabel = yLable;

        hlinePts.resize(4);
        vlinePts.resize(4);
        xLablePts.resize(2);
        yLablePts.resize(2);

        calcWidth();
        YAxisValue();
        XAxisValue();
        RealValue();
        calcHLinePts();
        calcVLinePts();
        calcXLablePts();
        calcYLablePts();
        calcBarPts();
        calcYAxisValuePts();
        calcXAxisValuePts();
        calcRealValuePts();

    }
    BarGraph() = delete;
    ~BarGraph(){
        this->hlinePts.clear();
        this->vlinePts.clear();
        this->xLablePts.clear();
        this->yLablePts.clear();
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

    void YAxisValue(){
        std::vector<double> axisValues;
        double max = find_max_value();
        double step = max / 10;
        for (int i = 0; i < 11; i++){
            axisValues.push_back(step * i);
        }
        this->yaxisValues = axisValues;
    }

    void XAxisValue(){
        std::vector<std::string> xaxisvlaue ;
        auto it = this->data.begin();
        for (int i = 0; i < this->data.size(); i++){
            xaxisvlaue.push_back(it->first);
            it++;
        }
        this->xaxisValues = xaxisvlaue;
    }

    void RealValue(){
        std::vector<double> realValues;
        auto it = this->data.begin();
        for (int i = 0; i < this->data.size(); i++){
            realValues.push_back(it->second);
            it++;
        }
        this->realValues = realValues;
    }

    void calcHLinePts(){
        this->hlinePts[0] = this->X_GRAPH_PADDING; //x1
        this->hlinePts[1] = this->height - this->Y_GRAPH_PADDING; //y1
        this->hlinePts[2] = this->width; //x2
        this->hlinePts[3] = this->height - this->Y_GRAPH_PADDING; //y2
    }

    void calcVLinePts(){
        this->vlinePts[0] = this->X_GRAPH_PADDING; //x1
        this->vlinePts[1] = this->height - this->Y_GRAPH_PADDING; //y1
        this->vlinePts[2] = this->X_GRAPH_PADDING; //x2
        this->vlinePts[3] = this->Y_GRAPH_PADDING; //y2
    }

    void calcXLablePts(){
        this->xLablePts[0] = this->width / 2; //x1
        this->xLablePts[1] = this->height - 15; //y1
    }

    void calcYLablePts(){
        this->yLablePts[0] = this->X_GRAPH_PADDING / 2; //x1
        this->yLablePts[1] = this->height / 2; //y1
    }

    void calcBarPts(){
        std::vector<std::vector<double>> barPts;
        double x = this->X_GRAPH_PADDING;
        double y = this->height - this->Y_GRAPH_PADDING;
        double step = 50 + this->BAR_MARGIN;
        double barWidth = step - this->BAR_MARGIN;
        for (const auto &data : this->data){
            double barHeight = data.second / find_max_value() * (this->height - 2 * this->Y_GRAPH_PADDING);
            std::vector<double> barPt = {x, y, barWidth, barHeight};
            barPts.push_back(barPt);
            x += step;
        }
        this->barPts = barPts;
    }

    void calcYAxisValuePts(){
        std::vector<std::vector<double>> yaxisavluePts;
        double x = 40;
        double y = this->height - this->Y_GRAPH_PADDING;
        double step = 50;
        for (int i = 0; i < 11; i++){
            std::vector<double> yaxisavluePt = {x, y};
            yaxisavluePts.push_back(yaxisavluePt);
            y -= step;
        }
        this->yaxisvlauePts = yaxisavluePts;
    }

    void calcXAxisValuePts(){
        std::vector<std::vector<double>> xaxisavluePts;
        double x = this->X_GRAPH_PADDING + 25; // 25 is the half of the bar width
        double y = this->height - 30;
        double step = 50 + this->BAR_MARGIN;
        for (int i = 0; i < this->data.size(); i++){
            std::vector<double> xaxisavluePt = {x, y};
            xaxisavluePts.push_back(xaxisavluePt);
            x += step;
        }
        this->xaxisvlauePts = xaxisavluePts;
    }

    void calcRealValuePts(){
        std::vector<std::vector<double>> realValuePts;
        for (int i = 0; i < this->data.size(); i++){
            double x = this->barPts[i][0] + this->barPts[i][2] / 2;
            double y = this->barPts[i][1] - this->barPts[i][3] - 10;
            std::vector<double> realValuePt = {x, y};
            realValuePts.push_back(realValuePt);
        }
        this->realValuePts = realValuePts;
    }

    std::vector<double> getyAxisValues() const
    {
        return yaxisValues;
    }

    std::vector<std::string> getxAxisValues() const
    {
        return xaxisValues;
    }

    void calcWidth(){
        this->width = this->X_GRAPH_PADDING * (data.size() + 1);
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

    std::vector<std::vector<double>> getBarPts() const
    {
        return barPts;
    }

    std::vector<double> getHLinePts() const
    {
        return hlinePts;
    }

    std::vector<double> getVLinePts() const
    {
        return vlinePts;
    }

    std::vector<double> getXLablePts() const
    {
        return xLablePts;
    }

    std::vector<double> getYLablePts() const
    {
        return yLablePts;
    }

    std::vector<std::vector<double>> getYaxisvlauePts() const
    {
        return yaxisvlauePts;
    }

    std::vector<std::vector<double>> getXaxisvlauePts() const
    {
        return xaxisvlauePts;
    }

    std::vector<std::vector<double>> getRealValuePts() const
    {
        return realValuePts;
    }

    std::vector<double> getRealValue(){
        return realValues;
    }
};

void GenHTML(BarGraph bg, std::string file){
    using namespace nse::html;
    Stream_writer html(std::cout);

    std::vector<Element> svg_elems;

    std::ofstream out(file);
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    std::cout << "<!DOCTYPE html>\n";

    auto graph_title = Element("text", {{"x", std::to_string(bg.getXLablePts()[0])}, {"y", std::to_string(25)}, {"fill", "white"}, {"class", "graph-title white"}}, {Element::text(bg.getTitle())});
    svg_elems.push_back(graph_title);

    auto xline = Element("line", {{"x1", std::to_string(bg.getHLinePts()[0])}, {"y1", std::to_string(bg.getHLinePts()[1])}, {"x2", std::to_string(bg.getHLinePts()[2])}, {"y2", std::to_string(bg.getHLinePts()[3])}, {"stroke", "white"}, {"stroke-width", "2"}}, {Element::text("")});
    svg_elems.push_back(xline);

    auto yline = Element("line", {{"x1", std::to_string(bg.getVLinePts()[0])}, {"y1", std::to_string(bg.getVLinePts()[1])}, {"x2", std::to_string(bg.getVLinePts()[2])}, {"y2", std::to_string(bg.getVLinePts()[3])}, {"stroke", "white"}, {"stroke-width", "2"}}, {Element::text("")});
    svg_elems.push_back(yline);

    auto xlable = Element("text", {{"x", std::to_string(bg.getXLablePts()[0])}, {"y", std::to_string(bg.getXLablePts()[1])}, {"fill", "white"}, {"class", "axis-label white"}}, {Element::text(bg.getXLabel())});
    svg_elems.push_back(xlable);

    auto ylable = Element("text", {{"x", std::to_string(bg.getYLablePts()[0])}, {"y", std::to_string(bg.getYLablePts()[1])}, {"fill", "white"}, {"class", "axis-label label-y white"}}, {Element::text(bg.getYLabel())});
    svg_elems.push_back(ylable);

    for(auto data : bg.getBarPts()){
        auto bar = Element("rect", {{"x", std::to_string(data[0])}, {"y", std::to_string(data[1] - data[3])}, {"width", std::to_string(data[2])}, {"height", std::to_string(data[3])}, {"class", "data"}}, {Element::text("")});
        svg_elems.push_back(bar);
    }

    for(int i = 0; i < bg.getyAxisValues().size(); i++){
        auto yaxisvalue = Element("text", {{"x", std::to_string(bg.getYaxisvlauePts()[i][0])}, {"y", std::to_string(bg.getYaxisvlauePts()[i][1])}, {"fill", "white"}, {"class", "content red"}}, {Element::text(std::to_string((int)bg.getyAxisValues()[i]))});
        svg_elems.push_back(yaxisvalue);
    }

    for(int i = 0; i < bg.getxAxisValues().size(); i++){
        auto xaxisvalue = Element("text", {{"x", std::to_string(bg.getXaxisvlauePts()[i][0])}, {"y", std::to_string(bg.getXaxisvlauePts()[i][1])}, {"fill", "white"}, {"class", "content data-lable cat"}}, {Element::text(bg.getxAxisValues()[i])});
        svg_elems.push_back(xaxisvalue);
    }

    for(int i = 0; i < bg.getRealValue().size(); i++){
        auto realvalue = Element("text", {{"x", std::to_string(bg.getRealValuePts()[i][0])}, {"y", std::to_string(bg.getRealValuePts()[i][1])}, {"fill", "white"}, {"class", "content data-lable"}}, {Element::text(std::to_string((int)bg.getRealValue()[i]))});
        svg_elems.push_back(realvalue);
    }

    auto svg = Element("svg", {{"width", std::to_string(bg.getWidth())}, {"height", std::to_string(bg.getHeight())}}, svg_elems);
    auto script = Element("script", {{"src", "../src/script.js"}}, {Element::text("")});

    auto body = Element("body", {{"background-color", "#282C34"}}, {svg, script});

    auto link_css = Element("link", {{"rel", "stylesheet"}, {"href", "../src/style.css"}}, {Element::text("")});
    auto head = Element("head", {}, {Element("title", {}, {Element::text(bg.getTitle())}), link_css});
    auto doc = Element("html", {}, {head, body});

    html.write(doc);
}

#endif