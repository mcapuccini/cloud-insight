/*
 * KolmogorovComputer.h
 *
 *  Created on: Jan 28, 2014
 *      Author: jan
 */

#ifndef KOLMOGOROVCOMPUTER_H_
#define KOLMOGOROVCOMPUTER_H_

#include <stddef.h>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <MatrixTypes.h>

namespace INSIGHTv3 {

class KolmogorovComputer {
public:
	KolmogorovComputer(const std::vector<EiVector>& original_data, double beta,
			double tolerance);
	KolmogorovComputer(double kappa_tolerance,
			const std::vector<EiVector>& original_data, double beta,
			double tolerance);
	KolmogorovComputer(const std::vector<EiVector>& original_data, double beta,
			double tolerance, double kappa);
	virtual ~KolmogorovComputer();

	double distTwoSample(const EiVector& first_sample,
			const size_t column_of_second_sample);

	static double getKappaForMApprox(int M, double beta);
	static double getKappaForM(int M, double beta, double tolerance);

	static double getThresholdForSApprox(int s, double beta, int M);
	static double getThresholdForSApprox(int s, double beta, double kappa);
	static double getThresholdForS(int s, double beta, double kappa,
			double tolerance);

	static int getSForThresholdApprox(double epsilon, double beta, int M);
	static int getSForThresholdApprox(double threshold, double beta,
			double kappa);
	static int getSForThreshold(double threshold, double beta, double kappa,
			double tolerance);

	double getThresholdForSApprox(int s);
	double getThresholdForS(int s);

	int getSForThresholdApprox(double threshold);
	int getSForThreshold(double threshold);

	static double kolmogorovCdfInverse(int s, double q, double tolerance);
	double kolmogorovCdfInverse(int s);

private:
	std::vector<EiVector> _sorted_Data;
	double _tolerance;
	double _kappa;
	double _beta;

	void _init(const std::vector<EiVector>& original_data);
	size_t _findSmallestPredecesorIndex(const double value,
			EiVectorRef& vector);
	size_t _findLargestIndex(const double value, EiVectorRef& vector);
};

typedef boost::shared_ptr<KolmogorovComputer> KolmogorovComputerPtr;

#ifdef __cplusplus
extern "C" {
#endif
void mMultiply(double *A, double *B, double *C, int m);
void mPower(double *A, int eA, double *V, int *eV, int m, int n);
double kolmogorov_cdf_marsaglia(int n, double d);
#ifdef __cplusplus
}
#endif

class KolmogorovCdfRoots {
public:
	KolmogorovCdfRoots(int s, double q) :
			_s(s), _q(q) {
	}
	;
	virtual ~KolmogorovCdfRoots() {
	}
	;

	double operator()(double x) {
		return kolmogorov_cdf_marsaglia(_s, x) - _q;
	}
private:
	int _s;
	double _q;
};

class KolmogorovTol {
public:
	KolmogorovTol(double tolerance) :
			_tolerance(tolerance) {

	}
	;
	virtual ~KolmogorovTol() {

	}
	;

	double operator()(double x_lo, double x_hi) {
		return x_hi - x_lo <= _tolerance;
	}
private:
	double _tolerance;
};

} /* namespace INSIGHTv3 */

#endif /* KOLMOGOROVCOMPUTER_H_ */
