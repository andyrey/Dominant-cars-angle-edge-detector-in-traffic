#pragma once
#include "allincludes.h" 
using namespace std;
using namespace cv;
int whichLineDisturbedMost2(const int values[], const int size_of_values_array, float&
	ratio);
void drawValues(Mat &plot, const int values[], const int number, const int active_id);
int scanSpecialLine(const Mat& mat, const int m); 
void scanLine(const Mat& mat, vector<int> &vvals, int row, int leftcol, int rightcol, int
	thickness);
