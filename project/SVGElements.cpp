// CAN BE MODIFIED

#include "SVGElements.hpp"
#include <iostream>
#include <vector>

namespace svg {

Group::Group(const std::vector<SVGElement *> &elements) {
    for (SVGElement *element : elements) {
        this->elements.push_back(element);
    }
}
void Group::draw(PNGImage &img) const {
    for (SVGElement *element : elements) {
        element->draw(img);
    }
};
void Group::transform(string transform, Point origin) {
    for (SVGElement *element : elements) {
        element->transform(transform, origin);
    }
};
SVGElement *Group::clone() const {
    std::vector<SVGElement *> cloned_elements;
    for (SVGElement *element : elements) {
        cloned_elements.push_back(element->clone());
    }
    return new Group(cloned_elements);
}
Group::~Group() {
    for (SVGElement *element : elements) {
        delete element;
    }
}

Use::Use(SVGElement *element) { this->element = element->clone(); }
Use::~Use() { delete element; }
void Use::draw(PNGImage &img) const { element->draw(img); }
void Use::transform(string transform, Point origin) {
    element->transform(transform, origin);
}
SVGElement *Use::clone() const { return new Use(element); }
// These must not be defined!
SVGElement::SVGElement() {}
SVGElement::~SVGElement() {}

// Ellipse (initial code provided)
Ellipse::Ellipse(const Color &fill, const Point &center, const Point &radius)
    : fill(fill), center(center), radius(radius) {}
void Ellipse::draw(PNGImage &img) const {
    img.draw_ellipse(center, radius, fill);
}
void Ellipse::transform(string transform, Point origin) {
    Point translate;
    int r_angle;
    int scale_factor;

    if (transform.find("translate") != string::npos) {
        translate = {
            stoi(transform.substr(transform.find("(") + 1,
                                  transform.find_first_of(" ,") -
                                      transform.find("(") - 1)),
            stoi(transform.substr(transform.find_first_of(", ") + 1,
                                  transform.find(")") -
                                      transform.find_first_of(", ") - 1))};
        center = center.translate(translate);
    }
    if (transform.find("rotate") != string::npos) {
        r_angle = stoi(
            transform.substr(transform.find("(") + 1,
                             transform.find(")") - transform.find("(") - 1));
        center = center.rotate(origin, r_angle);
    }
    if (transform.find("scale") != string::npos) {
        scale_factor = stoi(
            transform.substr(transform.find("(") + 1,
                             transform.find(")") - transform.find("(") - 1));
        radius = radius.scale({0, 0}, scale_factor);
        center = center.scale(origin, scale_factor);
    }
    // @todo implement the transformation
}
SVGElement *Ellipse::clone() const { return new Ellipse(fill, center, radius); }
// @todo provide the implementation of SVGElement derived classes
// HERE -->
Polygon::Polygon(const Color &fill, const std::vector<Point> &points)
    : fill(fill), points(points) {}
void Polygon::draw(PNGImage &img) const { img.draw_polygon(points, fill); }
void Polygon::transform(string transform, Point origin) {
    Point translate;
    int r_angle;
    int scale_factor;
    if (transform.find("translate") != string::npos) {
        translate = {
            stoi(transform.substr(transform.find("(") + 1,
                                  transform.find_first_of(" ,") -
                                      transform.find("(") - 1)),
            stoi(transform.substr(transform.find_first_of(", ") + 1,
                                  transform.find(")") -
                                      transform.find_first_of(" ,") - 1))};
        for (Point &p : points) {
            p = p.translate(translate);
        }
    }
    if (transform.find("rotate") != string::npos) {
        r_angle = stoi(
            transform.substr(transform.find("(") + 1,
                             transform.find(")") - transform.find("(") - 1));
        for (Point &p : points) {
            p = p.rotate(origin, r_angle);
        }
    }
    if (transform.find("scale") != string::npos) {
        scale_factor = stoi(
            transform.substr(transform.find("(") + 1,
                             transform.find(")") - transform.find("(") - 1));
        for (Point &p : points) {
            p = p.scale(origin, scale_factor);
        }
    }
}
SVGElement *Polygon::clone() const { return new Polygon(fill, points); }

Polyline::Polyline(const Color &stroke, const std::vector<Point> &points)
    : stroke(stroke), points(points) {}
void Polyline::draw(PNGImage &img) const {
    for (size_t i = 0; i < points.size() - 1; i++) {
        img.draw_line(points[i], points[i + 1], stroke);
    }
}
void Polyline::transform(string transform, Point origin) {
    Point translate;
    int r_angle;
    int scale_factor;
    if (transform.find("translate") != string::npos) {
        translate = {
            stoi(transform.substr(transform.find("(") + 1,
                                  transform.find_first_of(", ") -
                                      transform.find("(") - 1)),
            stoi(transform.substr(transform.find_first_of(", ") + 1,
                                  transform.find(")") -
                                      transform.find_first_of(", ") - 1))};
        for (Point &p : points) {
            p = p.translate(translate);
        }
    }
    if (transform.find("rotate") != string::npos) {
        r_angle = stoi(
            transform.substr(transform.find("(") + 1,
                             transform.find(")") - transform.find("(") - 1));
        for (Point &p : points) {
            p = p.rotate(origin, r_angle);
        }
    }
    if (transform.find("scale") != string::npos) {
        scale_factor = stoi(
            transform.substr(transform.find("(") + 1,
                             transform.find(")") - transform.find("(") - 1));
        for (Point &p : points) {
            p = p.scale(origin, scale_factor);
        }
    }
}
SVGElement *Polyline::clone() const { return new Polyline(stroke, points); }
}   // namespace svg
// namespace svg
