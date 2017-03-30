/*
 * INSIGHTv3.h
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 */

#ifndef INSIGHTV3_H_
#define INSIGHTV3_H_

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/poisson_distribution.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/smart_ptr/shared_ptr.hpp>

#include "MatrixTypes.h"

namespace INSIGHTv3 {

typedef boost::mt19937 RandomNumberGenerator;
typedef boost::shared_ptr<boost::mt19937> RngPtr;
typedef boost::uniform_int<> UniformIntMapping;
typedef boost::poisson_distribution<> PoissonMapping;
typedef boost::normal_distribution<> NormalMapping;

typedef boost::uniform_01<boost::mt19937> DistUniform;
typedef boost::variate_generator<boost::mt19937&, boost::uniform_int<> > DistUniformInt;
typedef boost::variate_generator<boost::mt19937&, boost::poisson_distribution<> > DistPoisson;
typedef boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > DistNormal;

} /* namespace INSIGHTv3 */

#endif /* INSIGHTV3_H_ */
