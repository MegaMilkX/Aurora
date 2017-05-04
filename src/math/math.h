#ifndef AURORA_MATH_H
#define AURORA_MATH_H

#include <math.h>
#include <assert.h>

namespace Au{
namespace Math{
    
const float PI = 3.14159265359f;
const double dblPI = 3.14159265359;
    
inline float QRSqrt(const float &n)
{
    long i;
    float x2, y;
    const float threehalves = 1.5f;

    x2 = n * 0.5f;
    y = n;
    i = *(long*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;

    y = y * (threehalves - (x2 * y * y));

    return y;
}

inline float QSqrt(const float &n)
{
    return n * QRSqrt(n);
}

// Vectors ======================================================

template<typename T>
struct Vec4
{
    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w){}
    T length() const { return QSqrt(x*x + y*y + z*z + w*w); }
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };
    union { T w, a; };
    T operator[](const int &i) const {
        return *((&x) + i);
    }
    T& operator[](const int &i){
        return *((&x) + i);
    }
};

template<typename T>
struct Vec3
{
    Vec3() : x(0), y(0), z(0){}
    Vec3(T x, T y, T z) : x(x), y(y), z(z){}
    Vec3(const Vec4<T>& v) : x(v.x), y(v.y), z(v.z){}
    T length() const { return QSqrt(x*x + y*y + z*z); }
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };
    T operator[](const int &i) const {
        return *((&x) + i);
    }
    T& operator[](const int &i){
        return *((&x) + i);
    }
};

template<typename T>
struct Vec2
{
    Vec2() : x(0), y(0){}
    Vec2(T x, T y) : x(x), y(y){}
    Vec2(const Vec2<T>& v) : x(v.x), y(v.y){}
    union { T x, r; };
    union { T y, g; };
    T operator[](const int &i) const {
        return *((&x) + i);
    }
    T& operator[](const int &i){
        return *((&x) + i);
    }
};

typedef Vec4<float> Vec4f;
typedef Vec3<float> Vec3f;
typedef Vec2<float> Vec2f;
typedef Vec4<int> Vec4i;
typedef Vec3<int> Vec3i;
typedef Vec2<int> Vec2i;
typedef Vec2<unsigned int> Vec2ui;
typedef Vec3<unsigned int> Vec3ui;
typedef Vec4<unsigned int> Vec4ui;
typedef Vec4<double> Vec4d;
typedef Vec3<double> Vec3d;
typedef Vec2<double> Vec2d;

// Vector operators =============================================

//addition and substraction
inline Vec3f operator+(const Vec3f &a, const Vec3f &b){
    return Vec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline Vec4f operator+(const Vec4f &a, const Vec4f &b){
    return Vec4f(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline Vec3f operator+=(Vec3f &a, const Vec3f &b){
    return a = a + b;
}
inline Vec4f operator+=(Vec4f &a, const Vec4f &b){
    return a = a + b;
}

inline Vec3f operator-(const Vec3f &a, const Vec3f &b){
    return Vec3f(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline Vec4f operator-(const Vec4f &a, const Vec4f &b){
    return Vec4f(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline Vec3f operator-=(Vec3f &a, const Vec3f &b){
    return a = a - b;
}
inline Vec4f operator-=(Vec4f &a, const Vec4f &b){
    return a = a - b;
}

inline Vec3f operator-(const Vec3f &a){
    return Vec3f(-a.x, -a.y, -a.z);
}
inline Vec4f operator-(const Vec4f &a){
    return Vec4f(-a.x, -a.y, -a.z, -a.w);
}

//multiplication and division
inline Vec3f operator*(const Vec3f &a, const float &f){
    return Vec3f(a.x * f, a.y * f, a.z * f);
}
inline Vec3f operator*(const float &f, const Vec3f &a){
    return Vec3f(a.x * f, a.y * f, a.z * f);
}
inline Vec4f operator*(const Vec4f &a, const float &f){
    return Vec4f(a.x * f, a.y * f, a.z * f, a.w * f);
}
inline Vec4f operator*(const float &f, const Vec4f &a){
    return Vec4f(a.x * f, a.y * f, a.z * f, a.w * f);
}

inline Vec3f operator*=(Vec3f &a, const float &f){
    return a = a*f;
}
inline Vec4f operator*=(Vec4f &a, const float &f){
    return a = a*f;
}

inline Vec3f operator/(const Vec3f &a, const float &f){
    return Vec3f(a.x / f, a.y / f, a.z / f);
}
inline Vec4f operator/(const Vec4f &a, const float &f){
    return Vec4f(a.x / f, a.y / f, a.z / f, a.w / f);
}

inline Vec3f operator/=(Vec3f &a, const float &f){
    return a / f;
}

//
inline float Dot(const Vec3f &a, const Vec3f &b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3f Cross(const Vec3f &a, const Vec3f &b){
    return Vec3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline Vec3f Normalize(const Vec3f &a){
    if(a.length() == 0.0f)
        return a;
    return a / a.length();
}

inline Vec4f Normalize(const Vec4f &a){
    if(a.length() == 0.0f)
        return a;
    return a / a.length();
}

// Matrices =====================================================

struct Mat3f
{
    Mat3f(){}
    explicit Mat3f(const float &f)
    {
        v[0][0] = f;
        v[1][1] = f;
        v[2][2] = f;
    }
    Mat3f(float xx, float xy, float xz,
        float yx, float yy, float yz,
        float zx, float zy, float zz)
    {
        v[0].x = xx; v[0].y = xy; v[0].z = xz;
        v[1].x = yx; v[1].y = yy; v[1].z = yz;
        v[2].x = zx; v[2].y = zy; v[2].z = zz;
    }
    Vec3f operator[](const int &i) const {
        return v[i];
    }
    Vec3f& operator[](const int &i){
        return v[i];
    }
private:
    Vec3f v[3];
};

struct Mat4f
{
    Mat4f(){}
    explicit Mat4f(const float &f)
    {
        v[0][0] = f;
        v[1][1] = f;
        v[2][2] = f;
        v[3][3] = f;
    }

    void operator=(const Mat3f &m)
    {
        v[0][0] = m[0][0]; v[0][1] = m[0][1]; v[0][2] = m[0][2];
        v[1][0] = m[1][0]; v[1][1] = m[1][1]; v[1][2] = m[1][2];
        v[2][0] = m[2][0]; v[2][1] = m[2][1]; v[2][2] = m[2][2];
    }
    Vec4f operator[](const int &i) const {
        return v[i];
    }
    Vec4f& operator[](const int &i){
        return v[i];
    }
private:
    Vec4f v[4];
};

inline Mat4f operator+(const Mat4f &m0, const Mat4f &m1){
    Mat4f m;
    for (int i = 0; i < 4; ++i)
        m[i] = m0[i] + m1[i];
    return m;
}

inline Mat3f operator*(const Mat3f &m0, const Mat3f &m1){
    Mat3f m;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                m[i][j] += m0[k][j] * m1[i][k];
    return m;
}

inline Mat4f operator*(const Mat4f &m0, const Mat4f &m1){
    Mat4f m;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 4; ++k)
                m[i][j] += m0[k][j] * m1[i][k];
    return m;
}

//Rewrite these to be more cache-friendly
inline Vec4f operator*(const Mat4f &m, const Vec4f &v)
{
    Vec4f r;
    for (int i = 0; i < 4; ++i)
        for (int k = 0; k < 4; ++k)
            r[i] += m[k][i] * v[k];
    return r;
}

//Taking vec3 as vec4 and assuming v.w is zero
//so it transforms as a direction vector
inline Vec3f operator*(const Mat4f &m, const Vec3f &v)
{
    Vec3f r;
    for (int i = 0; i < 3; ++i)
        for (int k = 0; k < 3; ++k)
            r[i] += m[k][i] * v[k];
    return r;
}

inline Vec3f operator*(const Mat3f &m, const Vec3f &v)
{
    Vec3f r;
    for (int i = 0; i < 3; ++i)
        for (int k = 0; k < 3; ++k)
            r[i] += m[k][i] * v[k];
    return r;
}

//
inline Mat3f Transpose(const Mat3f &m)
{
    //TODO
    return Mat3f();
}

inline Mat4f Transpose(const Mat4f &m)
{
    //TODO
    return Mat4f();
}

inline Mat4f Scale(const Mat4f &m, const Vec3f &v)
{
    Mat4f r = m;
    r[0] *= v[0];
    r[1] *= v[1];
    r[2] *= v[2];
    return r;
}

inline Mat4f Translate(const Mat4f &m, const Vec3f &v)
{
    Mat4f r = m;
    r[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
    return r;
}

inline Mat4f Inverse(const Mat4f &m)
{
    float det;
    Mat4f inv;

    inv[0][0] = m[1][1] * m[2][2] * m[3][3] -
        m[1][1] * m[2][3] * m[3][2] -
        m[2][1] * m[1][2] * m[3][3] +
        m[2][1] * m[1][3] * m[3][2] +
        m[3][1] * m[1][2] * m[2][3] -
        m[3][1] * m[1][3] * m[2][2];

    inv[1][0] = -m[1][0] * m[2][2] * m[3][3] +
        m[1][0] * m[2][3] * m[3][2] +
        m[2][0] * m[1][2] * m[3][3] -
        m[2][0] * m[1][3] * m[3][2] -
        m[3][0] * m[1][2] * m[2][3] +
        m[3][0] * m[1][3] * m[2][2];

    inv[2][0] = m[1][0] * m[2][1] * m[3][3] -
        m[1][0] * m[2][3] * m[3][1] -
        m[2][0] * m[1][1] * m[3][3] +
        m[2][0] * m[1][3] * m[3][1] +
        m[3][0] * m[1][1] * m[2][3] -
        m[3][0] * m[1][3] * m[2][1];

    inv[3][0] = -m[1][0] * m[2][1] * m[3][2] +
        m[1][0] * m[2][2] * m[3][1] +
        m[2][0] * m[1][1] * m[3][2] -
        m[2][0] * m[1][2] * m[3][1] -
        m[3][0] * m[1][1] * m[2][2] +
        m[3][0] * m[1][2] * m[2][1];

    inv[0][1] = -m[0][1] * m[2][2] * m[3][3] +
        m[0][1] * m[2][3] * m[3][2] +
        m[2][1] * m[0][2] * m[3][3] -
        m[2][1] * m[0][3] * m[3][2] -
        m[3][1] * m[0][2] * m[2][3] +
        m[3][1] * m[0][3] * m[2][2];

    inv[1][1] = m[0][0] * m[2][2] * m[3][3] -
        m[0][0] * m[2][3] * m[3][2] -
        m[2][0] * m[0][2] * m[3][3] +
        m[2][0] * m[0][3] * m[3][2] +
        m[3][0] * m[0][2] * m[2][3] -
        m[3][0] * m[0][3] * m[2][2];

    inv[2][1] = -m[0][0] * m[2][1] * m[3][3] +
        m[0][0] * m[2][3] * m[3][1] +
        m[2][0] * m[0][1] * m[3][3] -
        m[2][0] * m[0][3] * m[3][1] -
        m[3][0] * m[0][1] * m[2][3] +
        m[3][0] * m[0][3] * m[2][1];

    inv[3][1] = m[0][0] * m[2][1] * m[3][2] -
        m[0][0] * m[2][2] * m[3][1] -
        m[2][0] * m[0][1] * m[3][2] +
        m[2][0] * m[0][2] * m[3][1] +
        m[3][0] * m[0][1] * m[2][2] -
        m[3][0] * m[0][2] * m[2][1];

    inv[0][2] = m[0][1] * m[1][2] * m[3][3] -
        m[0][1] * m[1][3] * m[3][2] -
        m[1][1] * m[0][2] * m[3][3] +
        m[1][1] * m[0][3] * m[3][2] +
        m[3][1] * m[0][2] * m[1][3] -
        m[3][1] * m[0][3] * m[1][2];

    inv[1][2] = -m[0][0] * m[1][2] * m[3][3] +
        m[0][0] * m[1][3] * m[3][2] +
        m[1][0] * m[0][2] * m[3][3] -
        m[1][0] * m[0][3] * m[3][2] -
        m[3][0] * m[0][2] * m[1][3] +
        m[3][0] * m[0][3] * m[1][2];

    inv[2][2] = m[0][0] * m[1][1] * m[3][3] -
        m[0][0] * m[1][3] * m[3][1] -
        m[1][0] * m[0][1] * m[3][3] +
        m[1][0] * m[0][3] * m[3][1] +
        m[3][0] * m[0][1] * m[1][3] -
        m[3][0] * m[0][3] * m[1][1];

    inv[3][2] = -m[0][0] * m[1][1] * m[3][2] +
        m[0][0] * m[1][2] * m[3][1] +
        m[1][0] * m[0][1] * m[3][2] -
        m[1][0] * m[0][2] * m[3][1] -
        m[3][0] * m[0][1] * m[1][2] +
        m[3][0] * m[0][2] * m[1][1];

    inv[0][3] = -m[0][1] * m[1][2] * m[2][3] +
        m[0][1] * m[1][3] * m[2][2] +
        m[1][1] * m[0][2] * m[2][3] -
        m[1][1] * m[0][3] * m[2][2] -
        m[2][1] * m[0][2] * m[1][3] +
        m[2][1] * m[0][3] * m[1][2];

    inv[1][3] = m[0][0] * m[1][2] * m[2][3] -
        m[0][0] * m[1][3] * m[2][2] -
        m[1][0] * m[0][2] * m[2][3] +
        m[1][0] * m[0][3] * m[2][2] +
        m[2][0] * m[0][2] * m[1][3] -
        m[2][0] * m[0][3] * m[1][2];

    inv[2][3] = -m[0][0] * m[1][1] * m[2][3] +
        m[0][0] * m[1][3] * m[2][1] +
        m[1][0] * m[0][1] * m[2][3] -
        m[1][0] * m[0][3] * m[2][1] -
        m[2][0] * m[0][1] * m[1][3] +
        m[2][0] * m[0][3] * m[1][1];

    inv[3][3] = m[0][0] * m[1][1] * m[2][2] -
        m[0][0] * m[1][2] * m[2][1] -
        m[1][0] * m[0][1] * m[2][2] +
        m[1][0] * m[0][2] * m[2][1] +
        m[2][0] * m[0][1] * m[1][2] -
        m[2][0] * m[0][2] * m[1][1];

    det = m[0][0] * inv[0][0] + m[0][1] * inv[1][0] + m[0][2] * inv[2][0] + m[0][3] * inv[3][0];

    assert(det != 0);

    det = 1.0f / det;

    for (int j = 0; j < 4; ++j)
    for (int i = 0; i < 4; ++i)
        inv[i][j] *= det;

    return inv;
}

inline Mat4f Perspective(float fov, float aspect, float znear, float zfar)
{
    assert(aspect != 0.0f);
    assert(zfar != znear);

    float tanHalfFovy = tanf(fov / 2.0f);

    Mat4f r(0);
    r[0][0] = 1.0f / (aspect * tanHalfFovy);
    r[1][1] = 1.0f / (tanHalfFovy);
    r[2][2] = -(zfar + znear) / (zfar - znear);
    r[2][3] = -1.0f;
    r[3][2] = -(2.0f * zfar * znear) / (zfar - znear);
    return r;
}

inline Mat4f Ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
    Mat4f r(1.0f);
    r[0][0] = 2.0f / (right - left);
    r[1][1] = 2.0f / (top - bottom);
    r[2][2] = -2.0f / (zfar - znear);
    r[3][0] = -(right + left) / (right - left);
    r[3][1] = -(top + bottom) / (top - bottom);
    r[3][2] = -(zfar + znear) / (zfar - znear);
    return r;
}

// Quaternion ===================================================

typedef Vec4f Quat;

inline Quat operator*(const Quat &q0, const Quat &q1)
{
    return Quat((q0.w * q1.x + q1.w * q0.x) + (q0.y * q1.z - q1.y * q0.z),
        (q0.w * q1.y + q1.w * q0.y) + (q1.x * q0.z - q0.x * q1.z), //Inverted, since y axis rotation is inverted
        (q0.w * q1.z + q1.w * q0.z) + (q0.x * q1.y - q1.x * q0.y),
        (q1.w * q0.w) - (q1.x * q0.x) - (q1.y * q0.y) - (q1.z * q0.z));
}

inline Vec3f Rotate(const Quat &q, const Vec3f &v)
{
    //TODO
}

inline Quat AngleAxis(const float &a, const Vec3f &axis)
{
    float s = sinf(a * 0.5f);
    return Quat(axis.x * s, axis.y * s, axis.z * s, cosf(a*0.5f));
}

// Axis-aligned bounding box ====================================

template<typename T>
struct AABB3
{
    AABB3()
    : center(), size()
    {}
    AABB3(T cx, T cy, T cz, T sx, T sy, T sz)
    : center(cx, cy, cz), size(sx, sy, sz)
    {}
    Vec3<T> center;
    Vec3<T> size;
};

typedef AABB3<float> AABB3f;

inline bool Intersects(const AABB3f& a, const AABB3f& b)
{
    return (fabs(a.center.x - b.center.x) * 2.0f < (a.size.x + b.size.x)) &&
           (fabs(a.center.y - b.center.y) * 2.0f < (a.size.y + b.size.y)) &&
           (fabs(a.center.z - b.center.z) * 2.0f < (a.size.z + b.size.z));
}

// Check if b is inside of a
inline bool Contains(const AABB3f& a, const AABB3f& b)
{
    float xcdist = fabsf(a.center.x - b.center.x);
    float ycdist = fabsf(a.center.y - b.center.y);
    float zcdist = fabsf(a.center.z - b.center.z);
    float xszdif = a.size.x - b.size.x;
    float yszdif = a.size.y - b.size.y;
    float zszdif = a.size.z - b.size.z;
    
    return (xcdist - xszdif < 0.0f) &&
           (ycdist - yszdif < 0.0f) &&
           (zcdist - zszdif < 0.0f);
}

///////////////////////////////////////////////
//Conversion
///////////////////////////////////////////////
inline Mat3f ToMat3(const Quat &q)
{
    Mat3f m = { 1 - 2 * q.y * q.y - 2 * q.z * q.z, q.z * 2 * q.w + 2 * q.x * q.y, -q.y * 2 * q.w + 2 * q.x * q.z,

        -q.z * 2 * q.w + 2 * q.x * q.y, 1 - 2 * q.x * q.x - 2 * q.z * q.z, q.x * 2 * q.w + 2 * q.y * q.z,

        q.y * 2 * q.w + 2 * q.x * q.z, -q.x * 2 * q.w + 2 * q.y * q.z, 1 - 2 * q.x * q.x - 2 * q.y * q.y };
    return m;
}

inline Mat3f ToMat3(const Mat4f& m4)
{
    Mat3f m3;
    for(unsigned i = 0; i < 3; ++i)
        for(unsigned j = 0; j < 3; ++j)
            m3[i][j] = m4[i][j];
    return m3;
}

inline Mat4f ToMat4(const Quat &q)
{
    Mat4f m(1.0f);
    m = ToMat3(q);
    return m;
}

inline Quat EulerToQuat(Vec3f& euler)
{
    Au::Math::Quat qx = AngleAxis(euler.x, Au::Math::Vec3f(1.0f, 0.0f, 0.0f));
    Au::Math::Quat qy = AngleAxis(euler.y, Au::Math::Vec3f(0.0f, 1.0f, 0.0f));
    Au::Math::Quat qz = AngleAxis(euler.z, Au::Math::Vec3f(0.0f, 0.0f, 1.0f));
    return qx * qy * qz;
}

inline Quat ToQuat(Mat3f& m)
{
    Quat q;
    
    q.w = QSqrt(1.0f + m[0][0] + m[1][1] + m[2][2]) / 2.0f;
    q.x = (m[2][1] - m[1][2]) / (4.0f * q.w);
    q.y = (m[0][2] - m[2][0]) / (4.0f * q.w);
    q.z = (m[1][0] - m[0][1]) / (4.0f * q.w);
    
    return q;
}

///////////////////////////////////////////////
//Interpolation
///////////////////////////////////////////////
inline float Clamp(float f, float a, float b)
{
    f = f < a ? a : (f > b ? b : f);
    return f;
}

inline float Smoothstep(float a, float b, float x)
{
    x = Clamp((x - a) / (b - a), 0.0f, 1.0f);
    return x * x * (3 - 2 * x);
}
   
}
}

#endif
