#include <iostream>
#include <vector>
#include <fstream>
#include "convexHull.h"


int main(int argv, char* argc[]) {

    std::ofstream file("output.dot", std::ios::trunc); //clear output.dot
    convexHull hullObj; // Create an object of the convexHull class
    std::vector<Point> data = hullObj.createConvexHull("points.txt"); //creates a DOT file and stores the CH in data
    std::vector<Point> data2 = hullObj.createConvexHull("points2.txt");
}
