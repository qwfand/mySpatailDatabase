#include "common.h"
#include "Geometry.h"
#include "Curve.h"
#include "time.h"

using namespace lab3;

extern int mode;
extern vector<Geometry *> readGeom(const char *filename);
extern vector<string> readName(const char* filename);

void transformValue(double &res, const char* format = "%.2lf"){

	char buf[20];
	sprintf(buf, format, res);
	sscanf(buf, "%lf", &res);
}

void wrongMessage(Envelope e1, Envelope e2, bool cal)
{
	cout << "Your answer is " << cal << " for test between ";
	e1.print();
	cout << " and ";
	e2.print();
	cout << ", but the answer is " << !cal << endl;
}

void wrongMessage(const Point& pt1, const Point& pt2, double dis, double res)
{
	cout << "Your answer is " << dis << " for test ";
	pt1.print();
	cout << " and ";
	pt2.print();
	cout << ", but the answer is " << res << endl;
}

void wrongMessage(Envelope e1, Envelope e2, Envelope cal, Envelope res)
{
	cout << "Your answer is ";
	cal.print();
	cout << " for test between ";
	e1.print();
	cout << " and ";
	e2.print();
	cout << ", but the answer is ";
	res.print();
	cout << endl;
}

void QuadTreeAnalysis()
{
	vector<Feature> features;
	vector<Geometry *> geom = readGeom(".//data/taxi");
	vector<string> name = readName(".//data/taxi");

	features.clear();
	features.reserve(geom.size());
	for (size_t i = 0; i < geom.size(); ++i)
		features.push_back(Feature(name[i], geom[i]));

	cout << "taxi number: " << geom.size() << endl;

	srand(time(NULL));
	for (int cap = 70; cap <= 200; cap += 10) {
		QuadTree *qtree = new QuadTree();
		// Task 9 ¹¹ÔìËÄ²æÊ÷£¬Êä³öËÄ²æÊ÷µÄ½ÚµãÊýÄ¿ºÍ¸ß¶È
		// Write your code here
		qtree->setCapacity(cap);
		
		clock_t start_time = clock();
		// Write your code here
		qtree->constructQuadTree(features);
		clock_t end_time = clock();

		int height = 0, interiorNum = 0, leafNum = 0;
		// Write your code here
		qtree->countHeight(height);
		qtree->countQuadNode(interiorNum, leafNum);

		cout << "Capacity " << cap << "\n";
		cout << "Height: " << height << " \tInterior node number: " << interiorNum << " \tLeaf node number: " << leafNum << "\n";
		cout << "Construction time: " << (end_time - start_time) / 1000.0 << "s" << endl;

		double x, y;
		Feature f;
		start_time = clock();
		for (int i = 0; i < 100000; ++i) {
			x = -((rand() % 225) / 10000.0 + 73.9812);
			y = (rand() % 239) / 10000.0 + 40.7247;
			qtree->NNQuery(x, y, f);
		}
		end_time = clock();
		cout << "NNQuery time: " << (end_time - start_time) / 1000.0 << "s" << endl << endl;

		delete qtree;
	}
}

void test(int t)
{
	cout << "*********************Start*********************" << endl;
	
	if (t == TEST1) { 
		cout << "²âÊÔ1: Z-Curve" << endl;

		int coor[2];
		int order = 2;
		int value = 0;
		int num = int(pow(2, order));
		cout << "example of Z-Curver in order = 2" << endl;
		for (int y = num - 1; y >= 0; --y) {
			for (int x = 0; x < num; ++x) {
				coor[0] = x;
				coor[1] = y;
				zorder(order, value, coor);
				cout << value << " ";
			}
			cout << endl;
		}
		for (int y = 0; y < num; ++y) {
			for (int x = 0; x < num; ++x) {
				value = y * num + x;
				izorder(order, value, coor);
				cout << value << " --> (" << coor[0] << ", " << coor[1] << ")\t";
			}
			cout << endl;
		}
		cout << endl;

		int failedZorderTest  = 0;
		int failediZorderTest = 0;
		int orders[10] = { 2, 2, 3, 3, 4, 4, 5, 5, 6, 7 };
		int values[10] = { 7, 14, 16, 28, 35, 51, 79, 84, 200, 411 };
		int coorx[10]  = { 1, 3, 0, 2, 5, 5, 3, 0, 10, 11 };
		int coory[10]  = { 3, 2, 4, 6, 1, 5, 11, 14, 8, 21 };
		for (int i = 0; i < 10; ++i) {
			coor[0] = coorx[i];
			coor[1] = coory[i];
			zorder(orders[i], value, coor);
			if (value != values[i]) {
				failedZorderTest += 1;
				cout << "zorder test failed, order = " << orders[i] << ", coor = (" << coor[0] << ", " << coor[1] << "), your value is " << value << ", expected value is " << values[i] << endl;
			}

			izorder(orders[i], values[i], coor);
			if (coor[0] != coorx[i] || coor[1] != coory[i]) {
				failediZorderTest += 1;
				cout << "izorder test failed, order = " << orders[i] << ", value  = " << values[i] << ", your coor is  (" << coor[0] << ", " << coor[1] << "), expected coor is (" << coorx[i] << ", " << coory[i] << ")" << endl;
			}
		}
		cout << "zorder: "  << 10 - failedZorderTest  << " / " << 10 << " tests are passed" << endl;
		cout << "izorder: " << 10 - failediZorderTest << " / " << 10 << " tests are passed" << endl;

		int zdist2 = zdist(2);
		if (zdist2 != 49)
			cout << "pair of distances in Z-Curve (2) test failed: " << zdist2 << " (expected 49)" << endl;

		int zdist4 = zdist(4);
		if (zdist4 != 1097)
			cout << "pair of distances in Z-Curve (4) test failed: " << zdist4 << " (expected 1097)" << endl;

		int zdist6 = zdist(6);
		if (zdist6 != 18729)
			cout << "pair of distances in Z-Curve (6) test failed: " << zdist6 << " (expected 18729)" << endl;
	} 
	else if (t == TEST2) {
		cout << "²âÊÔ2: Hilbert Curve" << endl;

		int coor[2];
		int order = 2;
		int value = 0;
		int num = int(pow(2, order));
		cout << "example of Hilbert Curver in order = 2" << endl;
		for (int y = num - 1; y >= 0; --y) {
			for (int x = 0; x < num; ++x) {
				coor[0] = x;
				coor[1] = y;
				horder(order, value, coor);
				cout << value << " ";
			}
			cout << endl;
		}
		for (int y = 0; y < num; ++y) {
			for (int x = 0; x < num; ++x) {
				value = y * num + x;
				ihorder(order, value, coor);
				cout << value << " --> (" << coor[0] << ", " << coor[1] << ")\t";
			}
			cout << endl;
		}
		cout << endl;

		int failedHorderTest = 0;
		int failediHorderTest = 0;
		int orders[10] = { 2, 2, 3, 3, 4, 4, 5, 5, 6, 7 };
		int values[10] = { 6, 11, 19, 24, 18, 34, 224, 254, 132, 478 };
		int coorx[10] = { 1, 3, 0, 2, 5, 5, 3, 0, 10, 11 };
		int coory[10] = { 3, 2, 5, 6, 1, 5, 11, 14, 8, 21 };
		for (int i = 0; i < 10; ++i) {
			coor[0] = coorx[i];
			coor[1] = coory[i];
			horder(orders[i], value, coor);
			if (value != values[i]) {
				failedHorderTest += 1;
				cout << "horder test failed, order = " << orders[i] << ", coor = (" << coor[0] << ", " << coor[1] << "), your value is " << value << ", expected value is " << values[i] << endl;
			}

			ihorder(orders[i], values[i], coor);
			if (coor[0] != coorx[i] || coor[1] != coory[i]) {
				failediHorderTest += 1;
				cout << "ihorder test failed, order = " << orders[i] << ", value  = " << values[i] << ", your coor is  (" << coor[0] << ", " << coor[1] << "), expected coor is (" << coorx[i] << ", " << coory[i] << ")" << endl;
			}
		}
		cout << "horder: " << 10 - failedHorderTest << " / " << 10 << " tests are passed" << endl;
		cout << "ihorder: " << 10 - failediHorderTest << " / " << 10 << " tests are passed" << endl;
		
		int hdist2 = hdist(2);
		if (hdist2 != 42)
			cout << "pair of distances in Hilbert Curve (2) test failed: " << hdist2 << " (expected 42)" << endl;

		int hdist4 = hdist(4);
		if (hdist4 != 762)
			cout << "pair of distances in Hilbert Curve (4) test failed: " << hdist4 << " (expected 762)" << endl;

		int hdist6 = hdist(6);
		if (hdist6 != 12282)
			cout << "pair of distances in Hilbert Curve (6) test failed: " << hdist6 << " (expected 12282)" << endl;
	}
	else if (t == TEST3) {
		cout << "²âÊÔ3: Envelope Contain, Intersect, and Union" << endl;

		int failedCase = 0;
		Envelope e1(-1, 1, -1, 1);
		vector<Envelope> tests;
		tests.push_back(Envelope(-0.5, 0.5, -0.5, 0.5));
		tests.push_back(Envelope(-0.5, 0.5, 0.5, 1.5));
		tests.push_back(Envelope(0.5, 1.5, -0.5, 0.5));
		tests.push_back(Envelope(-1.5, -0.5, -1.5, -0.5));
		tests.push_back(Envelope(-2, -1, -0.5, 0.5));
		tests.push_back(Envelope(1, 1.5, 1, 1.5));
		tests.push_back(Envelope(-2, -1.5, -0.5, 0.5));
		tests.push_back(Envelope(-0.5, 0.5, 1.5, 2));
		tests.push_back(Envelope(-2, -1.5, 0.5, 1.5));
		tests.push_back(Envelope(0.5, 1.5, 1.5, 2));

		for (size_t i = 0; i < tests.size(); ++i) {
			if (e1.contain(tests[i]) != (i == 0)) {
				failedCase += 1;
				wrongMessage(e1, tests[i], (i != 0));
			}
			if (tests[i].contain(e1) == true) {
				failedCase += 1;
				wrongMessage(tests[i], e1, true);
			}
		}
		cout << "Envelope Contain: " << tests.size() * 2 - failedCase << " / " << tests.size() * 2 << " tests are passed" << endl;

		failedCase = 0;
		for (size_t i = 0; i < tests.size(); ++i) {
			if (e1.intersect(tests[i]) != (i < 6)) {
				failedCase += 1;
				wrongMessage(e1, tests[i], (i >= 6));
			}
			if (tests[i].intersect(e1) != (i < 6)) {
				failedCase += 1;
				wrongMessage(tests[i], e1, (i >= 6));
				cout << tests[i].intersect(e1) << "  ****" << endl;
			}
		}
		cout << "Envelope Intersect: " << tests.size() * 2 - failedCase << " / " << tests.size() * 2 << " tests are passed" << endl;

		failedCase = 0;
		vector<Envelope> results;
		results.push_back(Envelope(-1, 1, -1, 1));
		results.push_back(Envelope(-1, 1, -1, 1.5));
		results.push_back(Envelope(-1, 1.5, -1, 1));
		results.push_back(Envelope(-1.5, 1, -1.5, 1));
		results.push_back(Envelope(-2, 1, -1, 1));
		results.push_back(Envelope(-1, 1.5, -1, 1.5));
		results.push_back(Envelope(-2, 1, -1, 1));
		results.push_back(Envelope(-1, 1, -1, 2));
		results.push_back(Envelope(-2, 1, -1, 1.5));
		results.push_back(Envelope(-1, 1.5, -1, 2));
		for (size_t i = 0; i < tests.size(); ++i) {
			if (e1.unionEnvelope(tests[i]) != results[i]) {
				failedCase += 1;
				wrongMessage(e1, tests[i], e1.unionEnvelope(tests[i]), results[i]);
			}
			if (tests[i].unionEnvelope(e1) != results[i]) {
				failedCase += 1;
				wrongMessage(tests[i], e1, e1.unionEnvelope(tests[i]), results[i]);
			}
		}
		cout << "Envelope Union: " << tests.size() * 2 - failedCase << " / " << tests.size() * 2 << " tests are passed" << endl;
	}
	else if (t == TEST4) {
		cout << "²âÊÔ4: Distance between Point and LineString" << endl;

		vector<Point> points;
		points.push_back(Point(0, 0));
		points.push_back(Point(10, 10));
		LineString line(points);

		points.push_back(Point(-10, -10));
		points.push_back(Point(20, 20));
		points.push_back(Point(5, 5));
		points.push_back(Point(10, 0));
		points.push_back(Point(10, -10));
		points.push_back(Point(0, 10));
		points.push_back(Point(0, 20));
		points.push_back(Point(20, 0));

		double dists[] = { 0, 0, 14.1421, 14.1421, 0, 7.07107, 14.1421, 7.07107, 14.1421, 14.1421 };

		int failedCase = 0;
		for (size_t i = 0; i < points.size(); ++i) {
			double dist = points[i].distance(&line);
			if (fabs(dist - dists[i]) > 0.0001) {
				failedCase += 1;
				cout << "Your answer is " << dist << " for test between ";
				line.print();
				cout << " and ";
				points[i].print();
				cout << ", but the answer is " << dists[i] << endl;
			}
		}
		cout << "Distance between Point and LineString: " << points.size() - failedCase << " / " << points.size() << " tests are passed" << endl;
	}
	else if (t == TEST5) {
		cout << "²âÊÔ5: Distance between Point and Polygon" << endl;

		vector<Point> points;
		points.push_back(Point(5, 0));
		points.push_back(Point(3, 6));
		points.push_back(Point(2, 4));
		points.push_back(Point(-2, 4));
		points.push_back(Point(-3, 5));
		points.push_back(Point(-5, 0));
		points.push_back(Point(0, -3));
		points.push_back(Point(5, 0));
		LineString line(points);
		Polygon poly(line);

		points.clear();
		points.push_back(Point(5, 4));
		points.push_back(Point(3, 4));
		points.push_back(Point(0, 4));
		points.push_back(Point(-3, 4));
		points.push_back(Point(-5, 4));
		points.push_back(Point(5, 5));
		points.push_back(Point(3, 5));
		points.push_back(Point(0, 5));
		points.push_back(Point(-3, 5));
		points.push_back(Point(0, 0));

		double dists[] = { 1.26491, 0, 0, 0, 1.48556, 1.58114, 0, 1, 0, 0 };

		int failedCase = 0;
		for (size_t i = 0; i < points.size(); ++i) {
			double dist = points[i].distance(&poly);
			if (fabs(dist - dists[i]) > 0.00001) {
				failedCase += 1;
				cout << "Your answer is " << dist << " for test between ";
				poly.print();
				cout << " and ";
				points[i].print();
				cout << ", but the answer is " << dists[i] << endl;
			}
		}
		cout << "Distance between Point and Polygon: " << points.size() - failedCase << " / " << points.size() << " tests are passed" << endl;
	}
	else if (t == TEST6) {
		cout << "²âÊÔ6: QuadTree Construction" << endl;
		int ncase, cct;
		ncase = cct = 2;
		QuadTree qtree;
		vector<Geometry *> geom = readGeom(".//data/polygon");
		vector<Feature> features;

		for (size_t i = 0; i < geom.size(); ++i)
			features.push_back(Feature("", geom[i]));

		qtree.setCapacity(1);
		qtree.constructQuadTree(features);

		int height, interiorNum, leafNum;
		qtree.countHeight(height);
		qtree.countQuadNode(interiorNum, leafNum);

		if (!(height == 6 && interiorNum == 8 && leafNum == 25)){
			cout <<"Case 1: "<< "Your answer is height: " << height << ", interiorNum: " << interiorNum <<
				", leafNum: " << leafNum << " for case1, but the answer is height: 6, interiorNum: 8, leafNum: 25\n";
			--cct;
		}

		features.clear();
		for (size_t i = 0; i < geom.size(); ++i)
			delete geom[i];
		geom.clear();

		vector<Geometry *> geom2 = readGeom(".//data/highway");
		vector<Feature> features2;
		QuadTree qtree2;

		for (size_t i = 0; i < geom2.size(); ++i)
			features2.push_back(Feature("", geom2[i]));
		//cout << features2.size() << " hhh" << endl;
		qtree2.setCapacity(20);//20
		qtree2.constructQuadTree(features2);

		int height2, interiorNum2, leafNum2;
		qtree2.countHeight(height2);
		qtree2.countQuadNode(interiorNum2, leafNum2);

	    if (!(height2 == 11 && interiorNum2 == 1386 && leafNum2 == 4159)){
			cout <<"Case 2: "<< "Your answer is height: " << height2 << ", interiorNum: " << interiorNum2 <<
				", leafNum: " << leafNum2 << " for case2, but the answer is height: 11, interiorNum: 1386, leafNum: 4159\n";
			--cct;
		}

		features2.clear();
		for (size_t i = 0; i < geom2.size(); ++i)
			delete geom2[i];
		geom2.clear();

		cout << "QuadTree Construction: " << cct << " / " << ncase << " tests are passed" << endl;
	}
	else if (t == TEST7) {
		cout << "²âÊÔ7: Point DistanceOnSphere" << endl;
		int ncase, cct;
		ncase = cct = 2;

		Point pt1(100.5, 20.7), pt2(100.3, 20.6);
		double dis = pt1.distanceOnSphere(&pt2);
		transformValue(dis);

		if (fabs(dis - 23.58) > 1e-6) {
			--cct;
			wrongMessage(pt1, pt2, dis, 23.58);
		}

		pt1 = Point(90.5, 30.5);
		pt2 = Point(20.7, 99.9);

		dis = pt1.distanceOnSphere(&pt2);
		transformValue(dis);

		if (fabs(dis - 7037.71) > 1e-6) {
			--cct;
			wrongMessage(pt1, pt2, dis, 7037.71);
		}
		cout << "Point DistanceOnSphere: " << cct << " / " << ncase << " tests are passed" << endl;
	}
	else if (t == TEST8) {
		cout << "²âÊÔ8: QuadTreeAnalysis" << endl;
		QuadTreeAnalysis();
	}

	cout << "**********************End**********************" << endl;
}