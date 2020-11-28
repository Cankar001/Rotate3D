/*
 * Matrix.h
 *
 *  Created on: 21.11.2020
 *      Author: Sniper
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <GHSi.h>

double *create_identity();
double *create_matrix(double m00, double m01, double m02, double m03,
	                   double m10, double m11, double m12, double m13,
							 double m20, double m21, double m22, double m23,
							 double m30, double m31, double m32, double m33);

double *create_translation(double x, double y, double z);
double *create_scale(double x, double y, double z);
double *create_rotation(double radians, double x, double y, double z);

void add_matrices(double *mat1, double *mat2);
void subtract_matrices(double *mat1, double *mat2);
void multiply_matrices(double *result, double *mat1, double *mat2);

double *create_perspective(double fov, double aspectRatio, double zNear, double zFar);
double *create_orthographic(double left, double right, double bottom, double top, double zNear, double zFar);

void cross(double *result, double vector[], double other[]);

#endif /* MATRIX_H_ */
