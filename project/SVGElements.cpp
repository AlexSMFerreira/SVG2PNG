// CAN BE MODIFIED

#include "SVGElements.hpp"
#include <iostream>
#include <vector>

namespace svg {

class SVGGroup : public SVGElement {
  private:
    std::vector<SVGElement *> elements;

  public:
    SVGGroup() {}

    virtual ~SVGGroup() {
        for (auto &element : elements) {
            delete element;
        }
    }
};

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
        translate = {stoi(transform.substr(transform.find("(") + 1,
                                           transform.find(" ") -
                                               transform.find("(") - 1)),
                     stoi(transform.substr(transform.find(" ") + 1,
                                           transform.find(")") -
                                               transform.find(" ") - 1))};
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
string SVGElement::get_id() { return id; }
}   // namespace svg
// namespace svg
