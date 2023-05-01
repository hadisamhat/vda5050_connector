#include <functional>
#include <iostream>
#include <vector>

template <typename Point>
class PointProcessor {
 public:
  void set_function(const std::function<void(const Point&)>& func) { function_ = func; }

  void process_points(const std::vector<Point>& points) { apply_function(points, function_); }

 private:
  std::function<void(const Point&)> function_;

  void apply_function(
      const std::vector<Point>& vec, const std::function<void(const Point&)>& func) {
    if (!func) return;
    for (const auto& val : vec) {
      func(val);
    }
  }
};

struct Point2D {
  double x;
  double y;
};

void print_point(const Point2D& p) { std::cout << "(" << p.x << ", " << p.y << ")" << std::endl; }

int main() {
  PointProcessor<Point2D> processor;

  std::vector<Point2D> vec{{0.0, 0.0}, {1.0, 1.0}, {2.0, 2.0}};
  processor.set_function(
      [](const Point2D p) { std::cout << "(" << p.x << ", " << p.y << ")" << std::endl; });

  processor.process_points(vec);
  // Output: (0, 0)
  //         (1, 1)
  //         (2, 2)

  return 0;
}