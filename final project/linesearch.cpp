#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <string>

#include "linesearch.h"

using namespace std;

vector<Point> mikami_tabuchi_line_search(const std::vector<std::vector<int>>& grid, const Point& start, const Point& target) {
    // Initialize the source and target points
    Point S = start;
    Point T = target;
    
    // Initialize the line segments from source and target
    std::vector<Line> source_lines = generate_initial_lines(S, grid);
    std::vector<Line> target_lines = generate_initial_lines(T, grid);
    
    // Set up a data structure to store base points and intersections
    std::vector<Point> base_points = {S, T};
    std::vector<Point> intersections;

    // Perform the line-search algorithm
    while (true) {
        std::vector<Point> new_base_points;
        
        // Extend all lines from current base points
        for (const Point& point : base_points) {
            std::vector<Line> lines = generate_initial_lines(point, grid);
            for (Line& line : lines) {
                line.extend(grid);
                if (std::any_of(target_lines.begin(), target_lines.end(), [&line](const Line& target_line) {
                    return line.intersects(target_line);
                })) {
                    Point intersection = line.get_intersection_point(target_lines[0]); // Assuming there is only one target line for simplicity
                    intersections.push_back(intersection);
                    std::vector<Point> path = trace_to_point(intersection, S, grid);
                    std::vector<Point> path_to_T = trace_to_point(intersection, T, grid);
                    path.insert(path.end(), path_to_T.begin(), path_to_T.end());
                    return path;  // Path found
                }
            }
            std::vector<Point> line_intersections = get_line_intersections(lines);
            new_base_points.insert(new_base_points.end(), line_intersections.begin(), line_intersections.end());
        }
        
        // If no new base points, no path exists
        if (new_base_points.empty()) {
            return {};
        }
        
        base_points = new_base_points;
    }
}

vector<Line> generate_initial_lines(const Point& point, const std::vector<std::vector<int>>& grid) {
    std::vector<Line> lines;
    lines.emplace_back(point, "horizontal");
    lines.emplace_back(point, "vertical");
    return lines;
}


vector<Point> get_line_intersections(const std::vector<Line>& lines) {
    std::vector<Point> intersections;
    for (size_t i = 0; i < lines.size(); ++i) {
        for (size_t j = i + 1; j < lines.size(); ++j) {
            if (lines[i].intersects(lines[j])) {
                intersections.push_back(lines[i].get_intersection_point(lines[j]));
            }
        }
    }
    return intersections;
}


vector<Point> trace_to_point(const Point& intersection, const Point& point, const std::vector<std::vector<int>>& grid) {
    std::vector<Point> path;
    Point current = intersection;
    while (!(current == point)) {
        path.push_back(current);
        if (current.x != point.x) {
            current.x = (current.x > point.x) ? current.x - 1 : current.x + 1;
        } else {
            current.y = (current.y > point.y) ? current.y - 1 : current.y + 1;
        }
    }
    path.push_back(point);
    return path;
}

// int main() {
//     // Example usage (replace this part with your file reading logic)
//     std::vector<std::vector<int>> grid = {
//         {0, 0, 0, 1, 0},
//         {0, 1, 0, 1, 0},
//         {0, 1, 0, 0, 0},
//         {0, 0, 0, 1, 0},
//         {0, 0, 0, 1, 0}
//     };
//     Point start(0, 0);
//     Point target(4, 4);

//     std::vector<Point> path = mikami_tabuchi_line_search(grid, start, target);
//     if (path.empty()) {
//         std::cout << "No path found." << std::endl;
//     } else {
//         std::cout << "Path found: ";
//         for (const Point& p : path) {
//             std::cout << "(" << p.x << ", " << p.y << ") ";
//         }
//         std::cout << std::endl;
//     }
//     return 0;
// }
