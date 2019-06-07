#include "KMeans.h"
#include <gl/freeglut.h> 

/*
 * Cluster functions
 */
bool Cluster::resetMean()
{
	double x = 0, y = 0, _x = mean.getX(), _y = mean.getY();

	// Task 14.2
	// Write your code here

	mean = Point(x, y);

	
	return (!(fabs(mean.getX() - _x) < 1e-6 && fabs(mean.getY() - _y) < 1e-6));
}

void Cluster::draw()
{
	for (size_t i = 0; i < features.size(); ++i)
		features[i].draw();
}

void Cluster::print()
{
	cout << "mean is (";
	mean.print();
	cout << ")";
	for (size_t i = 0; i < features.size(); ++i)
		features[i].print();
}

/*
 * KMeans functions
 */
void KMeans::cluster(vector<Feature>& features, int k, int maxIterNum)
{
	clusters.clear();
	if (k <= 0 || k > 5) 
		k = 5;
	
	srand((unsigned)time(NULL));
	int num = features.size();
	for (int i = 0; i < k; ++i) {
		Point *pt = (Point *)(features[rand() % num].getGeom());
		clusters.push_back(Cluster(*pt));
	}

	// Task 14.1
	// Write your code here
}

void KMeans::draw()
{
	int r[5] = { 27, 217, 117, 231, 102 };
	int g[5] = { 158, 95, 112, 41, 166 };
	int b[5] = { 119, 2, 179, 138, 30 };

	for (size_t i = 0; i < clusters.size(); ++i) {
		glColor3d(r[i] / 255.0, g[i] / 255.0, b[i] / 255.0);
		clusters[i].draw();
	}
}

void KMeans::print()
{
	for (size_t i = 0; i < clusters.size(); ++i) {
		cout << "Cluster " << i << ":" << endl;
		clusters[i].print();
		cout << endl;
	}
}