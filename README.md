# Performing Geometric Operations
This program performs operations on a collection of polygons. It supports the following operations:
1. **Union**: Combine multiple polygons into a single polygon.
2. **Intersection**: Find the common area among multiple polygons.
3. **Difference**: Subtract one set of polygons from another.

## Solution
To achieve this task, C++ library Computational Geometry Algorithms Library (CGAL) has been used. The library provides robust and exact geometric computations where precision matters, especially involving complex polygon operations. It supports different polygon types, simple polygons, polygons with holes and more compelx polygon structures. It uses advanced algorithms and data structures to achieve good time and space complexity. A brief explanation of included header files and type definition from the code:

**#include <CGAL/Polygon_2.h>**: Polygon_2 is a class template which represents a simple polygon in the plane. It is a part of 2D polygon package in CGAL which provides algorithms and data structures to work with planar polygons.

**auto typedef Polygon = CGAL::Polygon_2<Kernel>**: The ‘Polygon_2’ class template is parameterized by the kernel type (here ‘Kernel’) . ‘Kernel’ is an instance of the ‘Exact_predicates_exact_constructions_kernel’ which is used as a kernel for the ‘Polyon_2’ class. This means that the Polygon will be represented by precise arithmetic operations (from ‘Kernel’) using both predicates (e.g., orientation test showing if a point is located at right or left of a 2D line) and constructions (e.g., intersection point of two lines or  calculating common area among multiple polygons)

Using the **Exact_predicates_exact_constructions_kernel** ensures that geometric operations, such as, union or intersection are performed with high precision.

## How to run
To compile and run the code using CGAL, make sure that CGAL is installed:
```sh
sudo apt-get update
sudo apt-get install libcgal-dev
```
For visualizations, matplotlib is required:
```sh
sudo apt-get install python-matplotlib python-numpy python2.7-dev
```
Then run:
```sh
g++ -std=c++11 -o polygon_operations polygon.cpp -lCGAL -lgmp -lmpfr -I/usr/include/python2.7 –lpython2.7
```

# Methodology

### Step 1: 

The input is provided from a text file. Each line in a text file represents vertices (x,y) of a polygon. Each vertex’s x and y coordinates are separated by a space. For example:

1.0 1.0 2.0 1.0 2.0 2.0 1.0 2.0
3.0 3.0 4.0 3.0 4.0 4.0 3.0 4.0

In this example, two polygons are represented. The first polygon has vertices (1.0, 1.0), (2.0, 1.0), (2.0, 2.0), and (1.0, 2.0). The second polygon has vertices (3.0, 3.0), (4.0, 3.0), (4.0, 4.0), and (3.0, 4.0).

The program then prompts the user to choose an operation (Union, Intersection, or Difference) by entering a corresponding number (1, 2, or 3). Each operation is summarized in the next steps.

### Step 2:

**Union Operation**: The union operation combines multiple polygons into a single polygon set. In CGAL, this can be achieved using the join method of Polygon_set_2. For each polygon in the input set, the join operation adds it to the result set. The final result is a set of polygons representing the union of all input polygon.

1. The `polygonUnion` function takes a vector of `Polygon_2` objects as input
   and returns a `Polygon_with_holes_2` object as the result of the union operation.
2. The function first checks if the input vector is empty. If it is, an 
   empty `Polygon_with_holes_2` object is returned.
3. The code creates a new `Polygon_with_holes_2` object called `result` 
   and initializes it with the vertices of the first polygon in the input vector.
4. It then iterates over the remaining polygons in the input vector, converts 
   each polygon to a `Polygon_with_holes_2` object called `current_polygon`, 
   and performs the union operation using the `CGAL::join` function.
5. The result of the union operation is updated in the `result` object.
6. Finally, the `result` object is returned as the output of the `polygonUnion` function.

**Intersection and Difference Operations**:
The intersection operation finds the common area shared by multiple polygons. In CGAL, this can be achieved using the intersection method of Polygon_set_2. 

The difference operation finds the difference between two polygons which can be computed using the difference method of Polygon_set_2.

1. The `performOperation` function takes a vector of `Polygon_2` objects and a string `operationName` specifying which operation (intersection or difference) to be performed as an input. It returns a vector of `Polygon_with_holes_2` object with intersection operation result at each instance.
2. The function uses nested loops to iterate over all pairs of input polygons (excluding self-intersections).
3. For each pair, it calculates the result using either the `CGAL::intersection` or `CGAL::difference` method based on the specified operation.
4. The result is stored in a `std::list<Polygon_with_holes_2>` called `result`.
5. The results from each pair are inserted into the final results vector `results`.
6. The function returns the vector of `Polygon_with_holes_2` `results`.

### Step 3:
**Visualization**: To provide information about the results obtained from the intersection or difference operation, `processResults` function is used.
1. The function takes a vector of `Polygon_with_holes_2` object and a string specifying the type of operation to be performed.
2. The function generates a filepath to save the resulting image for each polygon pair.
3. The function also prints the resulting polygon points.
4. If no error, it calls a separate function `printPolygon` to output the resulting polygon and saves an image representation in a local folder. 
5. **Note**: In case of union operation, `printPolygon` is called directly because union operation outputs a single image showing union of all input polygons.
# Error Handling
1. At least 2 polygons are required to perform union, intersection, or difference operation.
2. Check if each polygon must have at least three vertices.
3. Check if the three vertices of each polygon are not collinear. For example, given the polygon points   ((0,0), (2,2), (0,0)), the program performs error handling for such situations.
4. Check if each x,y point of a polygon is valid.
5. Check if polygon is not self-intersecting.