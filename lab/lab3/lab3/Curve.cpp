#include <cmath>
#include <algorithm>
#include "Curve.h"
#include <gl/freeglut.h> 
#include<iostream>  //my add
using namespace std;

namespace lab3 {

	/*
	 * zorder - Calculate the index number (Z-value) of the point (represented by coordinate) in the Z-curve with given order
	 */
	void zorder(int order, int& value, int coor[2])
	{
		// Calculate the z-order by bit shuffling
		value = 0;
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < order; ++j) {
				// Task 1.1 zorder，修改以下代码                      ##done
				int mask = 1 << j;
				// Check whether the value in the position is 1
				if (coor[i] & mask)
					// Do bit shuffling
					value |= 1 << (2 * j + 1 - i);
			}
		}
	}

	/*
	 * izorder - Calculate the coordinate of the point with given Z-value and order
	 */
	void izorder(int order, int value, int coor[2])
	{
		// Initialize the coordinate to zeros
		for (int i = 0; i < 2; ++i)
			coor[i] = 0;

		// Task 1.2 izoder                                        ##done
		// Write your code here
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < order; ++j) {
				int mask = 1 << (2 * j + 1 - i);
				if (value&mask)
					coor[i] |= 1 << j;
			}
		}
	}

	/*
	 * zdist - Calculate the pair-distance-sum-2 of Z-Curve in terms of the Manhattan distance
	 */
	int zdist(int order)
	{
		int dist = 0;
		int num = int(pow(2, order));
		int pointA[2], pointB[2], pointC[2];

		for (int value = 0; value < num * num - 2; ++value) {
			izorder(order, value, pointA);
			izorder(order, value + 1, pointB);
			izorder(order, value + 2, pointC);
			dist += abs(pointB[0] - pointA[0]);
			dist += abs(pointB[1] - pointA[1]);
			dist += abs(pointC[0] - pointA[0]);
			dist += abs(pointC[1] - pointA[1]);
		}

		return dist;
	}

	/*
	 * zrender - Render Z-Curve with the given order, width and height
	 */

	void zrender(int order, int width, int height)
	{
		int num = int(pow(2, order));
		double size = min(width, height) / (double)num;

		int pointA[2], pointB[2];
		izorder(order, 0, pointA);

		for (int value = 1; value < num * num; ++value) {
			izorder(order, value, pointB);

			glBegin(GL_LINES);
			glVertex2d((pointA[0] + 0.5) * size, (pointA[1] + 0.5) * size);
			glVertex2d((pointB[0] + 0.5) * size, (pointB[1] + 0.5) * size);
			glEnd();

			pointA[0] = pointB[0];
			pointA[1] = pointB[1];
		}
	}

	/*
	 * horder - Calculate the index number (H-value) of the point (represented by coordinate) in Hilbert curve with given order
	 */
	void horder(int order, int& value, int coor[2])
	{
		// order = 1
		int num = int(pow(2, 1));
		int * hcurve = new int[num * num];
		hcurve[0] = 1;
		hcurve[1] = 2;
		hcurve[2] = 0;
		hcurve[3] = 3;

		for (int i = 2; i <= order; ++i) {
			int add = (int)pow(2, 2 * i - 2);		// the number of values in order - 1
			int blockLen = (int)pow(2, i - 1);
			int * temp = hcurve;

			num = int(pow(2, i));
			hcurve = new int[num * num];

			for (int j = 0; j < blockLen; ++j) {
				for (int k = 0; k < blockLen; ++k) {
					// Task 2.1 horder，修改以下四行代码
					hcurve[k*num + j] = temp[k*blockLen + j] + add;
					hcurve[k*num + (j + blockLen)] = temp[k*blockLen + j] + 2 * add;
					hcurve[(k + blockLen)*num + j] = temp[(blockLen - 1 - j)*blockLen + (blockLen - 1 - k)];
					hcurve[(k + blockLen)*num + (j + blockLen)] = temp[j*blockLen + k] + 3 * add;
				}
			}
			
			delete temp;
		}
		// Task 2.1 horder，修改以下一行代码
		value = hcurve[(num - 1 - coor[1])*num + coor[0]];
		delete hcurve;
	}
	void ihorder(int order, int  value, int coor[])
	{
		// order = 1
		int num = int(pow(2, 1));
		int * hcurve = new int[num * num];
		hcurve[0] = 1;
		hcurve[1] = 2;
		hcurve[2] = 0;
		hcurve[3] = 3;

		// Task 2.2 ihorder
		// Write your code here
		for (int i = 2; i <= order; ++i) {
			int add = (int)pow(2, 2 * i - 2);	// the number of values in order - 1
			int blockLen = (int)pow(2, i - 1);
			int * temp = hcurve;

			num = int(pow(2, i));
			hcurve = new int[num * num];

			for (int j = 0; j < blockLen; ++j) {
				for (int k = 0; k < blockLen; ++k) {
					hcurve[k*num + j] = temp[k*blockLen + j] + add;
					if (i == order && hcurve[k*num + j] == value)
					{
						coor[0] = j;
						coor[1] = num - 1 - k;
						return;
					}
					hcurve[k*num + (j + blockLen)] = temp[k*blockLen + j] + 2 * add;
					if (i == order && hcurve[k*num + (j + blockLen)] == value)
					{
						coor[0] = j + blockLen;
						coor[1] = num - 1 - k;
						return;
					}
					hcurve[(k + blockLen)*num + j] = temp[(blockLen - 1 - j)*blockLen + (blockLen - 1 - k)];
					if (i == order && hcurve[(k + blockLen)*num + j] == value)
					{
						coor[0] = j;
						coor[1] = num - 1 - (k + blockLen);
						return;
					}
					hcurve[(k + blockLen)*num + (j + blockLen)] = temp[j*blockLen + k] + 3 * add;
					if (i == order && hcurve[(k + blockLen)*num + (j + blockLen)] == value)
					{
						coor[0] = j + blockLen;
						coor[1] = num - 1 - (k + blockLen);
						return;
					}
				}
			}
			delete temp;
		}
		delete hcurve;
	}

	/*
	 * hdist - Calculate the pair-distance-sum-2 of Hilbert Curve in terms of the Manhattan distance
	 */
	int hdist(int order)
	{
		int dist = 0;
		int num = (int)pow(2, order);
		int pointA[2], pointB[2], pointC[2];

		for (int value = 0; value < num * num - 2; value++) {
			ihorder(order, value, pointA);
			ihorder(order, value + 1, pointB);
			ihorder(order, value + 2, pointC);
			dist += abs(pointB[0] - pointA[0]);
			dist += abs(pointB[1] - pointA[1]);
			dist += abs(pointC[0] - pointA[0]);
			dist += abs(pointC[1] - pointA[1]);
		}

		return dist;
	}

	/*
	 * hrender - Render Hilbert Curve with the given order, width and height
	 */

	void hrender(int order, int width, int height)
	{
		int num = int(pow(2, order));
		double size = min(width, height) / (double)num;

		int pointA[2], pointB[2];
		ihorder(order, 0, pointA);

		for (int value = 1; value < num * num; ++value) {
			ihorder(order, value, pointB);

			glBegin(GL_LINES);
			glVertex2d((pointA[0] + 0.5) * size, (pointA[1] + 0.5) * size);
			glVertex2d((pointB[0] + 0.5) * size, (pointB[1] + 0.5) * size);
			glEnd();

			pointA[0] = pointB[0];
			pointA[1] = pointB[1];
		}
	}

}


