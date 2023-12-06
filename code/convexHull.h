
#ifndef PROJECT_CONVEXHULL_H
#define PROJECT_CONVEXHULL_H
#include <vector>


struct Point {
    int x;
    int y;
    double angle;
};


class convexHull {
    void appendToFile(const std::string& filename, const std::string& content);

    void removeLastLine(const std::string& filename);

    int findDistance(Point p1, Point p2);

    std::vector<Point> initialize(std::string file);

    int findLowestY(std::vector<Point> dataSet);

    void swap(Point &p1, Point &p2);

    void selection_sort(std::vector<Point> &dataSet);

    double findAngle(Point anchor, Point p2);

    int orientation(Point p1, Point p2, Point p3);

    void print(std::vector<Point> convexHull);

    void saveToDotFile(const std::vector<Point>& points, const std::string& filename);

public:
    std::vector<Point> createConvexHull(std::string filename);

    void detectShape(std::vector<Point> convexHull);
};


#endif //PROJECT_CONVEXHULL_H