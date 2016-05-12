#ifndef UTIL_H_
#define UTIL_H_

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

namespace tcp_util {

// Returns True if the first number comes after the second one in 
// 32-bit sequence number space with wraparound.
//
// Sequence numbers are unsigned and can wrap around, in which case
// the highest bit would be flipped. Since sequence numbers are 32-bits
// we assume wraparound if the difference between two sequence numbers
// is larger than 2^31
bool After(uint32_t first, uint32_t second);

bool Before(uint32_t first, uint32_t second);

bool Between(uint32_t middle, uint32_t first, uint32_t second);

// Checks if the first range is included in the second range
bool RangeIncluded(uint32_t first_start, uint32_t first_end,
        uint32_t second_start, uint32_t second_end);

// Checks if the two ranges overlap (at least one common value)
bool Overlaps(uint32_t left_a, uint32_t right_a,
        uint32_t left_b, uint32_t right_b);

}  // namespace tcp_util

namespace stats_util {

// Parameters for a linear fit with a constant term of the form:
// y = c_0 + c_1 * x
typedef struct {
    double c_0, c_1;  // function parameters
    double cov_00, cov_01, cov_11;  // covariance results
    double sum_sq;  // sum-squared error of the fit

    void clear() {
        c_0 = c_1 = cov_00 = cov_01 = cov_11 = sum_sq = 0;
    }
} LinearFitParameters;

// Returns the x-th percentile in a list of values
template<typename Number>
Number Percentile(const std::vector<Number> values, const uint8_t percentile,
        const bool input_is_sorted);

// Returns the x-th percentile in a list of values
template<typename Number>
inline Number Percentile(const std::vector<Number> values,
        const uint8_t percentile) {
    return Percentile(values, percentile, false);    
}

// Returns the median in a sorted list of values
template<typename Number>
inline Number Median(const std::vector<Number> values) {
    return Percentile(values, 50);
}

// Returns the median in a list of values
template<typename Number>
inline Number Median(const std::vector<Number> values, const bool input_is_sorted) {
    return Percentile(values, 50, input_is_sorted);
}

// Returns the mean (rounded to the Number type if necessary) of a list of values
template<typename Number>
inline Number Mean(const std::vector<Number> values);

// Return Pearson correlation coefficient for two vectors (assuming that
// both have the same size)
double PearsonCorrelation(const std::vector<double> x,
        const std::vector<double> y);

// Uses regression to compute the best linear fit with a constant term for the
// given set of samples
LinearFitParameters LinearFit(const std::vector<double> x,
        const std::vector<double> y);

// Computes the estimated function value for a given x using the given
// linear fit as function
double LinearFitValueForX(const LinearFitParameters& fit, double x);

// Generates a 2D histogram and then returns a new set of samples
// generated by taking center values of each bin with at least value 1.
// The interval specifies the width/height of each bin
std::vector<std::pair<double, double>> PopulatedHistogramBins(
        std::vector<std::pair<double, double>> samples,
        size_t x_interval, size_t y_interval);

}  // namespace stats_util

namespace vector_util {

// Separates a vector of pair values into two vectors storing the first
// and second values respectively
template<typename Number>
void SplitPairs(const std::vector<std::pair<Number, Number>> pairs,
        std::vector<Number>* firsts,
        std::vector<Number>* seconds);

}  // namespace vector_util


// Non-specialized template functions need to be visible at this time so we have
// to include them here immediately
#include "util.tcc"

#endif  /* UTIL_H_ */
