#ifndef _LINESEARCH_H_
#define _LINESEARCH_H_

#include <iostream>
#include <vector>

using namespace std;

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct Line {
    Point start, end;
    string orientation;
    Line(Point start, string orientation) : start(start), end(start), orientation(orientation) {}
    
    void extend(const vector<vector<int>>& grid) {
        if (orientation == "horizontal") {
            while (end.x + 1 < grid[0].size() && grid[end.y][end.x + 1] != 1) {
                end.x++;
            }
        } else if (orientation == "vertical") {
            while (end.y + 1 < grid.size() && grid[end.y + 1][end.x] != 1) {
                end.y++;
            }
        }
    }

    bool intersects(const Line& other) const {
        if (orientation == "horizontal" && other.orientation == "vertical") {
            return start.y == other.start.y && other.start.x <= end.x && other.start.x >= start.x;
        } else if (orientation == "vertical" && other.orientation == "horizontal") {
            return start.x == other.start.x && other.start.y <= end.y && other.start.y >= start.y;
        }
        return false;
    }

    Point get_intersection_point(const Line& other) const {
        if (intersects(other)) {
            if (orientation == "horizontal") {
                return Point(other.start.x, start.y);
            } else {
                return Point(start.x, other.start.y);
            }
        }
        return Point(-1, -1);  // Invalid intersection
    }
};

// Function prototypes
vector<Line> generate_initial_lines(const Point& point, const std::vector<std::vector<int>>& grid);
vector<Point> get_line_intersections(const std::vector<Line>& lines);
vector<Point> trace_to_point(const Point& intersection, const Point& point, const std::vector<std::vector<int>>& grid);


#endif