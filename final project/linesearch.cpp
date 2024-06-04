#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <unordered_map>
#include "linesearch.h"

using namespace std;

vector<Point_2> mikami_tabuchi_line_search(const vector<vector<int>>& grid, const Point_2& start, const Point_2& target) {
    // Initialize the source and target points
    Point_2 S = start;
    Point_2 T = target;
    
    // Initialize the line segments from source and target
    vector<Line> target_lines = generate_initial_lines(T, grid);
    
    // Set up a data structure to store base points and their parents
    queue<Point_2> base_points;   // Using a queue for BFS-like expansion
    unordered_map<Point_2, Point_2> parent_map;
    base_points.push(S);
    parent_map[S] = Point_2(-1, -1);  // Mark the start point with a dummy parent

    // Perform the line-search algorithm
    while (!base_points.empty()) {
        Point_2 point = base_points.front();
        base_points.pop();
        vector<Line> lines = generate_initial_lines(point, grid);

        for (Line& line : lines) {
            vector<Point_2> extended_points = line.extend(grid);

            for (const Point_2& ext_point : extended_points) {
                // Check if the extended point intersects any target line            
                if (any_of(target_lines.begin(), target_lines.end(), [&ext_point](const Line& target_line) {
                    return target_line.start == ext_point || target_line.end == ext_point;
                })) {
                    // Found a path to the target
                    vector<Point_2> path;
                    Point_2 current = ext_point;

                    // Backtrack to find the full path
                    while (current.x != -1 && current.y != -1) {
                        path.push_back(current);
                        current = parent_map[current];
                    }

                    reverse(path.begin(), path.end());
                    return path;  // Path found
                }

                if (parent_map.find(ext_point) == parent_map.end()) {  // If not visited
                    base_points.push(ext_point);
                    parent_map[ext_point] = point;  // Record the parent
                }
            }
        }
    }
    
    // If no path is found, return an empty vector
    return {};
}



vector<Line> generate_initial_lines(const Point_2& point, const vector<vector<int>>& grid) {
    vector<Line> lines;
    lines.emplace_back(point, "horizontal");
    lines.emplace_back(point, "vertical");


    // print Lines
    // for (const Line& line : lines) {
    //     cout << "Line(start=(" << line.start.x << ", " << line.start.y << "), end=(" 
    //         << line.end.x << ", " << line.end.y << "), orientation=" << line.orientation << ")";
    // }
    // cout << endl;
    return lines;
}


vector<Point_2> get_line_intersections(const vector<Line>& lines) {
    vector<Point_2> intersections;
    for (size_t i = 0; i < lines.size(); i++) {
        for (size_t j = i + 1; j < lines.size(); j++) {
            if (lines[i].intersects(lines[j])) {
                intersections.push_back(lines[i].get_intersection_point(lines[j]));
            }
        }
    }
    return intersections;
}


/*
// intersection is start intersect point 
// point is the target point
// this is function may have some logic error, need to be check!
vector<Point_2> trace_to_point(const Point_2& intersection, const Point_2& point, const vector<vector<int>>& grid) {
    vector<Point_2> path;
    Point_2 current = intersection;
    cout << "Tracing path from intersection (" << intersection.x << ", " << intersection.y << ") to point (" << point.x << ", " << point.y << ")" << endl;

    while (!(current == point)) {
        path.push_back(current);
        if (current.x != point.x) {
            current.x = (current.x > point.x) ? current.x - 1 : current.x + 1;
        } 
        else if (current.y != point.y)  {
            current.y = (current.y > point.y) ? current.y - 1 : current.y + 1;
        }
        // // check the corordinate is valid or not
        // if (current.x < 0 || current.x >= int(grid[0].size()) || current.y < 0 || current.y >= int(grid.size())) {
        //     cout << "Error: Coordinate out of bounds: (" << current.x << ", " << current.y << ")" << endl;
        //     return vector<Point_2>(); // return error if no route
        // }
    }
    path.push_back(point);

    cout << "Traced path: ";
    for (const Point_2& p : path) {
        cout << "(" << p.x << ", " << p.y << ") ";
    }
    cout << endl;
    
    return path;
}
*/
