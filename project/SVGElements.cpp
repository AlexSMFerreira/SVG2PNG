#include "SVGElements.hpp"
#include <iostream>
#include <vector>

namespace svg {

//! Constructor for the Group class.
Group::Group(const std::vector<SVGElement *> &elements) {
    for (SVGElement *element : elements) {
        this->elements.push_back(element);
    }
}

//! Draw function for the Group class.
void Group::draw(PNGImage &img) const {
    for (SVGElement *element : elements) {
        element->draw(img);
    }
}

//! Transform function for the Group class.
void Group::transform(string transform, Point origin) {
    for (SVGElement *element : elements) {
        element->transform(transform, origin);
    }
}

//! Clone function for the Group class.
SVGElement *Group::clone() const {
    std::vector<SVGElement *> cloned_elements;
    for (SVGElement *element : elements) {
        cloned_elements.push_back(element->clone());
    }
    return new Group(cloned_elements);
}

//! Destructor for the Group class.
Group::~Group() {
    for (SVGElement *element : elements) {
        delete element;
    }
}

//! Constructor for the Use class.
Use::Use(SVGElement *element) { this->element = element->clone(); }

//! Destructor for the Use class.
Use::~Use() { delete element; }

//! Draw function for the Use class.
void Use::draw(PNGImage &img) const { element->draw(img); }

//! Transform function for the Use class.
void Use::transform(string transform, Point origin) {
    element->transform(transform, origin);
}

//! Clone function for the Use class.
SVGElement *Use::clone() const { return new Use(element); }

//! Default constructor for the SVGElement class.
SVGElement::SVGElement() {}

//! Destructor for the SVGElement class.
SVGElement::~SVGElement() {}

//! Constructor for the Ellipse class.
Ellipse::Ellipse(const Color &fill, const Point &center, const Point &radius)
    : fill(fill), center(center), radius(radius) {}

//! Draw function for the Ellipse class.
void Ellipse::draw(PNGImage &img) const {
    img.draw_ellipse(center, radius, fill);
}

//! Transform function for the Ellipse class.
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
}

//! Clone function for the Ellipse class.
SVGElement *Ellipse::clone() const { return new Ellipse(fill, center, radius); }

//! Constructor for the Polygon class.
Polygon::Polygon(const Color &fill, const std::vector<Point> &points)
    : fill(fill), points(points) {}

//! Draw function for the Polygon class.
void Polygon::draw(PNGImage &img) const { img.draw_polygon(points, fill); }

//! Transform function for the Polygon class.
void Polygon::transform(string transform, Point origin) {
    Point translate;
    int r_angle;
    int scale_factor;
    //! Translate
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
    //! Rotate
    if (transform.find("rotate") != string::npos) {
        r_angle = stoi(
            transform.substr(transform.find("(") + 1,
                             transform.find(")") - transform.find("(") - 1));
        for (Point &p : points) {
            p = p.rotate(origin, r_angle);
        }
    }
    //! Scale
    if (transform.find("scale") != string::npos) {
        scale_factor = stoi(
            transform.substr(transform.find("(") + 1,
                             transform.find(")") - transform.find("(") - 1));
        for (Point &p : points) {
            p = p.scale(origin, scale_factor);
        }
    }
}

//! Clone function for the Polygon class.
SVGElement *Polygon::clone() const { return new Polygon(fill, points); }

//! Constructor for the Polyline class.
Polyline::Polyline(const Color &stroke, const std::vector<Point> &points)
    : stroke(stroke), points(points) {}

//! Draw function for the Polyline class.
void Polyline::draw(PNGImage &img) const {
    for (size_t i = 0; i < points.size() - 1; i++) {
        img.draw_line(points[i], points[i + 1], stroke);
    }
}

//! Transform function for the Polyline class.
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

//! Clone function for the Polyline class.
SVGElement *Polyline::clone() const { return new Polyline(stroke, points); }

}   // namespace svg