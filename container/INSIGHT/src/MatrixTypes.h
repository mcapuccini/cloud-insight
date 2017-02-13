/*
 * MatrixTypes.h
 *
 *  Created on: Jan 24, 2014
 *      Author: jan
 */

#ifndef MATRIXTYPES_H_
#define MATRIXTYPES_H_

#include <Eigen/Dense>


namespace INSIGHTv3 {

typedef Eigen::VectorXd EiVector;
typedef Eigen::MatrixXd EiMatrix;
typedef Eigen::Block<Eigen::MatrixXd, 1, Eigen::Dynamic> EiVectorRow;
typedef Eigen::Block<Eigen::MatrixXd, Eigen::Dynamic, 1> EiVectorCol;
typedef Eigen::Block<const Eigen::MatrixXd, 1, Eigen::Dynamic> EiConstSubVectorRow;
typedef Eigen::Block<const Eigen::MatrixXd, Eigen::Dynamic, 1> EiConstSubVectorCol;

typedef Eigen::Block<Eigen::MatrixXd> EiSubMatrix;
typedef Eigen::Ref<Eigen::MatrixXd> EiMatrixRef;
typedef Eigen::Ref<Eigen::VectorXd, 0, Eigen::InnerStride<> > EiVectorRef;
typedef Eigen::Ref<const Eigen::VectorXd, 0, Eigen::InnerStride<> > EiConstVectorRef;

} /* namespace INSIGHTv3 */

#endif /* MATRIXTYPES_H_ */
