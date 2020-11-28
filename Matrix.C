/*
 * Matrix.C
 *
 *  Created on: 21.11.2020
 *      Author: Sniper
 */

#include <stdlib.h>
#include <math.h>
#include "Matrix.h"
#include "MathFunctions.h"

double *create_identity()
	{
	static double result[16];

	result[0 + 0 * 4] = 1.0;
	result[1 + 0 * 4] = 0.0;
	result[2 + 0 * 4] = 0.0;
	result[3 + 0 * 4] = 0.0;

	result[0 + 1 * 4] = 0.0;
	result[1 + 1 * 4] = 1.0;
	result[2 + 1 * 4] = 0.0;
	result[3 + 1 * 4] = 0.0;

	result[0 + 2 * 4] = 0.0;
	result[1 + 2 * 4] = 0.0;
	result[2 + 2 * 4] = 1.0;
	result[3 + 2 * 4] = 0.0;

	result[0 + 3 * 4] = 0.0;
	result[1 + 3 * 4] = 0.0;
	result[2 + 3 * 4] = 0.0;
	result[3 + 3 * 4] = 1.0;

	return result;
	}

double *create_matrix(double m00, double m01, double m02, double m03,
	                  double m10, double m11, double m12, double m13,
					  double m20, double m21, double m22, double m23,
					  double m30, double m31, double m32, double m33)
	{
	static double result[16];

	result[0 + 0 * 4] = m00;
	result[1 + 0 * 4] = m01;
	result[2 + 0 * 4] = m02;
	result[3 + 0 * 4] = m03;

	result[0 + 1 * 4] = m10;
	result[1 + 1 * 4] = m11;
	result[2 + 1 * 4] = m12;
	result[3 + 1 * 4] = m13;

	result[0 + 2 * 4] = m20;
	result[1 + 2 * 4] = m21;
	result[2 + 2 * 4] = m22;
	result[3 + 2 * 4] = m23;

	result[0 + 3 * 4] = m30;
	result[1 + 3 * 4] = m31;
	result[2 + 3 * 4] = m32;
	result[3 + 3 * 4] = m33;

	return result;
	}

double *create_translation(double x, double y, double z)
	{
	double *result = create_identity();
	result[3] = result[0] * x + result[1] * y + result[2] * z + result[3];
	return result;
	}

double *create_scale(double x, double y, double z)
	{
	double *result = create_identity();
	result[0 + 0 * 4] = x;
	result[1 + 1 * 4] = y;
	result[2 + 2 * 4] = z;
	return result;
	}

double *create_rotation(double radians, double x, double y, double z)
	{
	double c = cos(radians);
	double s = sin(radians);
	double oneOverCos = 1.0 - c;

	double *result = create_identity();
	result[0 + 0 * 4] = x * oneOverCos + c;
	result[1 + 0 * 4] = y * x * oneOverCos + z * s;
	result[2 + 0 * 4] = x * z * oneOverCos - y * s;

	result[0 + 1 * 4] = x * y * oneOverCos - z * s;
	result[1 + 1 * 4] = y * oneOverCos + c;
	result[2 + 1 * 4] = y * z * oneOverCos + x * s;

	result[0 + 2 * 4] = x * z * oneOverCos + y * s;
	result[1 + 2 * 4] = y * z * oneOverCos - x * s;
	result[2 + 2 * 4] = z * oneOverCos + c;
	return result;
	}

void add_matrices(double *result, double *mat1, double *mat2)
	{
	int x, y;
	double temp[16];

	for (y = 0; y < 4; ++y)
		{
		for (x = 0; x < 4; ++x)
			{
			temp[x + y * 4] = mat1[x + y * 4] + mat2[x + y * 4];
			}
		}

	memcpy(result, temp, 16 * sizeof(double));
	}

void subtract_matrices(double *result, double *mat1, double *mat2)
	{
	int x, y;
	double temp[16];

	for (y = 0; y < 4; ++y)
		{
		for (x = 0; x < 4; ++x)
			{
			temp[x + y * 4] = mat1[x + y * 4] - mat2[x + y * 4];
			}
		}

	memcpy(result, temp, 16 * sizeof(double));
	}

void multiply_matrices(double *result, double *mat1, double *mat2)
	{
	int x, y, i;
	double sum;
	double temp[16];

	for (y = 0; y < 4; ++y)
		{
		for (x = 0; x < 4; ++x)
			{
			sum = 0.0;

			for (i = 0; i < 4; ++i)
				{
				sum += mat1[x + i * 4] * mat2[i + y * 4];
				}

			temp[x + y * 4] = sum;
			}
		}

	memcpy(result, temp, 16 * sizeof(double));
	}

double *create_perspective(double fov, double aspectRatio, double zNear, double zFar)
	{
	double q = 1 / tan(to_radians(0.5 * (double)fov));
	double a = q / aspectRatio;
	double b = (zNear + zFar) / (zNear - zFar);
	double c = (2 * zNear * zFar) / (zNear - zFar);

	double *result = create_identity();
	result[0 + 0 * 4] = a;
	result[1 + 1 * 4] = q;
	result[2 + 2 * 4] = b;
	result[3 + 2 * 4] = -1;
	result[2 + 3 * 4] = c;
	result[3 + 3 * 4] = 0;
	return result;
	}

double *create_orthographic(double left, double right, double bottom, double top, double zNear, double zFar)
	{
	double *result = create_identity();

	

	return result;
	}

void cross(double *result, double vector[], double other[])
	{
	result[0] = vector[1] * other[2] - vector[2] * other[1];
	result[1] = vector[2] * other[0] - vector[0] * other[2];
	result[2] = vector[0] * other[1] - vector[1] * other[0];
	}


