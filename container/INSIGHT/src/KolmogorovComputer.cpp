/*
 * KolmogorovComputer.cpp
 *
 *  Created on: Jan 28, 2014
 *      Author: jan
 *
 *      This class handles everything that deals with the Kolmogorov distance or Kolmogorov function. In particular this class provides the number of needed
 *      simulations S for each provided tolerance epsilon, and the other way around. It also computes the distance between a computed trajectory and the original data.
 *      The parameters beta  comes from: d(Y_S, X_M) < epsilon ==> P(d(Y, X) > epsilon) <= beta for further details see Lillacci & Khammash 2013
 *      The parameter kappa is the computed tolerance corresponding to the number of samples in the original dataset
 */

#include "KolmogorovComputer.h"

#include <cmath>
#include <iostream>
#include <cstdlib>

#include <Eigen/Dense>
#include <boost/math/tools/roots.hpp>

#include "IllegalArgumentException.h"

namespace INSIGHTv3 {

// Constructor for approximated kappa
KolmogorovComputer::KolmogorovComputer(
		const std::vector<EiVector>& original_data, double beta,
		double tolerance) {

	_kappa = getKappaForMApprox(original_data[0].size(), beta);
	_beta = beta;
	_tolerance = tolerance;
	_init(original_data);
}

// Constructor for exact kappa
KolmogorovComputer::KolmogorovComputer(double kappa_tolerance,
		const std::vector<EiVector>& original_data, double beta,
		double tolerance) {

	std::cout
			<< "Exact kappa for the KolmovorovComputer is being computed. M is "
			<< original_data[0].size()
			<< ". The tolerance for the kolmogorov function root solver is "
			<< kappa_tolerance
			<< ". This may take a long time. Consider using one of the other constructors that use a precomputed kappa, or compute kappa only approximately"
			<< std::endl;
	_kappa = getKappaForM(original_data[0].size(), beta, kappa_tolerance);
	_beta = beta;
	_tolerance = tolerance;
	_init(original_data);
}

// Constructor for precomputed kappa
KolmogorovComputer::KolmogorovComputer(
		const std::vector<EiVector>& original_data, double beta,
		double tolerance, double kappa) {

	_kappa = kappa;
	_beta = beta;
	_tolerance = tolerance;
	_init(original_data);
}

KolmogorovComputer::~KolmogorovComputer() {
}

void KolmogorovComputer::_init(const std::vector<EiVector>& original_data) {
	for (size_t column = 0; column < original_data.size(); column++) {
		EiVector single_column = original_data[column];
		std::sort(single_column.data(),
				single_column.data() + single_column.size());
		_sorted_Data.push_back(single_column);
	}

}

double KolmogorovComputer::distTwoSample(const EiVector& first_sample,
		const size_t column_of_second_sample) {

	size_t j1 = 0, j2 = 0;
	double distance = 0.0, d1, d2, dt, fn1 = 0.0, n1 =
			(double) first_sample.size(), fn2 = 0.0, n2 =
			(_sorted_Data[column_of_second_sample]).size();

	EiVector first_sample_sorted = EiVector(first_sample);
	std::sort(first_sample_sorted.data(),
			first_sample_sorted.data() + first_sample_sorted.size());

	EiVector* second_sample_sorted = (&_sorted_Data[column_of_second_sample]);

	while (j1 < n1 && j2 < n2) {
		if ((d1 = first_sample_sorted(j1))
				<= (d2 = (*second_sample_sorted)(j2))) {

			EiVectorRef first_sample_tail = first_sample_sorted.tail(
					first_sample_sorted.size() - j1);

			if (d1 == d2) {
				j1 = j1 + _findLargestIndex(d2, first_sample_tail) + 1;
			} else {
				j1 = j1 + _findSmallestPredecesorIndex(d2, first_sample_tail)
						+ 1;
			}
		}
		if (d2 <= d1) {
			EiVectorRef second_sample_tail = second_sample_sorted->tail(
					second_sample_sorted->size() - j2);
			if (d1 == d2) {
				j2 = j2 + _findLargestIndex(d1, second_sample_tail) + 1;
			} else {
				j2 = j2 + _findSmallestPredecesorIndex(d1, second_sample_tail)
						+ 1;
			}
		}
		fn2 = (j2) / n2;
		fn1 = (j1) / n1;
		if ((dt = fabs((double) fn2 - fn1)) > distance)
			distance = dt;
	}

	return distance;
}

size_t KolmogorovComputer::_findSmallestPredecesorIndex(const double value,
		EiVectorRef& _vector) {
	if (_vector.size() == 1) {
		return 0;
	} else {
		size_t median_index = floor(_vector.size() / 2.0);
		double median = _vector(median_index);
		if (value > median) {

			EiVectorRef upper_sub_vector = _vector.tail(
					_vector.size() - median_index);

			return median_index
					+ _findSmallestPredecesorIndex(value, upper_sub_vector);
		} else {
			EiVectorRef lower_sub_vector = _vector.head(median_index);
			return _findSmallestPredecesorIndex(value, lower_sub_vector);
		}
	}

}

size_t KolmogorovComputer::_findLargestIndex(const double value,
		EiVectorRef& _vector) {
	if (_vector.size() == 1) {
		return 0;
	} else {
		size_t median_index = floor(_vector.size() / 2.0);
		double median = _vector(median_index);
		if (value >= median) {
			EiVectorRef upper_sub_vector = _vector.tail(
					_vector.size() - median_index);

			return median_index + _findLargestIndex(value, upper_sub_vector);
		} else {
			EiVectorRef lower_sub_vector = _vector.head(median_index);
			return _findLargestIndex(value, lower_sub_vector);
		}
	}
}

double KolmogorovComputer::getKappaForMApprox(int M, double beta) {
	double alpha = 1 - sqrt(1.0 - beta);
	return sqrt(-(1 / (2.0 * M)) * log(alpha / 2.0));
}

double KolmogorovComputer::getKappaForM(int M, double beta, double tolerance) {
	return kolmogorovCdfInverse(M, 1 - (1 - sqrt(1 - beta)), tolerance);
}

double KolmogorovComputer::getThresholdForSApprox(int s, double beta, int M) {
	double kappa = getKappaForMApprox(M, beta);
	return getThresholdForSApprox(s, beta, kappa);
}

double KolmogorovComputer::getThresholdForSApprox(int s, double beta,
		double kappa) {
	double alpha = 1 - sqrt(1.0 - beta);
	return sqrt(-(1 / (2.0 * s)) * log(alpha / 2.0)) + kappa;
}

double KolmogorovComputer::getThresholdForS(int s, double beta, double kappa,
		double tolerance) {
	return kolmogorovCdfInverse(s, 1 - (1 - sqrt(beta)), tolerance) + kappa;
}

int KolmogorovComputer::getSForThresholdApprox(double threshold, double beta,
		int M) {
	double kappa = getKappaForMApprox(M, beta);
	return getSForThresholdApprox(threshold, beta, kappa);
}

int KolmogorovComputer::getSForThresholdApprox(double threshold, double beta,
		double kappa) {
	double alpha = 1 - sqrt(1.0 - beta);
	int s = ceil(-log(alpha / 2.0) / (2.0 * pow(threshold - kappa, 2.0)));
	return s;
}

int KolmogorovComputer::getSForThreshold(double threshold, double beta,
		double kappa, double tolerance) {
	int s = 1;
	double tol = kolmogorovCdfInverse(s, 1 - (1 - sqrt(beta)), tolerance);
	while (tol < threshold - kappa) {
		s++;
		tol = kolmogorovCdfInverse(s, 1 - (1 - sqrt(beta)), tolerance);
	}
	return s;
}

double KolmogorovComputer::getThresholdForSApprox(int s) {
	double alpha = 1 - sqrt(1.0 - _beta);
	return sqrt(-(1 / (2.0 * s)) * log(alpha / 2.0)) + _kappa;
}

double KolmogorovComputer::getThresholdForS(int s) {
	return kolmogorovCdfInverse(s) + _kappa;
}

int KolmogorovComputer::getSForThresholdApprox(double threshold) {

	double alpha = 1 - sqrt(1.0 - _beta);
	int s = ceil(-log(alpha / 2.0) / (2.0 * pow(threshold - _kappa, 2.0)));
	return s;
}

int KolmogorovComputer::getSForThreshold(double threshold) {
	int s = 1;
	double tol = kolmogorovCdfInverse(s);
	while (tol < threshold - _kappa) {
		s++;
		tol = kolmogorovCdfInverse(s);
	}
	return s;
}

#ifdef __cplusplus
extern "C" {
#endif
/**
 Supporting function for kolmogorov_cdf_marsaglia, not to be used directly.
 */
void mMultiply(double *A, double *B, double *C, int m) {
	int i, j, k;
	double s;
	for (i = 0; i < m; i++)
		for (j = 0; j < m; j++) {
			s = 0.;
			for (k = 0; k < m; k++)
				s += A[i * m + k] * B[k * m + j];
			C[i * m + j] = s;
		}
}

/**
 Supporting function for kolmogorov_cdf_marsaglia, not to be used directly.
 */
void mPower(double *A, int eA, double *V, int *eV, int m, int n) {
	double *B;
	int eB, i;
	if (n == 1) {
		for (i = 0; i < m * m; i++)
			V[i] = A[i];
		*eV = eA;
		return;
	}
	mPower(A, eA, V, eV, m, n / 2);
	B = (double*) malloc((m * m) * sizeof(double));
	mMultiply(V, V, B, m);
	eB = 2 * (*eV);
	if (n % 2 == 0) {
		for (i = 0; i < m * m; i++)
			V[i] = B[i];
		*eV = eB;
	} else {
		mMultiply(A, B, V, m);
		*eV = eA + eB;
	}
	if (V[(m / 2) * m + (m / 2)] > 1e140) {
		for (i = 0; i < m * m; i++)
			V[i] = V[i] * 1e-140;
		*eV += 140;
	}
	free(B);
}

double kolmogorov_cdf_marsaglia(int n, double d) {
	int k, m, i, j, g, eH, eQ;
	double h, s, *H, *Q;

	//OMIT NEXT LINE IF YOU REQUIRE >7 DIGIT ACCURACY IN THE RIGHT TAIL
	s = d * d * n;
	if (s > 7.24 || (s > 3.76 && n > 99))
		return 1 - 2 * exp(-(2.000071 + .331 / sqrt(n) + 1.409 / n) * s);

	k = (int) (n * d) + 1;
	m = 2 * k - 1;
	h = k - n * d;
	H = (double*) malloc((m * m) * sizeof(double));
	Q = (double*) malloc((m * m) * sizeof(double));
	for (i = 0; i < m; i++)
		for (j = 0; j < m; j++)
			if (i - j + 1 < 0)
				H[i * m + j] = 0;
			else
				H[i * m + j] = 1;
	for (i = 0; i < m; i++) {
		H[i * m] -= pow(h, i + 1);
		H[(m - 1) * m + i] -= pow(h, (m - i));
	}
	H[(m - 1) * m] += (2 * h - 1 > 0 ? pow(2 * h - 1, m) : 0);
	for (i = 0; i < m; i++)
		for (j = 0; j < m; j++)
			if (i - j + 1 > 0)
				for (g = 1; g <= i - j + 1; g++)
					H[i * m + j] /= g;
	eH = 0;
	mPower(H, eH, Q, &eQ, m, n);
	s = Q[(k - 1) * m + k - 1];
	for (i = 1; i <= n; i++) {
		s = s * i / n;
		if (s < 1e-140) {
			s *= 1e140;
			eQ -= 140;
		}
	}
	s *= pow(10., eQ);
	free(H);
	free(Q);
	return s;
}

#ifdef __cplusplus
}
#endif

// q = 1- alpha or, in terms of beta: q = 1 - (1 - sqrt(beta))
double KolmogorovComputer::kolmogorovCdfInverse(int S, double q,
		double tolerance) {
	double x_lo = 0.0;

	// Set initial guesses using the DKW bounds
	double x_hi = sqrt(-1 / (2 * (double) S) * log((1 - q) / 2));

	KolmogorovCdfRoots kolmogorov_roots(S, q);
	KolmogorovTol kolmogorov_tol(tolerance);

	std::pair<double, double> result = boost::math::tools::bisect<
			KolmogorovCdfRoots, double, KolmogorovTol>(kolmogorov_roots, x_lo,
			x_hi, kolmogorov_tol);

	return (result.first + result.second) / 2.0;
}

double KolmogorovComputer::kolmogorovCdfInverse(int S) {
	double q = 1 - (1 - sqrt(1 - _beta));

	return kolmogorovCdfInverse(S, q, _tolerance);
}

} /* namespace INSIGHTv3 */
