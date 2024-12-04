#include "include/cpp_argv.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <tbb/task_scheduler_init.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include <functional>
#include <numeric>

#define DEFAULT_NAME "pearson"

typedef tbb::blocked_range<size_t> Range;

class Body {
  public:
    Body(const Data_Set* const data_set) : data_set_(data_set), tot_x_(0.0), tot_y_(0.0) {}
    Body(const Body& rhs, tbb::split) : data_set_(rhs.data_set_), tot_x_(0.0), tot_y_(0.0) {}
  public:
    const double& getTotX() const { return tot_x_; }
    const double& getTotY() const { return tot_y_; }
  public:
    void operator()(const Range& range) {
      for (size_t i = range.begin(); i != range.end(); i ++) {
        tot_x_ += data_set_.x[i];
        tot_y_ += data_set_.y[i];
      }
    }
  public:
    void join(const Body& rhs) {
      tot_x_ += rhs.tot_x_;
      tot_y_ += rhs.tot_y_;
    }
  private:
    const Data_Set* const data_set_;
    double tot_x_;
    double tot_y_;
};

/**
 * @brief Data measurement set.
 *
 */
struct Data_Set {
  size_t n;  /** Number of measurements.  */
  double *x; /** Variable X measurements. */
  double *y; /** Variable Y measurements. */
};

/**
 * @brief Pearson correlation.
 *
 */
struct Correlation {
  double a; /** Right slope.         */
  double b; /** Y-axis shift.        */
  double r; /** Pearson coefficient. */
};

/**
 * @brief Loads a data set from a input stream then returns it.
 *
 * @param stream The input stream.
 * @return Data_Set The data set.
 */
Data_Set load_file(std::istream &stream) noexcept;

/**
 * @brief Calculates then returns the Pearson correlation of a data set.
 *
 * @param data_set The data set.
 * @return Correlation The corresponding Pearson correlation.
 */
Correlation calculate(const Data_Set &data_set) noexcept;

/**
 * @brief Main program.
 *
 * @param argc number of arguments in the command line.
 * @param argv arguments of the command line.
 * @return @c EXIT_SUCCESS if command succeeds else @c EXIT_FAILURE.
 */
int main(int argc, char *argv[]) {

  // User expects help.
  CPP_ARGV_TEST_HELP_REQUEST(argc, argv[0], DEFAULT_NAME, "filename")

  // Bad argument number.
  CPP_ARGV_TEST_ARG_NUM(argc, 2)

  // Retrieves the data filename.
  const char *const filename = argv[1];

  // Tries to open it.
  std::ifstream stream(filename);
  if (not stream) {
    std::cerr << std::strerror(errno) << std::endl;
    return EXIT_FAILURE;
  }

  // Loads the data set.
  const Data_Set data_set = load_file(stream);

  // 1 thread par coeur
  tbb::task_scheduler_init init;

  // Calculates the corresponding Pearson correlation.
  const Correlation result = calculate(data_set);

  // Prints results onto the standard output.
  std::cout << "a: " << result.a << "\tb: " << result.b << "\tr: " << result.r
            << std::endl;

  // It's over.
  return EXIT_SUCCESS;
}

/* -------------------------------------------------------------------------- */
/*                                  load_file                                 */
/* -------------------------------------------------------------------------- */

Data_Set load_file(std::istream &stream) noexcept {
  Data_Set res;

  stream >> res.n;
  res.x = new double[res.n];
  res.y = new double[res.n];

  for (size_t i = 0; i < res.n; i++) {
    stream >> res.x[i] >> res.y[i];
  }

  return res;
}

/* -------------------------------------------------------------------------- */
/*                                  calculate                                 */
/* -------------------------------------------------------------------------- */

Correlation calculate(const Data_Set &data_set) noexcept {

  double tot_x = 0.0, tot_y = 0.0;
  /*
  for (size_t i = 0; i != data_set.n; i ++) {
    tot_x += data_set.x[i];
    tot_y += data_set.y[i];
  }
  */

  //par classe
  {
    Body body(&data_set);
    const Range range(0, data_set.n);

    tbb::parallel_reduce(range, body);
    tot_x = body.getTotX();
    tot_y = body.getTotY();
  }

  const double moy_x = tot_x / data_set.n, moy_y = tot_y / data_set.n;
  
  double tot_xx = 0.0, tot_xy = 0.0, tot_yy = 0.0;
  for (size_t i = 0; i != data_set.n; i ++) {
    tot_xy += (data_set.x[i] - moy_x) * (data_set.y[i] - moy_y);
    tot_xx += (data_set.x[i] - moy_x) * (data_set.x[i] - moy_x);
    tot_yy += (data_set.y[i] - moy_y) * (data_set.y[i] - moy_x);
  }

  Correlation res;
  res.a = tot_xy / tot_xx;
  res.b = moy_y - res.a * moy_x;
  res.r = tot_xy / std::sqrt(tot_xx * tot_yy);

  return res;
}
