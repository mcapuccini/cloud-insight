/*
 * MathUtils.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 *
 *      This is a utility class with some basic math functions. So far only a binary search is implements
 */

#include "MathUtils.h"

namespace INSIGHTv3 {

MathUtils::MathUtils() {
	// TODO Auto-generated constructor stub

}

MathUtils::~MathUtils() {
	// TODO Auto-generated destructor stub
}

size_t MathUtils::uniformSampleNormalizedBinarySearch(
		const std::vector<double>& weight_sum,
		double random_uniform_number_01) {

	return binarySearch(weight_sum, random_uniform_number_01, 0,
			weight_sum.size() - 1);
}

size_t MathUtils::binarySearch(const std::vector<double>& vector, double entry,
		size_t start, size_t end) {
	if (start == end) {
		return start;
	}

	size_t middle = start + floor((end - start) / 2.0);

	if (entry > vector[middle]) {
		size_t newStart = middle + 1;
		size_t index = binarySearch(vector, entry, newStart, end);
		return index;
	} else {
		size_t newEnd = middle;
		size_t index = binarySearch(vector, entry, start, newEnd);
		return index;
	}

}

} /* namespace INSIGHTv3 */
