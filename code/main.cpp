#include <iostream>
#include <vector>
#include "convexHull.h"


int main(int argv, char* argc[]) {

    std::string file = argc[1];
    convexHull hullObj; // Create an object of the convexHull class
    std::vector<Point> data = hullObj.createConvexHull(file);
}
