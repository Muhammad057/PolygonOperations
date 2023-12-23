#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Boolean_set_operations_2.h>

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2;

// Function to read polygons from a file
std::vector<Polygon_2> readPolygonsFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    std::vector<Polygon_2> polygons;
    
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    // Read each line as a polygon and add it to the vector
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        Polygon_2 polygon;
        Point_2 point;

        // Read vertices from the line
        while (iss >> point) {
            polygon.push_back(point);
        }
        polygons.push_back(polygon);
    }

    inputFile.close();
    return polygons;
}

// Function to create a directory
void createDirectory(const std::string& path) {
    if (mkdir(path.c_str(), 0777) == -1) {
        std::cerr << "Error: Unable to create directory " << path << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Function to check if a point has valid coordinates
bool isValidPoint(const Point_2& point) {
    return CGAL::to_double(point.x()) != CGAL::to_double(std::numeric_limits<double>::infinity()) &&
           CGAL::to_double(point.y()) != CGAL::to_double(std::numeric_limits<double>::infinity());
}

// Function to check if proper x, y points are input for a polygon
bool isValidPolygon(const Polygon_2& polygon) {
    // Check if the polygon has at least three vertices
    if (polygon.size() < 3) {
        std::cerr << "Error: Polygon must have at least three vertices. \n";
        return false;
    }

    for (auto vertexIt = polygon.vertices_begin(); vertexIt != polygon.vertices_end(); ++vertexIt) {
        if (!isValidPoint(*vertexIt)) {
            return false;
        }
    }

    // Additional check for collinearity of the first three vertices
    auto vertexIt = polygon.vertices_begin();
    auto vertexEnd = polygon.vertices_begin() + 3;
    if (CGAL::collinear(vertexIt[0], vertexIt[1], vertexIt[2])) {
        std::cerr << "Error: First three vertices of the polygon are collinear. \n";
        return false;
    }

    return true;
}

// Function to check if a polygon is simple (not self-intersecting)
bool isSimple(const Polygon_2& polygon) {
    return polygon.is_simple();
}

// Function to print the coordinates of a polygon with holes
void printPolygon(const Polygon_with_holes_2& polygon, const std::string& filePath) {
    plt::figure();
    std::vector<double> x, y;
    // Print the outer boundary vertices
    for (auto vertex = polygon.outer_boundary().vertices_begin();
         vertex != polygon.outer_boundary().vertices_end(); ++vertex) {
        std::cout << "(" << vertex->x() << ", " << vertex->y() << ") ";

        x.push_back(CGAL::to_double(vertex->x()));
        y.push_back(CGAL::to_double(vertex->y()));
    }
    // Close the loop by connecting the first and last vertices
    x.push_back(x.front());
    y.push_back(y.front());
    plt::plot(x, y);
    plt::save(filePath);
    plt::show();
    plt::clf();

    std::cout << "Figure saved to: " << filePath << std::endl;
}

// Function to process intersection results
void processResults(const std::vector<Polygon_with_holes_2>& results, const std::string& operationName) {
    for (std::size_t i = 0; i < results.size(); ++i) {
        const auto& result = results[i];

        if (operationName == "intersection") {
            std::string filePath = operationName + "Results" + "/intersection_" + std::to_string(i) + ".png";
            std::cout << "Processing Intersection Result " << i << ":\n";

            if (result.is_unbounded()) {
                std::cout << "Error: No intersection among polygons.\n";
            } else {
                std::cout << "Resulting polygon points after intersection operation are: \n";
                printPolygon(result, filePath);
                std::cout << "\n" << std::endl;
            }
        }
        else if (operationName == "difference") {
            std::string filePath = operationName + "Results" + "/difference_" + std::to_string(i) + ".png";
            std::cout << "Processing Difference Result " << i << ": \n";

            if (result.is_unbounded()) {
                std::cout << "Error: No difference among polygons. \n";
            } else {
                std::cout << "Resulting polygon points after difference operation are: \n";
                printPolygon(result, filePath);
                std::cout << "\n" << std::endl;
            }
        } else {
            std::cerr << "Invalid operation code. Exiting." << std::endl;
        }
    }
}

// Function to perform Intersection or Difference of polygons
std::vector<Polygon_with_holes_2> performOperation(const std::vector<Polygon_2>& inputPolygons, const std::string& operationName) {
    std::vector<Polygon_with_holes_2> results;
    for (std::size_t i = 0; i < inputPolygons.size(); ++i) {
        for (std::size_t j = i + 1; j < inputPolygons.size(); ++j) {
            std::list <Polygon_with_holes_2> result;
            if (operationName == "intersection") {
                std::cout << "Calculating intersection of polygon " << i + 1 << " and polygon " << j + 1 << ":" << std::endl;
                CGAL::intersection(inputPolygons[i], inputPolygons[j], std::back_inserter(result));
            }
            if (operationName == "difference") {
                std::cout << "Calculating difference of polygon " << i + 1 << " and polygon " << j + 1 << ":" << std::endl;
                CGAL::difference(inputPolygons[i], inputPolygons[j], std::back_inserter(result));
            }
            results.insert(results.end(), result.begin(), result.end());
            std::cout << std::endl;
        }
    }
    return results;
}

// Function to perform Union of polygons
Polygon_with_holes_2 polygonUnion(const std::vector<Polygon_2>& polygons) {
    if (polygons.empty()) {
        return Polygon_with_holes_2();
    }

    // Create a new Polygon_with_holes_2 and copy the vertices
    Polygon_with_holes_2 result;
    for (auto vertexIt = polygons[0].vertices_begin(); vertexIt != polygons[0].vertices_end(); ++vertexIt) {
        result.outer_boundary().push_back(*vertexIt);
    }

    for (std::size_t i = 1; i < polygons.size(); ++i) {
        // Convert each subsequent polygon to Polygon_with_holes_2
        Polygon_with_holes_2 current_polygon;
        for (auto vertexIt = polygons[i].vertices_begin(); vertexIt != polygons[i].vertices_end(); ++vertexIt) {
            current_polygon.outer_boundary().push_back(*vertexIt);
        }

        // Perform the union operation
        CGAL::join(result, current_polygon, result);
    }

    return result;
}

int main() {
    // Get input file name from user
    std::cout << "Enter the input file name: ";
    std::string filename;
    std::cin >> filename;

    // Read polygons from the file
    std::vector<Polygon_2> inputPolygons = readPolygonsFromFile(filename);

    // Get operation from user
    std::cout << "Choose operation (1 for Union, 2 for Intersection, 3 for Difference): ";
    int operation;
    std::cin >> operation;

    // Check if there are at least two non-empty polygons
    auto nonEmptyPolygon = [](const Polygon_2& polygon) { return !polygon.is_empty(); };
    if (std::count_if(inputPolygons.begin(), inputPolygons.end(), nonEmptyPolygon) < 2) {
        std::cerr << "Error: At least two non-empty polygons are required for performing operation." << std::endl;
        return 1; // Exit with an error code
    }

    // Check for valid points in each input polygon
    for (std::size_t i = 0; i < inputPolygons.size(); ++i) {
        if (!isValidPolygon(inputPolygons[i])) {
            std::cerr << "Error: Input polygon " << i << " has invalid points.\n";
            return 1; // Exit with an error code
        }
    }

    for (const auto& polygon : inputPolygons) {
        if (!isSimple(polygon)) {
            std::cerr << "Error: Input polygon is not simple (self-intersecting).\n";
            return 1; // Exit with an error code
        }
    }

    if (operation == 1) {
        // Union operation
        std::string folderName = "UnionResults";
        createDirectory(folderName);
        std::string filePath = folderName + "/union.png";

        Polygon_with_holes_2 resultUnion = polygonUnion(inputPolygons);

        // Output the union result
        if (resultUnion.is_unbounded()) {
            std::cout << "Error: Union result is an empty polygon.\n";
        } else {
            std::cout << "Resulting polygon points after union operation are: \n";
            printPolygon(resultUnion, filePath);
        }
    } else if (operation == 2) {
        // Intersection operation
        std::string folderName = "IntersectionResults";
        std::string operationName = "intersection";
        // Create a directory for the results
        createDirectory(folderName);

        std::vector<Polygon_with_holes_2> intersections = performOperation(inputPolygons, operationName);
        processResults(intersections, operationName);

    } else if (operation == 3) {
        // Difference operation
        std::string folderName = "DifferenceResults";
        std::string operationName = "difference";
        // Create a directory for the results
        createDirectory(folderName);
        std::vector<Polygon_with_holes_2> differences = performOperation(inputPolygons, operationName);
        processResults(differences, operationName);
    } else {
        std::cerr << "Invalid operation code. Exiting." << std::endl;
    }

}
