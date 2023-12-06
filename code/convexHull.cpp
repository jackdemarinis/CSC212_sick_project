#include "convexHull.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <iomanip>
#include <string>
#include <cmath>

void convexHull::detectShape(std::vector<Point> convexHull)
{
    if(convexHull.size() == 3)//could be triangle, equilaterian isosoles and scalene
    {
        int sameSides = 0;
        if(findDistance(convexHull[0],convexHull[1]) == findDistance(convexHull[0],convexHull[2]))
        {
            sameSides++;
        }
        if(findDistance(convexHull[0],convexHull[1]) == findDistance(convexHull[1],convexHull[2]))
        {
            sameSides++;
        }
        if(findDistance(convexHull[0],convexHull[2]) == findDistance(convexHull[1],convexHull[2]))
        {
            sameSides++;
        }

        if(sameSides == 3)
        {
            std::cout << "It is an equilateral Triangle" << std::endl;
        }
        else if(sameSides == 2)
        {
            std::cout << "It is an isosceles triangle" << std::endl;
        }
        else
        {
            std::cout << "It is a scalene triangle" << std::endl;
        }

    }
    else if(convexHull.size() == 4) //could be square / rectangle
    {
        if(findDistance(convexHull[0],convexHull[1]) == findDistance(convexHull[2],convexHull[3])) //if top equals bottom
        {
            if(findDistance(convexHull[0],convexHull[1]) == findDistance(convexHull[3],convexHull[0])) //bottom also equals side
            {
                std::cout << "It is a square" << std::endl;
            }
            else
            {
                std::cout << "It is a rectangle" << std::endl;
            }
        }
        else
        {
            std::cout << "It is a rectangle" << std::endl;
        }
    }
}

int convexHull::findDistance(Point p1, Point p2) //calcualte distance between 2 points (using a^2 + b ^2 = c^2)
{
    return sqrt(pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2));
}

std::vector<Point> convexHull::createConvexHull(std::string filename) //main function to create the CH
{
    std::vector<Point> dataSet;
    std::vector<Point> convexHull;
    Point anchor;
    dataSet = initialize(filename); //read in from file and store all data points

    if(dataSet.size() < 3) //convex hull cannot work with less than 3 points
    {
        std::cout <<"Convex Hull not possible";
    }

    int index = findLowestY(dataSet);
    dataSet[index].angle = 0;
    anchor = dataSet[index]; //save the point with the lowest y as the anchor point
    swap(dataSet[index],dataSet[0]); //put the anchor point at front

    for(int i = 1; i < dataSet.size(); i++) //give each point an angle based off the anchor point
    {
        dataSet[i].angle = findAngle(anchor, dataSet[i]);
    }

    //using selection sort, sort the array smallest angle to largest
    selection_sort(dataSet);
    //start the hull by adding the anchor point and the first element
    convexHull.push_back(dataSet[0]);
    convexHull.push_back(dataSet[1]);

    for(int i = 2; i < dataSet.size(); i++) {
        while(convexHull.size() > 1 && orientation(convexHull[convexHull.size()-2], convexHull[convexHull.size()-1], dataSet[i]) != 1) {
            convexHull.pop_back(); // Remove points creating a clockwise or collinear turn
        }
        convexHull.push_back(dataSet[i]); // Add the point creating a counterclockwise turn
    }
    print(convexHull);
    std::cout<<std::endl;
    saveToDotFile(convexHull, "output.dot");
    return convexHull;
}

void convexHull::appendToFile(const std::string& filename, const std::string& content) {
    std::ofstream outFile(filename, std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Unable to open file for appending.\n";
        return;
    }
    outFile << content;
    outFile.close();
}

void convexHull::saveToDotFile(const std::vector<Point>& points, const std::string& filename) {
    std::ifstream fileCheck(filename);
    bool fileIsEmpty = fileCheck.peek() == std::ifstream::traits_type::eof();
    fileCheck.close();

    if (fileIsEmpty) {
        std::ofstream dotFile(filename); // Open file normally for writing if it's empty

        if (!dotFile.is_open()) {
            std::cerr << "Unable to open DOT file!\n";
            return;
        }

        dotFile << "graph convexHull {\n";
        dotFile << "  layout=\"neato\";\n\n";

        // Write each point as a node in the DOT file with coordinates as labels
        for (size_t i = 0; i < points.size(); ++i) {
            dotFile << "  " << "P" << i << " [pos=\"" << points[i].x << "," << points[i].y << "!\""
                    << " label=\"" << std::fixed << std::setprecision(2) << "(" << points[i].x << ", " << points[i].y
                    << ")\"";
            dotFile << (i < points.size() - 1 ? "];\n" : "];\n\n");
        }

        // Add edges between points
        for (size_t i = 0; i < points.size(); ++i) {
            dotFile << "  P" << i << " -- "
                    << "P" << ((i + 1) % points.size()) << (i < points.size() - 1 ? ";\n" : ";\n");
        }

        dotFile << "}\n";

        dotFile.close();
        std::cout << "DOT file '" << filename << "' has been updated.\n";
    } else {
        removeLastLine(filename); // Remove the last line '}' to prepare for appending

        std::stringstream content;

        for (size_t i = 0; i < points.size(); ++i) {
            content << "  " << "P" << i + 50 << " [pos=\"" << points[i].x << "," << points[i].y << "!\""
                    << " label=\"" << std::fixed << std::setprecision(2) << "(" << points[i].x << ", " << points[i].y
                    << ")\"";
            content << (i < points.size() - 1 ? "];\n" : "];\n\n");
        }

        for (size_t i = 0; i < points.size(); ++i) {
            content << "  P" << i + 50 << " -- "
                    << "P" << ((i + 1) % points.size()) + 50 << (i < points.size() - 1 ? ";\n" : ";\n");
        }

        content << "}\n";

        appendToFile(filename, content.str());
        std::cout << "DOT file '" << filename << "' has been updated.\n";
    }
}

void convexHull::removeLastLine(const std::string& filename) {
    std::ifstream inFile(filename);
    std::vector<std::string> lines;
    std::string line;

    // Read lines from the file and store in a vector
    while (std::getline(inFile, line)) {
        lines.push_back(line);
    }

    inFile.close();

    if (!lines.empty()) {
        // Remove the last line from the vector
        lines.pop_back();

        // Rewrite the modified content back to the file
        std::ofstream outFile(filename);
        for (const std::string& updatedLine : lines) {
            outFile << updatedLine << "\n";
        }
        outFile.close();
        std::cout << "Last line removed from the file '" << filename << "'.\n";
    } else {
        std::cout << "File is empty. Nothing to remove.\n";
    }
}


void convexHull::print(std::vector<Point> convexHull) //print out a vector of points
{
    for(int i = 0; i < convexHull.size();i++)
    {
        std::cout << "(" << convexHull[i].x << "," << convexHull[i].y << ") ";
    }
    std::cout << "(" << convexHull[0].x << "," << convexHull[0].y << ")";
}

int convexHull::orientation(Point p1, Point p2, Point p3) //takes 3 points to determine their orientation to one another
{

    int val = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
    if (val == 0)
        return 0; // collinear

    return (val > 0) ? 1 : 2; //returns one if a counterclockwise turn is made, 2 if clockwise
}


int convexHull::findLowestY(std::vector<Point> dataSet) //find the lowest Y value in a vector of Points
{
    int index = 0;
    for(int i = 1; i < dataSet.size(); i++)
    {
        if(dataSet[i].y < dataSet[index].y) //this will find the index of the lowest y;
        {
            index = i;
        }
        if(dataSet[i].y == dataSet[index].y) //if there are 2 lowest y, take lowest x between them
        {
            if(dataSet[i].x < dataSet[index].x)
            {
                index = i;
            }
        }
    }
    return index;
}

double convexHull::findAngle(Point anchor, Point p2) //finds the angle between 2 points
{
    return atan2(p2.y - anchor.y, p2.x - anchor.x);
}

void convexHull::selection_sort(std::vector<Point> &dataSet) //sorts a vector of points smallest to largest angle
{
    int min_indx;
    for(int i = 1; i < dataSet.size(); i++)
    {
        min_indx = i;
        for(int j = i+1; j < dataSet.size(); j++)
        {
            if(dataSet[j].angle < dataSet[min_indx].angle)
            {
                min_indx = j;
            }
            if(dataSet[j].angle == dataSet[min_indx].angle) //if there are 2 of the same angles, put the one closest first
            {
                int distance1 = findDistance(dataSet[0], dataSet[j]);
                int distance2 = findDistance(dataSet[0], dataSet[min_indx]);
                if(distance1 < distance2)
                {
                    min_indx = j;
                }
            }
        }
        swap(dataSet[i], dataSet[min_indx]);
    }
}

void convexHull::swap(Point &p1, Point &p2) //swap two points
{
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}

std::vector<Point> convexHull::initialize(std::string file) //creates the initial vector that holds data points from a file
{
    //read in the file and store each point x and y coordinate into a pair
    //which is then stored into a vector together
    std::string line;
    std::ifstream table;

    table.open(file);

    std::vector<Point> points;

    while (std::getline(table, line))
    {
        std::pair<int,int> p;
        std::vector<int> temp;
        std::stringstream ss(line);
        int num;
        while(ss >> num)
        {
            temp.push_back(num);
        }
        Point myPoint;
        myPoint.x = temp[0];
        myPoint.y = temp[1];

        points.push_back(myPoint);
    }
    return points;
}