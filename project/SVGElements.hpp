// CAN BE MODIFIED
//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "PNGImage.hpp"
#include "Point.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <unordered_map>
using namespace std;

namespace svg {
class SVGElement {

  public:
    SVGElement();
    virtual ~SVGElement();
    virtual void draw(PNGImage &img) const = 0;
    virtual void transform(string transform_string, Point transform_origin) = 0;
    virtual SVGElement *clone() const = 0;
};

// Declaration of namespace functions
// readSVG -> implement it in readSVG.cpp
// convert -> already given (DO NOT CHANGE) in convert.cpp

void readSVG(const std::string &svg_file, Point &dimensions,
             std::vector<SVGElement *> &svg_elements);
void parseElement(tinyxml2::XMLElement *child,
                  std::vector<svg::SVGElement *> &shapes,
                  unordered_map<string, SVGElement *> &dictionary);
void convert(const string &svg_file, const std::string &png_file);

class Ellipse : public SVGElement {
  public:
    Ellipse(const Color &fill, const Point &center, const Point &radius);
    void draw(PNGImage &img) const override;
    void transform(string transform_string, Point transform_origin) override;
    SVGElement *clone() const override;

  private:
    Color fill;
    Point center;
    Point radius;
};
class Polygon : public SVGElement {
  public:
    Polygon(const Color &fill, const std::vector<Point> &points);
    void draw(PNGImage &img) const override;
    void transform(string transform_string, Point transform_origin) override;
    SVGElement *clone() const override;

  private:
    Color fill;
    std::vector<Point> points;
};

class Polyline : public SVGElement {
  public:
    Polyline(const Color &stroke, const std::vector<Point> &points);
    void draw(PNGImage &img) const override;
    void transform(string transform_string, Point transform_origin) override;
    SVGElement *clone() const override;

  private:
    Color stroke;
    std::vector<Point> points;
};

class Group : public SVGElement {
  public:
    Group(const std::vector<SVGElement *> &elements);
    ~Group();
    void draw(PNGImage &img) const override;
    void addElement(SVGElement *element);
    void transform(string transform_string, Point transform_origin) override;
    SVGElement *clone() const override;

  private:
    std::vector<SVGElement *> elements;
};

class Use : public SVGElement {
  public:
    Use(SVGElement *element);
    ~Use();
    void draw(PNGImage &img) const override;
    void transform(string transform_string, Point transform_origin) override;
    SVGElement *clone() const override;

  private:
    SVGElement *element;
};
}   // namespace svg
#endif
