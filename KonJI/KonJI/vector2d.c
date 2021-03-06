#pragma once

#include "vector2d.h"

struct Vector2d vector2dAdd(struct Vector2d vector1, struct Vector2d vector2) {
	struct Vector2d ret_vector = { vector1.x + vector2.x, vector1.y + vector2.y };
	return ret_vector;
}

struct Vector2d vector2dSubtract(struct Vector2d vector1, struct Vector2d vector2) {
	struct Vector2d ret_vector = { vector1.x - vector2.x, vector1.y - vector2.y };
	return ret_vector;
}

struct Vector2d vector2dScalarMult(double scalar, struct Vector2d vector) {
	struct Vector2d ret_vector = { scalar * vector.x, scalar * vector.y };
	return ret_vector;
}

double vector2dDotProduct(struct Vector2d vector1, struct Vector2d vector2) {
	return (vector1.x * vector2.x) + (vector1.y * vector2.y);
}

double vector2dMagnitude(struct Vector2d vector) {
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

struct Vector2d vector2dNormalize(struct Vector2d vector) {
	double magnitude = vector2dMagnitude(vector);

	struct Vector2d ret_vector = { vector.x / magnitude, vector.y / magnitude };

	return ret_vector;
}

struct Vector2d vector2dNormal(struct Vector2d vector) {
	struct Vector2d ret_vector = { -vector.y, vector.x };

	return ret_vector;
}


struct Vector2d vector2dRotation(struct Vector2d v, double angle) {
	struct Vector2d ret_vector = {
		cos(angle) * v.x - sin(angle) * v.y,
		sin(angle) * v.x + cos(angle) * v.y
	};
	return ret_vector;
}

double vector2dPointPointDistance(struct Vector2d point1, struct Vector2d point2) {
	return vector2dMagnitude(vector2dSubtract(point2, point1));
}

double vector2dAngleLine(struct Vector2d v1, struct Vector2d v2) {
	return acos(vector2dDotProduct(v1, v2) / (vector2dMagnitude(v1) * vector2dMagnitude(v2)));
}

struct Vector2d vector2dTrendLine(struct Vector2d* points, unsigned int points_len) {
	double sum_x = 0;
	double sum_y = 0;
	double sum_xy = 0;
	double sum_xx = 0;

	for (unsigned int i = 0; i < points_len; i++) {
		sum_x += points[i].x;
		sum_y += points[i].y;
		sum_xy += points[i].x * points[i].y;
		sum_xx += points[i].x * points[i].x;
	}

	struct Vector2d result = { 0.0, 1.0 };
	double divisor = points_len * sum_xx - sum_x*sum_x;

	if (divisor == 0) {
		return result;
	}

	double slope = (points_len * sum_xy - sum_x * sum_y) / divisor;

	result.x = 1.0;
	result.y = slope * result.x;

	return result;
}

struct Vector2d vector2dSymmetry(struct Vector2d reflector, struct Vector2d reflected) {
	struct Vector2d n = vector2dNormalize(vector2dNormal(reflector));

	return vector2dSubtract(vector2dScalarMult(2 * vector2dDotProduct(n, reflected), n), reflected);
}

struct Vector2d vector2dReflection(struct Vector2d reflector, struct Vector2d reflected) {
	return vector2dScalarMult(-1.0, vector2dSymmetry(reflector, reflected));
}

struct Vector2d vector2dLineLineIntersection(struct Vector2d start1, struct Vector2d vect1, struct Vector2d start2, struct Vector2d vect2, bool* parallel)
{   
	struct Vector2d ret_vector = { 0.0, 0.0 };

	double x1 = start1.x;
	double y1 = start1.y;
	double x2 = start1.x + vect1.x;
	double y2 = start1.y + vect1.y;

	double x3 = start2.x;
	double y3 = start2.y;
	double x4 = start2.x + vect2.x;
	double y4 = start2.y + vect2.y;

	double denominator = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);

	if (denominator == 0.0) {
		*parallel = true;
		return start1; //return garbage
	}
	*parallel = false;

	ret_vector.x = ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4)) / denominator;
	ret_vector.y = ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4)) / denominator;

	return ret_vector;
}


struct Vector2d vector2dCenterOfMass(struct Vector2d* points, unsigned int points_len) {
	struct Vector2d ret_vector = { 0.0, 0.0 };

	for (unsigned int i = 0; i < points_len; i++) {
		ret_vector = vector2dAdd(ret_vector, points[i]);
	}

	return vector2dScalarMult(1.0 / points_len, ret_vector);
}