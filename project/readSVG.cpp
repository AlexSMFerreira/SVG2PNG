#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{
    enum type_code {
        ellipse,
        circle,
        polygon,
        rect,
        polyline,
        line,
        other
    };

    type_code encode (std::string const& type_string) {
        if (type_string == "ellipse") return ellipse;
        else if (type_string == "circle") return circle;
        else if (type_string == "rect") return rect;
        else if (type_string == "polygon") return polygon;
        else if (type_string == "polyline") return polyline;
        else if (type_string == "line") return line;
        else return other;
    }
    
    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();
        vector<SVGElement *> shapes;

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");
        
        string delimiter = " ";
        size_t pos = 0;

        for(XMLElement* child = xml_elem->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
            string child_name(child->Name());
            Color c_fill;
            Point c_center;
            Point c_radius;
            vector<Point> c_points;
            Color c_stroke;
            string point_str;
            string point;
            switch(encode(child_name)) {
                case ellipse:
                    c_fill = parse_color(child->Attribute("fill"));
                    c_center = {child->IntAttribute("cx"), child->IntAttribute("cy")};
                    c_radius = {child->IntAttribute("rx"), child->IntAttribute("ry")};
                    shapes.push_back(new Ellipse(c_fill, c_center, c_radius));
                    break;
                case circle:
                    c_fill = parse_color(child->Attribute("fill"));
                    c_center = {child->IntAttribute("cx"), child->IntAttribute("cy")};
                    c_radius = {child->IntAttribute("r"), child->IntAttribute("r")};
                    shapes.push_back(new Ellipse(c_fill, c_center, c_radius));
                    break;
                case polygon:
                    c_fill = parse_color(child->Attribute("fill"));
                    point_str = child->Attribute("points");
                    while ((pos = point_str.find(delimiter)) != string::npos) {
                        point = point_str.substr(0, pos);
                        Point p = {stoi(point.substr(0, point.find(","))), stoi(point.substr(point.find(",") + 1, point.size()))};
                        c_points.push_back(p);
                        point_str.erase(0, pos + delimiter.length());
                    }
                    c_points.push_back({stoi(point_str.substr(0, point_str.find(","))), stoi(point_str.substr(point_str.find(",") + 1, point_str.size()))});
                    shapes.push_back(new Polygon(c_fill, c_points));
                    break;
                case rect:
                    c_fill = parse_color(child->Attribute("fill"));
                    c_points.push_back({child->IntAttribute("x"), child->IntAttribute("y")});
                    c_points.push_back({child->IntAttribute("x") + child->IntAttribute("width") - 1, child->IntAttribute("y")});
                    c_points.push_back({child->IntAttribute("x") + child->IntAttribute("width") - 1, child->IntAttribute("y") + child->IntAttribute("height") - 1});
                    c_points.push_back({child->IntAttribute("x"), child->IntAttribute("y") + child->IntAttribute("height") - 1});
                    shapes.push_back(new Polygon(c_fill, c_points));
                    break;
                case polyline:
                    c_stroke = parse_color(child->Attribute("stroke"));
                    point_str = child->Attribute("points");
                    while ((pos = point_str.find(delimiter)) != string::npos) {
                        point = point_str.substr(0, pos);
                        c_points.push_back({stoi(point.substr(0, point.find(","))), stoi(point.substr(point.find(",") + 1))});
                        point_str.erase(0, pos + delimiter.length());
                    }
                    c_points.push_back({stoi(point_str.substr(0, point_str.find(","))), stoi(point_str.substr(point_str.find(",") + 1))});
                    shapes.push_back(new Polyline(c_stroke, c_points));
                    break;
                case line:
                    c_stroke = parse_color(child->Attribute("stroke"));
                    c_points.push_back({child->IntAttribute("x1"), child->IntAttribute("y1")});
                    c_points.push_back({child->IntAttribute("x2"), child->IntAttribute("y2")});
                    shapes.push_back(new Polyline(c_stroke, c_points));
                    break;
                default:
                    cout << "Unknown element" << endl;
            }
        }

        svg_elements = shapes;
        shapes.clear();
    }
}