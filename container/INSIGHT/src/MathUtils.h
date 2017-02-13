/*
 * MathUtils.h
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 */

#ifndef MATHUTILS_H_
#define MATHUTILS_H_

#include <stddef.h>
#include <vector>

#include "INSIGHTv3.h"

namespace INSIGHTv3 {

class MathUtils {
public:
	MathUtils();
	virtual ~MathUtils();

	static size_t uniformSampleNormalizedBinarySearch(
			const std::vector<double>& weight_sum, double random_uniform_number_01);

private:
	static size_t binarySearch(const std::vector<double>& vector, double entry,
			size_t start, size_t end);
};

} /* namespace INSIGHTv3 */

#endif /* MATHUTILS_H_ */
