#include "include/cpp_argv.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>

#define DEFAULT_NAME "pearson"

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
  const size_t n = data_set.n;
  const double *x = data_set.x;
  const double *y = data_set.y;

  // Step 1: Calculate means
  struct MeanReducer {
    const double *x;
    const double *y;
    double sum_x = 0.0;
    double sum_y = 0.0;

    MeanReducer(const double *x, const double *y) : x(x), y(y) {}

    MeanReducer(const MeanReducer &other, tbb::split)
        : x(other.x), y(other.y), sum_x(0.0), sum_y(0.0) {}

    void operator()(const tbb::blocked_range<size_t> &range) {
      for (size_t i = range.begin(); i < range.end(); ++i) {
        sum_x += x[i];
        sum_y += y[i];
      }
    }

    void join(const MeanReducer &other) {
      sum_x += other.sum_x;
      sum_y += other.sum_y;
    }
  };

  MeanReducer mean_reducer(x, y);
  tbb::parallel_reduce(tbb::blocked_range<size_t>(0, n), mean_reducer);

  const double moy_x = mean_reducer.sum_x / n;
  const double moy_y = mean_reducer.sum_y / n;

  // Step 2: Calculate Pearson components
  struct PearsonReducer {
    const double *x;
    const double *y;
    double moy_x;
    double moy_y;
    double sum_xy = 0.0;
    double sum_xx = 0.0;
    double sum_yy = 0.0;

    PearsonReducer(const double *x, const double *y, double moy_x, double moy_y)
        : x(x), y(y), moy_x(moy_x), moy_y(moy_y) {}

    PearsonReducer(const PearsonReducer &other, tbb::split)
        : x(other.x), y(other.y), moy_x(other.moy_x), moy_y(other.moy_y),
          sum_xy(0.0), sum_xx(0.0), sum_yy(0.0) {}

    void operator()(const tbb::blocked_range<size_t> &range) {
      for (size_t i = range.begin(); i < range.end(); ++i) {
        const double dx = x[i] - moy_x;
        const double dy = y[i] - moy_y;
        sum_xy += dx * dy;
        sum_xx += dx * dx;
        sum_yy += dy * dy;
      }
    }

    void join(const PearsonReducer &other) {
      sum_xy += other.sum_xy;
      sum_xx += other.sum_xx;
      sum_yy += other.sum_yy;
    }
  };

  PearsonReducer pearson_reducer(x, y, moy_x, moy_y);
  tbb::parallel_reduce(tbb::blocked_range<size_t>(0, n), pearson_reducer);

  Correlation res;
  res.a = pearson_reducer.sum_xy / pearson_reducer.sum_xx;
  res.b = moy_y - res.a * moy_x;
  res.r = pearson_reducer.sum_xy /
          std::sqrt(pearson_reducer.sum_xx * pearson_reducer.sum_yy);

  return res;
}
