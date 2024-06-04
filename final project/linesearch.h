#ifndef _LINESEARCH_H_
#define _LINESEARCH_H_

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

struct Point_2 {
    int x, y;
    Point_2(int x, int y) : x(x), y(y) {}     // for default initialization
    Point_2() : x(0), y(0) {} // 默認構造函數

    bool operator==(const Point_2& other) const {
        return x == other.x && y == other.y;
    }
};

namespace std {
    template <>
    struct hash<Point_2> {
        std::size_t operator()(const Point_2& p) const {
            return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
        }
    };
}

struct Line {
    Point_2 start, end;
    string orientation;
    Line(Point_2 start, string orientation) : start(start), end(start), orientation(orientation) {} // init assume line length=0 
    
    // Extends the line until it hits an obstacle or the grid boundary
    // grid [y][x]
    vector<Point_2> extend(const vector<vector<int>>& grid) {
        vector<Point_2> extended_points;
        int grid_width = grid[0].size();
        int grid_height = grid.size();

        if (orientation == "horizontal") {
            while (end.x + 1 < grid_width && grid[end.y][end.x + 1] != 1) {
                end.x++;
                extended_points.push_back(end);
            }
            while (start.x - 1 >= 0 && grid[start.y][start.x - 1] != 1) {
                start.x--;
                extended_points.push_back(start);
            }
        } 
        else if (orientation == "vertical") {
            while (end.y + 1 < grid_height && grid[end.y + 1][end.x] != 1) {
                end.y++;
                extended_points.push_back(end);
            }
            while (start.y - 1 >= 0 && grid[start.y - 1][start.x] != 1) {
                start.y--;
                extended_points.push_back(start);
            }
        }
        return extended_points;
    }

    // Checks if two lines intersect
    // if condition may have error, because line have two direction!
    bool intersects(const Line& other) const {
        if (orientation == "horizontal" && other.orientation == "vertical") {
            return start.y >= other.start.y && end.y <= other.end.y && other.start.x <= end.x && other.start.x >= start.x;
        } 
        else if (orientation == "vertical" && other.orientation == "horizontal") {
            return start.x >= other.start.x && end.x <= other.end.x && other.start.y <= end.y && other.start.y >= start.y;
        }
        return false;
    }

    // Returns the escape point of two lines if they intersect.
    Point_2 get_intersection_point(const Line& other) const {
        if (intersects(other)) {
            if (orientation == "horizontal") {
                return Point_2(other.start.x, start.y);
            } 
            else if(orientation == "vertical"){
                return Point_2(start.x, other.start.y);
            }
        }
        return Point_2(-1, -1);  // Invalid intersection !! alert!
    }
};

// Function prototypes
vector<Line> generate_initial_lines(const Point_2& point, const vector<vector<int>>& grid);
vector<Point_2> get_line_intersections(const vector<Line>& lines);
// vector<Point_2> trace_to_point(const Point_2& intersection, const Point_2& point, const vector<vector<int>>& grid);
vector<Point_2> mikami_tabuchi_line_search(const vector<vector<int>>& grid, const Point_2& start, const Point_2& target);

#endif