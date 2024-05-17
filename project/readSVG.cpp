
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <cmath>
#include <iostream>

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

//! Function to encode the SVG element type string into a type code
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

//! Function to read an SVG file and extract its elements
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

    //! Iterate through each child element of the root element
    for (XMLElement *child = xml_elem->FirstChildElement(); child != NULL;
         child = child->NextSiblingElement()) {
        parseElement(child, shapes, dictionary);
    }

    svg_elements = shapes;
    shapes.clear();
}

//! Function to parse an SVG element and create the corresponding shape object
void parseElement(tinyxml2::XMLElement *child,
                  vector<svg::SVGElement *> &shapes,
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

    switch (encode(child_name)) {   // Switch based on the encoded child name
    case group: {                   // If the element is a group
        vector<SVGElement *> group_shapes;
        // Iterate through each child element of the group element
        for (XMLElement *group_child = child->FirstChildElement();
             group_child != NULL;
             group_child = group_child->NextSiblingElement()) {
            parseElement(group_child, group_shapes,
                         dictionary);   // Recursively parse each child element
        }
        // Check and store the transform attribute if it exists
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
        Group *g = new Group(
            group_shapes);   // Create a new Group object with the parsed shapes
        if (transform != "") {
            g->transform(transform, origin);   // Apply transformation if exists
        }
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] =
                g;   // Store the group in the dictionary if it has an ID
        }
        shapes.push_back(g);   // Add the group to the shapes vector
        break;
    }
    case ellipse: {   // If the element is an ellipse
        c_fill =
            parse_color(child->Attribute("fill"));   // Parse the fill color
        c_center = {child->IntAttribute("cx"),
                    child->IntAttribute("cy")};   // Get center coordinates
        c_radius = {child->IntAttribute("rx"),
                    child->IntAttribute("ry")};   // Get radii

        // Check and store the transform attribute if it exists
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
        Ellipse *e = new Ellipse(c_fill, c_center,
                                 c_radius);   // Create a new Ellipse object
        if (transform != "") {
            e->transform(transform, origin);   // Apply transformation if exists
        }
        shapes.push_back(e);   // Add the ellipse to the shapes vector
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] =
                e;   // Store the ellipse in the dictionary if it has an ID
        }
        break;
    }
    case circle: {   // If the element is a circle
        c_fill =
            parse_color(child->Attribute("fill"));   // Parse the fill color
        c_center = {child->IntAttribute("cx"),
                    child->IntAttribute("cy")};   // Get center coordinates
        c_radius = {child->IntAttribute("r"),
                    child->IntAttribute("r")};   // Get radius

        // Check and store the transform attribute if it exists
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
        Ellipse *c = new Ellipse(c_fill, c_center,
                                 c_radius);   // Create a new Ellipse object
                                              // (circles are special ellipses)
        if (transform != "") {
            c->transform(transform, origin);   // Apply transformation if exists
        }
        shapes.push_back(c);   // Add the circle to the shapes vector
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] =
                c;   // Store the circle in the dictionary if it has an ID
        }
        break;
    }
    case polygon: {   // If the element is a polygon
        c_fill =
            parse_color(child->Attribute("fill"));   // Parse the fill color
        point_str = child->Attribute("points");      // Get the points attribute
        while ((pos = point_str.find(delimiter)) !=
               string::npos) {   // Parse each point
            point = point_str.substr(0, pos);
            Point p = {stoi(point.substr(0, point.find(","))),
                       stoi(point.substr(point.find(",") + 1, point.size()))};
            c_points.push_back(p);
            point_str.erase(0, pos + delimiter.length());
        }
        c_points.push_back({stoi(point_str.substr(0, point_str.find(","))),
                            stoi(point_str.substr(point_str.find(",") + 1,
                                                  point_str.size()))});

        // Check and store the transform attribute if it exists
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
        Polygon *p =
            new Polygon(c_fill, c_points);   // Create a new Polygon object
        if (transform != "") {
            p->transform(transform, origin);   // Apply transformation if exists
        }
        shapes.push_back(p);   // Add the polygon to the shapes vector
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] =
                p;   // Store the polygon in the dictionary if it has an ID
        }
        break;
    }
    case rect: {   // If the element is a rectangle
        c_fill =
            parse_color(child->Attribute("fill"));   // Parse the fill color
        // Get the rectangle's four corners
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

        // Check and store the transform attribute if it exists
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
        Polygon *r = new Polygon(
            c_fill,
            c_points);   // Create a new Polygon object for the rectangle
        if (transform != "") {
            r->transform(transform, origin);   // Apply transformation if exists
        }
        shapes.push_back(r);   // Add the rectangle to the shapes vector
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] =
                r;   // Store the rectangle in the dictionary if it has an ID
        }
        break;
    }
    case polyline: {   // If the element is a polyline
        c_stroke =
            parse_color(child->Attribute("stroke"));   // Parse the stroke color
        point_str = child->Attribute("points");   // Get the points attribute
        while ((pos = point_str.find(delimiter)) !=
               string::npos) {   // Parse each point
            point = point_str.substr(0, pos);
            c_points.push_back({stoi(point.substr(0, point.find(","))),
                                stoi(point.substr(point.find(",") + 1))});
            point_str.erase(0, pos + delimiter.length());
        }
        c_points.push_back({stoi(point_str.substr(0, point_str.find(","))),
                            stoi(point_str.substr(point_str.find(",") + 1))});

        // Check and store the transform attribute if it exists
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
        Polyline *p =
            new Polyline(c_stroke, c_points);   // Create a new Polyline object
        if (transform != "") {
            p->transform(transform, origin);   // Apply transformation if exists
        }
        shapes.push_back(p);   // Add the polyline to the shapes vector
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] =
                p;   // Store the polyline in the dictionary if it has an ID
        }
        break;
    }
    case line: {   // If the element is a line
        c_stroke =
            parse_color(child->Attribute("stroke"));   // Parse the stroke color
        // Get the line's start and end points
        c_points.push_back(
            {child->IntAttribute("x1"), child->IntAttribute("y1")});
        c_points.push_back(
            {child->IntAttribute("x2"), child->IntAttribute("y2")});

        // Check and store the transform attribute if it exists
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
        Polyline *l = new Polyline(
            c_stroke, c_points);   // Create a new Polyline object for the line
        if (transform != "") {
            l->transform(transform, origin);   // Apply transformation if exists
        }
        shapes.push_back(l);   // Add the line to the shapes vector
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] =
                l;   // Store the line in the dictionary if it has an ID
        }
        break;
    }
    case use: {   // If the element is a use element (reference to another
                  // element)
        string href = child->Attribute("href");   // Get the href attribute
        string ref = href.substr(1);   // Extract the ID being referenced
        SVGElement *elem =
            dictionary[ref];   // Find the referenced element in the dictionary
        // Check and store the transform attribute if it exists
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

        Use *u = new Use(
            elem);   // Create a new Use object for the referenced element
        if (transform != "") {
            u->transform(transform, origin);   // Apply transformation if exists
        }
        if (child->Attribute("id") != NULL) {
            dictionary[child->Attribute("id")] =
                u;   // Store the use element in the dictionary if it has an ID
        }
        shapes.push_back(u);   // Add the use element to the shapes vector
        break;
    }
    default:
        cout << "Unknown element"
             << endl;   // Print an error message if the element is unknown
    }
}
}   // namespace svg