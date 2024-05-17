#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "PNGImage.hpp"
#include "Point.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <unordered_map>
using namespace std;

namespace svg {

//! Base class for SVG elements.
class SVGElement {
  public:
    //! Default constructor for SVGElement.
    SVGElement();

    //! Virtual destructor for SVGElement.
    virtual ~SVGElement();

    //! Draws the SVG element on the given PNG image.
    //! @param img The PNG image to draw on.
    virtual void draw(PNGImage &img) const = 0;

    //! Transforms the SVG element using the specified transform string
    //! and origin.
    //! @param transform_string The transform string.
    //! @param transform_origin The origin of the transformation.
    virtual void transform(string transform_string, Point transform_origin) = 0;

    //! Creates a clone of the SVG element.
    //! @return A pointer to the cloned SVG element.
    virtual SVGElement *clone() const = 0;
};

//! Reads an SVG file and extracts its dimensions and SVG elements.
//! @param svg_file The path to the SVG file.
//! @param dimensions The dimensions of the SVG file.
//! @param svg_elements The vector to store the SVG elements.
void readSVG(const string &svg_file, Point &dimensions,
             vector<SVGElement *> &svg_elements);

//! Parses an XML element and creates the corresponding SVG element.
//! @param child The XML element to parse.
//! @param shapes The vector to store the parsed SVG elements.
//! @param dictionary The dictionary to store the SVG elements by ID.
void parseElement(tinyxml2::XMLElement *child,
                  vector<svg::SVGElement *> &shapes,
                  unordered_map<string, SVGElement *> &dictionary);

//! Converts an SVG file to a PNG file.
//! @param svg_file The path to the SVG file.
//! @param png_file The path to the PNG file.
void convert(const string &svg_file, const string &png_file);

//! @class Ellipse
//! Represents an SVG ellipse element.
class Ellipse : public SVGElement {
  public:
    //! Constructor for Ellipse.
    //! @param fill The fill color of the ellipse.
    //! @param center The center point of the ellipse.
    //! @param radius The radius of the ellipse.
    Ellipse(const Color &fill, const Point &center, const Point &radius);

    //! Draws the ellipse on the given PNG image.
    //! @param img The PNG image to draw on.
    void draw(PNGImage &img) const override;

    //! Transforms the ellipse using the specified transform string and
    //! origin.
    //! @param transform_string The transform string.
    //! @param transform_origin The origin of the transformation.
    void transform(string transform_string, Point transform_origin) override;

    //! Creates a clone of the ellipse.
    //! @return A pointer to the cloned ellipse.
    SVGElement *clone() const override;

  private:
    Color fill;     //! The fill color of the ellipse.Point center;
    Point center;   //! The center point of the ellipse.Point radius;
    Point radius;   //! The radius of the ellipse.
};

//! @class Polygon
//! Represents an SVG polygon element.
class Polygon : public SVGElement {
  public:
    //! Constructor for Polygon.
    //! @param fill The fill color of the polygon.
    //! @param points The points that define the polygon.
    Polygon(const Color &fill, const vector<Point> &points);

    //! Draws the polygon on the given PNG image.
    //! @param img The PNG image to draw on.
    void draw(PNGImage &img) const override;

    //! Transforms the polygon using the specified transform string and
    //! origin.
    //! @param transform_string The transform string.
    //! @param transform_origin The origin of the transformation.
    void transform(string transform_string, Point transform_origin) override;

    //! Creates a clone of the polygon.
    //! @return A pointer to the cloned polygon.
    SVGElement *clone() const override;

  private:
    Color fill;   //! The fill color of the polygon.vector<Point> points;
    vector<Point> points;   //! The points that define the polygon.
};

//! @class Polyline
//! Represents an SVG polyline element.
class Polyline : public SVGElement {
  public:
    //! Constructor for Polyline.
    //! @param stroke The stroke color of the polyline.
    //! @param points The points that define the polyline.
    Polyline(const Color &stroke, const vector<Point> &points);

    //! Draws the polyline on the given PNG image.
    //! @param img The PNG image to draw on.
    void draw(PNGImage &img) const override;

    //! Transforms the polyline using the specified transform string and
    //! origin.
    //! @param transform_string The transform string.
    //! @param transform_origin The origin of the transformation.
    void transform(string transform_string, Point transform_origin) override;

    //! Creates a clone of the polyline.
    //! @return A pointer to the cloned polyline.
    SVGElement *clone() const override;

  private:
    Color stroke;           //!  The stroke color of the polyline.
    vector<Point> points;   //! The points that define the polyline.
};

//! @class Group
//! Represents an SVG group element.
class Group : public SVGElement {
  public:
    //! Constructor for Group.
    //! @param elements The SVG elements contained in the group.
    Group(const vector<SVGElement *> &elements);

    //! Destructor for Group.
    ~Group();

    //! Draws the group on the given PNG image.
    //! @param img The PNG image to draw on.
    void draw(PNGImage &img) const override;

    //! Adds an SVG element to the group.
    //! @param element The SVG element to add.
    void addElement(SVGElement *element);

    //! Transforms the group using the specified transform string and
    //! origin.
    //! @param transform_string The transform string.
    //! @param transform_origin The origin of the transformation.
    void transform(string transform_string, Point transform_origin) override;

    //! Creates a clone of the group.
    //! @return A pointer to the cloned group.
    SVGElement *clone() const override;

  private:
    vector<SVGElement *> elements;
    //! The SVG elements contained in the group.
};

//! @class Use
//! Represents an SVG use element.
class Use : public SVGElement {
  public:
    //! Constructor for Use.
    //! @param element The SVG element to use.
    Use(SVGElement *element);

    //! Destructor for Use.
    ~Use();

    //! Draws the used SVG element on the given PNG image.
    //! @param img The PNG image to draw on.
    void draw(PNGImage &img) const override;

    //! Transforms the used SVG element using the specified transform
    //! string and origin.
    //! @param transform_string The transform string.
    //! @param transform_origin The origin of the transformation.
    void transform(string transform_string, Point transform_origin) override;

    //! Creates a clone of the use element.
    //! @return A pointer to the cloned use element.
    SVGElement *clone() const override;

  private:
    SVGElement *element;
    //! The SVG element to use.
};
}   // namespace svg
#endif
