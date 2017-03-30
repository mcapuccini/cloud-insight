/*
 * ToleranceProvider.h
 *
 *  Created on: Feb 17, 2014
 *      Author: jan
 */

#ifndef TOLERANCEPROVIDER_H_
#define TOLERANCEPROVIDER_H_

#include <boost/shared_ptr.hpp>
namespace INSIGHTv3 {

class InsightToleranceProvider {
public:
	InsightToleranceProvider(int s) {
	}
	;
	virtual ~InsightToleranceProvider() {
	}
	;

	virtual int getCurrentSimulationNbr() = 0;
	virtual double getCurrentTolerance() = 0;
	virtual void next() = 0;
};

typedef boost::shared_ptr<InsightToleranceProvider> ToleranceProviderPtr;
} /* namespace INSIGHTv3 */

#endif /* TOLERANCEPROVIDER_H_ */
