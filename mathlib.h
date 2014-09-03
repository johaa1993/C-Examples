#include <string.h>
#include <math.h>

/*
@brief Multiplicate vectors
@param[in] n dimensions
@param[in] a vector
@param[in] b vector
@param[out] r vector
*/
void fv_mul(size_t n, float const * a, float const * b, float * r)
{
	for (int i = 0; i < n; i++)
	{
		r[i] = a[i] * b[i];
	}
}

/*
@brief Add vectors
@param[in] n dimensions
@param[in] a vector
@param[in] b vector
@param[out] r vector
*/
void fv_add(size_t n, float const * a, float const * b, float * r)
{
	for (int i = 0; i < n; i++)
	{
		r[i] = a[i] + b[i];
	}
}

/*
@brief Subtract vectors
@param[in] n dimensions
@param[in] a vector
@param[in] b vector
@param[out] r vector
*/
void fv_sub(size_t n, float const * a, float const * b, float * r)
{
	for (int i = 0; i < n; i++)
	{
		r[i] = a[i] - b[i];
	}
}

/*
@brief Dot product
@param[in] n dimensions
@param[in] a vector
@param[in] b vector
@return a dot b
*/
float fv_dot(size_t n, float const * a, float const * b)
{
	float sum = 0;
	for (int i = 0; i < n; i++)
	{
		sum += (a[i] * b[i]);
	}
	return sum;
}

/*
@brief Calculate magnitude squared
@param[in] n dimensions
@param[in] n vector
@return magnitude^2
*/
float fv_magnitude2(size_t n, float const * v)
{
	return fv_dot(n, v, v);
}


/*
@brief Calculate magnitude
@param[in] n dimensions
@param[in] n vector
@return magnitude
*/
float fv_magnitude(size_t n, float const * v)
{
	float mag = fv_dot(n, v, v);
	return sqrt(mag);
}


/*
@brief Normalize a vector
@param[out] r vector
*/
void fv_normalize(size_t n, float * v)
{
	float mag = fv_magnitude(n, v);
	for (int i = 0; i < n; i++)
	{
		v[i] /= mag;
	}
}

/*
@brief Multiplicate quaternions
@param[in] a quaternion
@param[in] b quaternion
@param[out] r quaternion
*/
void qf_mul(float const * a, float const * b, float * r)
{
	r[0] = (a[0] * b[0]) - (a[1] * b[1]) - (a[2] * b[2]) - (a[3] * b[3]);
	r[1] = (a[0] * b[1]) + (a[1] * b[0]) + (a[2] * b[3]) - (a[3] * b[2]);
	r[2] = (a[0] * b[2]) - (a[1] * b[3]) + (a[2] * b[0]) + (a[3] * b[1]);
	r[3] = (a[0] * b[3]) + (a[1] * b[2]) - (a[2] * b[1]) + (a[3] * b[0]);
}

/*
@brief Multiplicate quaternions and assign (r=rb)
@param[in] b quaternion
@param[out] r quaternion
*/
void qf_mulab(float const * b, float * r)
{
	float R[4];
	memcpy(R, r, 4);
	qf_mul(R, b, r);
}

/*
@brief Multiplicate quaternions and assign (r=br)
@param[in] b quaternion
@param[out] r quaternion
*/
void qf_mulba(float const * b, float * r)
{
	float R[4];
	memcpy(R, r, 4);
	qf_mul(b, R, r);
}

/*
@brief Convert angle axis to quaternion
@param[in] a angle
@param[in] v direction
@param[out] q quaternion
*/
void qf_angleaxis(float a, const float * v, float * q)
{
	a /= 2.0f;
	float s = sin(a);
	q[0] = cos(a);
	q[1] = v[1] * s;
	q[2] = v[2] * s;
	q[3] = v[3] * s;
}

void mf_mac(size_t nn, const float * a, const float * b, float * r)
{
	int i, j, k;
	for (i = 0; i < nn; i++)
	for (j = 0; j < nn; j++)
	for (k = 0; k < nn; k++)
		r[i*nn + j] += a[i*nn + k] * b[k*nn + j];
}


void m3f_quaternionc(const float * q, float * r)
{
	float ww = q[0] * q[0];
	float wx2 = q[0] * q[1] * 2;
	float wy2 = q[0] * q[2] * 2;
	float wz2 = q[0] * q[3] * 2;

	float xx = q[1] * q[1];
	float xy2 = q[1] * q[2] * 2;
	float xz2 = q[1] * q[3] * 2;

	float yy = q[2] * q[2];
	float yz2 = q[2] * q[3] * 2;

	float zz = q[3] * q[3];

	r[0] = ww + xx - yy - zz;
	r[3] = xy2 - wz2;
	r[6] = yz2 + wy2;

	r[1] = xy2 + xz2;
	r[4] = ww - xx + yy - zz;
	r[7] = yz2 - wx2;

	r[2] = xz2 - wy2;
	r[5] = yz2 + wx2;
	r[8] = ww - xx - yy + zz;
}
