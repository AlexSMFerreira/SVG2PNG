// CAN BE MODIFIED

#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace tinyxml2;

namespace svg {
enum type_code {
    group,
    ellipse,
    circle,
    polygon,
    rect,
    polyline,
    line,
    use,
    other
};

type_code encode(string const &type_string) {
    if (type_string == "ellipse")
        return ellipse;
    else if (type_string == "circle")
        return circle;
    else if (type_string == "rect")
        return rect;
    else if (type_string == "polygon")
        return polygon;
    else if (type_string == "polyline")
        return polyline;
    else if (type_string == "line")
        return line;
    else if (type_string == "g")
        return group;
    else if (type_string == "use")
        return use;
    else
        return other;
}

void readSVG(const string &svg_file, Point &dimensions,
             vector<SVGElement *> &svg_elements) {
    XMLDocument doc;
    XMLError r = doc.LoadFile(svg_file.c_str());
    if (r != XML_SUCCESS) {
        throw runtime_error("Unable to load " + svg_file);
    }
    XMLElement *xml_elem = doc.RootElement();
    vector<SVGElement *> shapes;

    dimensions.x = xml_elem->IntAttribute("width");
    dimensions.y = xml_elem->IntAttribute("height");
    unordered_map<string, SVGElement *> dictionary;

    for (XMLElement *child = xml_elem->FirstChildElement(); child != NULL;
         child = child->NextSiblingElement()) {
        parseElement(child, shapes, dictionary);
    }

    svg_elements = shapes;

    shapes.clear();
}

void parseElement(tinyxml2::XMLElement *child,
                  std::vector<svg::SVGElement *> &shapes,
                  unordered_map<string, SVGElement *> &dictionary) {
    string child_name(child->Name());
    Color c_fill;
    Point c_center;
    Point c_radius;
    vector<Point> c_points;
    Color c_stroke;
    string point_str;
    string point;
    string delimiter = " ";
    size_t pos = 0;
    Point origin = {0, 0};

    string transform = "";
    string transform_origin = "";

    switch (encode(child_name)) {
    case ellipse: {
        c_fill = parse_color(child->Attribute("fill"));
        c_center = {child->IntAttribute("cx"), child->IntAttribute("cy")};
        c_radius = {child->IntAttribute("rx"), child->IntAttribute("ry")};

        // transforms
        if (child->Attribute("transform") != NULL) {
            transform = child->Attribute("transform");
            if (child->Attribute("transform-origin") != NULL) {
                transform_origin = child->Attribute("transform-origin");
                origin = {
                    stoi(
                        transform_origin.substr(0, transform_origin.find(" "))),
                    stoi(transform_origin.substr(transform_origin.find(" ") + 1,
                                                 transform_origin.size()))};
            }
        }
        Ellipse *e = new Ellipse(c_fill, c_center, c_radius);
        if (transform != "") {
            e->transform(transform, origin);
        }
        shapes.push_back(e);
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] = e;
        }
        break;
    }
    case circle: {
        c_fill = parse_color(child->Attribute("fill"));
        c_center = {child->IntAttribute("cx"), child->IntAttribute("cy")};
        c_radius = {child->IntAttribute("r"), child->IntAttribute("r")};

        // transforms
        if (child->Attribute("transform") != NULL) {
            transform = child->Attribute("transform");
            if (child->Attribute("transform-origin") != NULL) {
                transform_origin = child->Attribute("transform-origin");
                origin = {
                    stoi(
                        transform_origin.substr(0, transform_origin.find(" "))),
                    stoi(transform_origin.substr(transform_origin.find(" ") + 1,
                                                 transform_origin.size()))};
            }
        }
        Ellipse *c = new Ellipse(c_fill, c_center, c_radius);
        if (transform != "") {
            c->transform(transform, origin);
        }
        shapes.push_back(c);
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] = c;
        }
        break;
    }
    case polygon: {
        c_fill = parse_color(child->Attribute("fill"));
        point_str = child->Attribute("points");
        while ((pos = point_str.find(delimiter)) != string::npos) {
            point = point_str.substr(0, pos);
            Point p = {stoi(point.substr(0, point.find(","))),
                       stoi(point.substr(point.find(",") + 1, point.size()))};
            c_points.push_back(p);
            point_str.erase(0, pos + delimiter.length());
        }
        c_points.push_back({stoi(point_str.substr(0, point_str.find(","))),
                            stoi(point_str.substr(point_str.find(",") + 1,
                                                  point_str.size()))});
        // transforms
        if (child->Attribute("transform") != NULL) {
            transform = child->Attribute("transform");
            if (child->Attribute("transform-origin") != NULL) {
                transform_origin = child->Attribute("transform-origin");
                origin = {
                    stoi(
                        transform_origin.substr(0, transform_origin.find(" "))),
                    stoi(transform_origin.substr(transform_origin.find(" ") + 1,
                                                 transform_origin.size()))};
            }
        }
        Polygon *p = new Polygon(c_fill, c_points);
        if (transform != "") {
            p->transform(transform, origin);
        }
        shapes.push_back(p);
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] = p;
        }
        break;
    }
    case rect: {
        c_fill = parse_color(child->Attribute("fill"));
        c_points.push_back(
            {child->IntAttribute("x"), child->IntAttribute("y")});
        c_points.push_back(
            {child->IntAttribute("x") + child->IntAttribute("width") - 1,
             child->IntAttribute("y")});
        c_points.push_back(
            {child->IntAttribute("x") + child->IntAttribute("width") - 1,
             child->IntAttribute("y") + child->IntAttribute("height") - 1});
        c_points.push_back(
            {child->IntAttribute("x"),
             child->IntAttribute("y") + child->IntAttribute("height") - 1});
        // transforms
        if (child->Attribute("transform") != NULL) {
            transform = child->Attribute("transform");
            if (child->Attribute("transform-origin") != NULL) {
                transform_origin = child->Attribute("transform-origin");
                origin = {
                    stoi(
                        transform_origin.substr(0, transform_origin.find(" "))),
                    stoi(transform_origin.substr(transform_origin.find(" ") + 1,
                                                 transform_origin.size()))};
            }
        }
        Polygon *r = new Polygon(c_fill, c_points);
        if (transform != "") {
            r->transform(transform, origin);
        }
        shapes.push_back(r);
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] = r;
        }
        break;
    }
    case polyline: {
        c_stroke = parse_color(child->Attribute("stroke"));
        point_str = child->Attribute("points");
        while ((pos = point_str.find(delimiter)) != string::npos) {
            point = point_str.substr(0, pos);
            c_points.push_back({stoi(point.substr(0, point.find(","))),
                                stoi(point.substr(point.find(",") + 1))});
            point_str.erase(0, pos + delimiter.length());
        }
        c_points.push_back({stoi(point_str.substr(0, point_str.find(","))),
                            stoi(point_str.substr(point_str.find(",") + 1))});
        // transforms
        if (child->Attribute("transform") != NULL) {
            transform = child->Attribute("transform");
            if (child->Attribute("transform-origin") != NULL) {
                transform_origin = child->Attribute("transform-origin");
                origin = {
                    stoi(
                        transform_origin.substr(0, transform_origin.find(" "))),
                    stoi(transform_origin.substr(transform_origin.find(" ") + 1,
                                                 transform_origin.size()))};
            }
        }
        Polyline *p = new Polyline(c_stroke, c_points);
        if (transform != "") {
            p->transform(transform, origin);
        }
        shapes.push_back(p);
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] = p;
        }
        break;
    }
    case line: {
        c_stroke = parse_color(child->Attribute("stroke"));
        c_points.push_back(
            {child->IntAttribute("x1"), child->IntAttribute("y1")});
        c_points.push_back(
            {child->IntAttribute("x2"), child->IntAttribute("y2")});
        // transforms
        if (child->Attribute("transform") != NULL) {
            transform = child->Attribute("transform");
            if (child->Attribute("transform-origin") != NULL) {
                transform_origin = child->Attribute("transform-origin");
                origin = {
                    stoi(
                        transform_origin.substr(0, transform_origin.find(" "))),
                    stoi(transform_origin.substr(transform_origin.find(" ") + 1,
                                                 transform_origin.size()))};
            }
        }
        Polyline *l = new Polyline(c_stroke, c_points);
        if (transform != "") {
            l->transform(transform, origin);
        }
        shapes.push_back(l);
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] = l;
            cout << dictionary[child->Attribute("id")] << endl;
        }
        break;
    }
    default:
        cout << "Unknown element" << endl;
    }
}
}   // namespace svg