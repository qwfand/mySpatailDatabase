#include "Geometry.h"
#include <cmath>
#include <gl/freeglut.h> 

#define NOT_IMPLEMENT -1.0

namespace lab3 {


/*
 * Envelope functions
 */
bool Envelope::contain(double x, double y) const
{
	return x >= minX && x <= maxX && y >= minY && y <= maxY;
}

bool Envelope::contain(const Envelope& envelope) const
{
	// Task 3.1 测试Envelope是否包含关系
	// Write your code here
	if (getMaxX() >= envelope.getMaxX() && getMinX() <= envelope.getMinX() && getMaxY() >= envelope.getMaxY() && getMinY() <= envelope.getMinY())
		return true;
	else
		return false;
}

bool Envelope::intersect(const Envelope& envelope) const
{
	// Task 3.2 测试Envelope是否相交
	// Write your code here
	if (getMaxX() < envelope.getMinX() || getMinX() > envelope.getMaxX() || getMaxY() < envelope.getMinY() || getMinY() > envelope.getMaxY())
		return false;
	else 
		return true;
}

Envelope Envelope::unionEnvelope(const Envelope& envelope) const
{
	// Task 3.3 合并两个Envelope生成一个新的Envelope
	// Write your code here
	double maxX1;
	maxX1 = (maxX >= envelope.getMaxX()) ? maxX: envelope.getMaxX();
	double minX1;
	minX1 = (minX <= envelope.getMinX()) ? minX : envelope.getMinX();
	double maxY1;
	maxY1= (maxY >= envelope.getMaxY()) ? maxY : envelope.getMaxY();
	double minY1;
	minY1 = (minY <= envelope.getMinY()) ? minY : envelope.getMinY();
	//cout << minX1 << " " << maxX1 << " " << minY1 << " " << maxY1 << endl;
	Envelope envelope1(minX1, maxX1, minY1, maxY1);
	return envelope1;
}

void Envelope::draw() const
{
	glBegin(GL_LINE_STRIP);

	glVertex2d(minX, minY);
	glVertex2d(minX, maxY);
	glVertex2d(maxX, maxY);
	glVertex2d(maxX, minY);
	glVertex2d(minX, minY);

	glEnd();
}


/*
 * Points functions
 */
double Point::distanceOnSphere(const Point* point) const
{
	// Task 8 计算两点之间的球面距离(km)
	double R  = 6367;
	double PI = 3.14159265358979323846;
	// Write your code here
	double x1 = x / 20037508.34 * 180;
	double y1 = y / 20037508.34 * 180;
	y1 = 180 / PI * (2 * atan(exp(y1*PI / 180)) -PI / 2);
	cout << x1 << " " << y1 << "@@@@";

	double X1 = point->getX() / 20037508.34 * 180;
	double Y1 = point->getY() / 20037508.34 * 180;
	Y1 = 180 / PI * (2 * atan(exp(Y1*PI / 180)) - PI / 2);

	double dist = R * acos(cos(x1)*cos(X1)*cos(Y1 - y1) + sin(x1)*sin(X1));
	//dist = 2 * R*asin(distance(point) / R);
	return dist;
	return NOT_IMPLEMENT;
}

double Point::distance(const Point* point) const
{
	return sqrt((x - point->x) * (x - point->x) + (y - point->y) * (y - point->y));
}

double Point::distance(const LineString* line) const
{
	double mindist = line->getPointN(0).distance(this);
	for (size_t i = 0; i < line->numPoints() - 1; ++i) {
		double dist = 0;
		double x1 = line->getPointN(i).getX();
		double y1 = line->getPointN(i).getY();
		double x2 = line->getPointN(i + 1).getX();
		double y2 = line->getPointN(i + 1).getY();
		// Task 4.1 calculate the distance between Point P(x, y) and Line [P1(x1, y1), P2(x2, y2)] (less than 10 lines)
		// Write your code here
		if (x1 == x2)
		{
			if ((y >= y1 && y <= y2) || (y <= y1 && y >= y2))
				dist =  ((x - x1) > 0) ? x - x1 : x1 - x;
			else
			{
				Point p1(x1, y1), p2(x2, y2);
				dist = (distance(&p1) < distance(&p2)) ? distance(&p1) : distance(&p2);
			}
		}
		else if (y1 == y2)
		{
			if ((x >= x1 && x <= x2) || (x <= x1 && x >= x2))
				dist = ((y - y1) >= 0) ? y - y1 : y1 - y;
			else
			{
				Point p1(x1, y1), p2(x2, y2);
				dist = (distance(&p1) < distance(&p2)) ? distance(&p1) : distance(&p2);
			}
		}
		else
		{
			double Y = y2 - y1, X = x2 - x1;
			double y0 = (y1*X*X+Y*Y*y+X*Y*x-X*Y*x1) / (X*X + Y*Y);
			double x0 = (X/ Y * (y0 - y1) + x1);
			Point p(x0, y0);
			Point p1(x1, y1), p2(x2, y2);
			Envelope e(x1 < x2 ? x1 : x2, x1 < x2 ? x2 : x1, y1 < y2 ? y1 : y2, y1 < y2 ? y2 : y1);
			if (e.contain(x0, y0))
				dist = distance(&p);
			else
				dist= (distance(&p1) < distance(&p2)) ? distance(&p1) : distance(&p2);
		}
			if (dist < mindist)
			mindist = dist;
	}
	return mindist;
}

double Point::distance(const Polygon* polygon) const
{
	LineString line = polygon->getExteriorRing();
	size_t n = line.numPoints();

	bool inPolygon = false;
	// Task 4.2 whether Point P(x, y) is within Polygon (less than 15 lines)
	// write your code here
	int crossings = 0;
	for (size_t i = 0; i < line.numPoints() - 1; i++) {
		double x1 = line.getPointN(i).getX();
		double y1 = line.getPointN(i).getY();
		double x2 = line.getPointN(i + 1).getX();
		double y2 = line.getPointN(i + 1).getY();
		double slope = (y2 - y1) / (x2 - x1);
		bool cond1 = (x1 <= x) && (x < x2);
		bool cond2 = (x2 <= x) && (x < x1);
		bool above = (y < slope*(x - x1) + y1);
		if ((cond1 || cond2) && above)
			crossings++;
	}
	if (crossings % 2 != 0)
		inPolygon = true;
	if (crossings == 0)
		inPolygon = false;
	double mindist = 0;
	if (!inPolygon)
		mindist = this->distance(&line);	
	return mindist;
}

bool Point::intersects(const Envelope& rect)  const
{
	return (x >= rect.getMinX()) && (x <= rect.getMaxX()) && (y >= rect.getMinY()) && (y <= rect.getMaxY());
}

void Point::draw()  const
{
	glBegin(GL_POINTS);
	glVertex2d(x, y);
	glEnd();
}


/*
 * LineString functions
 */
void LineString::constructEnvelope()
{
	double minX, minY, maxX, maxY;
	maxX = minX = points[0].getX();
	maxY = minY = points[0].getY();
	for (size_t i = 1; i < points.size(); ++i) {
		maxX = max(maxX, points[i].getX());
		maxY = max(maxY, points[i].getY());
		minX = min(minX, points[i].getX());
		minY = min(minY, points[i].getY());
	}
	envelope = Envelope(minX, maxX, minY, maxY);
}

double LineString::distance(const LineString* line) const
{
	//cout << "to be implemented: LineString::distance(const LineString* line)\n";
	int numberOfPoint1, numberOfPoint2;
	numberOfPoint1 = this->numPoints();
	numberOfPoint2 = line->numPoints();
	//判断是否相交
	double x1, y1, x2, y2, x3, y3, x4, y4;
	bool ISInterset = false;
	for (int i = 0; i < numberOfPoint1 - 1; i++)
	{
		x1 = this->getPointN(i).getX(), y1 = this->getPointN(i).getY();
		x2 = this->getPointN(i + 1).getX(), y2 = this->getPointN(i + 1).getY();
		for (int j = 0; j < numberOfPoint2 - 1; j++)
		{
			x3 = line->getPointN(j).getX(), y3 = line->getPointN(j).getY();
			x4 = line->getPointN(j + 1).getY(), y4 = line->getPointN(j + 1).getY();

			if (!(min(x1, x2) <= max(x3, x4) && min(y3, y4) <= max(y1, y2) && min(x3, x4) <= max(x1, x2) && min(y1, y2) <= max(y3, y4)))
				ISInterset = false;

			double fc = (y3 - y1) * (x2 - x1) - (x3 - x1) *(y2 - y1);
			double fd = (y4 - y1) * (x2 - x1) - (x4 - x1) *(y2 - y1);

			if (fc * fd > 0)
				ISInterset = false;
			else
				ISInterset = true;
			if (ISInterset == true)
				break;
		}
		if (ISInterset == true)
			break;
	}
	double distance, mindist;
	if (ISInterset)//如果相交距离为0
		mindist = 0;
	else
	{
		mindist = this->getPointN(0).distance(line);
		for (int i = 1; i < numberOfPoint1; i++)
		{
			distance = getPointN(i).distance(line);
			if (distance < mindist)
				mindist = distance;
		}
		for (int i = 0; i < numberOfPoint2; i++)
		{
			distance = line->getPointN(i).distance(this);
			if (distance < mindist)
				mindist = distance;
		}
	}
	return mindist;
}

double LineString::distance(const Polygon* polygon) const
{
	//cout << "to be implemented: LineString::distance(const Polygon* polygon)\n";
	//判断线是不是在多边形内
	bool inPolygon = false;
	int number = this->numPoints();
	LineString line = polygon->getExteriorRing();
	for (int i = 0; i < number; i++)
	{
		double x = this->getPointN(i).getX(), y = this->getPointN(i).getY();
		int crossings = 0;
		for (size_t i = 0; i <line.numPoints() - 1; i++) {
			double x1 = line.getPointN(i).getX();
			double y1 = line.getPointN(i).getY();
			double x2 = line.getPointN(i + 1).getX();
			double y2 = line.getPointN(i + 1).getY();
			double slope = (y2 - y1) / (x2 - x1);
			bool cond1 = (x1 <= x) && (x < x2);
			bool cond2 = (x2 <= x) && (x < x1);
			bool above = (y < slope*(x - x1) + y1);
			if ((cond1 || cond2) && above)
				crossings++;
		}
		if (crossings % 2 != 0)
			inPolygon = true;
		if (crossings == 0)
			inPolygon = false;
		if (inPolygon == true)
			break;
	}
	double dist;
	if (inPolygon)//在多边形内部，距离为0
		dist = 0;
	else
		dist = this->distance(&line);
	return dist;
}

typedef int OutCode;

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// Compute the bit code for a point (x, y) using the clip rectangle
// bounded diagonally by (xmin, ymin), and (xmax, ymax)
// ASSUME THAT xmax, xmin, ymax and ymin are global constants.
OutCode ComputeOutCode(double x, double y, double xmin, double xmax, double ymin, double ymax)
{
	OutCode code;

	code = INSIDE;          // initialised as being inside of [[clip window]]

	if (x < xmin)           // to the left of clip window
		code |= LEFT;
	else if (x > xmax)      // to the right of clip window
		code |= RIGHT;
	if (y < ymin)           // below the clip window
		code |= BOTTOM;
	else if (y > ymax)      // above the clip window
		code |= TOP;
	
	return code;
}

// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
// diagonal from (xmin, ymin) to (xmax, ymax).
bool intersectTest(double x0, double y0, double x1, double y1, double xmin, double xmax, double ymin, double ymax)
{
	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	OutCode outcode0 = ComputeOutCode(x0, y0, xmin, xmax, ymin, ymax);
	OutCode outcode1 = ComputeOutCode(x1, y1, xmin, xmax, ymin, ymax);
	bool accept = false;

	while (true) {
		if (!(outcode0 | outcode1)) {
			// bitwise OR is 0: both points inside window; trivially accept and exit loop
			accept = true;
			break;
		}
		else if (outcode0 & outcode1) {
			// bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
			// or BOTTOM), so both must be outside window; exit loop (accept is false)
			break;
		}
		else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			double x, y;

			// At least one endpoint is outside the clip rectangle; pick it.
			OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

			// Now find the intersection point;
			// use formulas:
			//   slope = (y1 - y0) / (x1 - x0)
			//   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
			//   y = y0 + slope * (xm - x0), where xm is xmin or xmax
			// No need to worry about divide-by-zero because, in each case, the
			// outcode bit being tested guarantees the denominator is non-zero
			if (outcodeOut & TOP) {           // point is above the clip window
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			}
			else if (outcodeOut & BOTTOM) { // point is below the clip window
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			}
			else if (outcodeOut & RIGHT) {  // point is to the right of clip window
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = xmax;
			}
			else if (outcodeOut & LEFT) {   // point is to the left of clip window
				y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
				x = xmin;
			}
			
			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = ComputeOutCode(x0, y0, xmin, xmax, ymin, ymax);
			}
			else {
				x1 = x;
				y1 = y;
				outcode1 = ComputeOutCode(x1, y1, xmin, xmax, ymin, ymax);
			}
		}
	}
	return accept;
}

bool LineString::intersects(const Envelope& rect)  const
{
	double xmin = rect.getMinX();
	double xmax = rect.getMaxX();
	double ymin = rect.getMinY();
	double ymax = rect.getMaxY();

	for (size_t i = 1; i < points.size(); ++i)
		if (intersectTest(points[i - 1].getX(), points[i - 1].getY(), points[i].getX(), points[i].getY(), xmin, xmax, ymin, ymax))
			return true;
	return false;
}

void LineString::draw()  const
{
	glBegin(GL_LINE_STRIP);
	for (size_t i = 0; i < points.size(); ++i)
		glVertex2d(points[i].getX(), points[i].getY());
	glEnd();
}

void LineString::print() const
{
	cout << "LineString(";
	for (size_t i = 0; i < points.size(); ++i) {
		if (i != 0)
			cout << ", ";
		cout << points[i].getX() << " " << points[i].getY();
	}
	cout << ")";
}

/*
 * Polygon
 */
double Polygon::distance(const Polygon* polygon) const
{
	return min(exteriorRing.distance(polygon), polygon->getExteriorRing().distance(this));
}

bool Polygon::intersects(const Envelope& rect)  const
{
	//cout << "to be implemented: Polygon::intersects(const Envelope& box)\n";
	bool IsInterset = false;
	LineString polyLine = this->getExteriorRing();
	for (int i = 0; i < polyLine.numPoints(); i++)
	{
		if(rect.intersect(polyLine.getPointN(i).getEnvelope()))
			IsInterset=true;
	}
	return IsInterset;
	/*
    IsInterset=this->getEnvelope().intersect(rect);
	if (IsInterset)
		return true;
	else 
		return false;*/
}

void Polygon::draw() const
{
	exteriorRing.draw();
}

void Polygon::print() const
{
	cout << "Polygon(";
	for (size_t i = 0; i < exteriorRing.numPoints(); ++i) {
		if (i != 0)
			cout << ", ";
		Point p = exteriorRing.getPointN(i);
		cout << p.getX() << " " << p.getY();
	}
	cout << ")";
}
	
}
