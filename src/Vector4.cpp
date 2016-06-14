// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#include "Vector4.h"
#include <cmath>
#include <cstdio>

Vector4::Vector4() 
: x(0.0f),
  y(0.0f),
  z(0.0f),
  w(0.0f)
{
}

Vector4::Vector4(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   this->x = x;
   this->y = y;
   this->z = z;
   this->w = w;
}

Vector4::Vector4(const Vector4 &vec)
{
   this->x = vec.x;
   this->y = vec.y;
   this->z = vec.z;
   this->w = vec.w;
}

Vector4::Vector4(const GLfloat *vec)
{
   this->x = vec[0];
   this->y = vec[1];
   this->z = vec[2];
   this->w = vec[3];
}

Vector4::~Vector4() 
{
}

GLfloat Vector4::length() const
{
   GLfloat length = 0.0f;

   length = sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w));

   return length;
}

GLfloat Vector4::dot(const Vector4 &vec) const
{
   GLfloat dot = 0.0f;

   dot = (this->x * vec.x) + (this->y * vec.y) + (this->z * vec.z) + (this->w * vec.w);

   return dot;
}

GLfloat Vector4::angle(const Vector4 &vec) const
{
    GLfloat d = this->dot(vec);

    GLfloat a = this->length();    
    GLfloat b = vec.length();

    GLfloat s = d / (a*b);
    GLfloat angle = (GLfloat)acos((double)s);

    return angle;
}

Vector4 Vector4::cross(const Vector4 &vec) const
{
   Vector4 cross;

   cross.x = (this->y * vec.z) - (this->z * vec.y);
   cross.y = (this->z * vec.x) - (this->x * vec.z);
   cross.z = (this->x * vec.y) - (this->y * vec.x);
   cross.w = 1.0f;

   return cross;
}

void Vector4::normalize()
{
   GLfloat len = this->length();

   if(len > 0.0f)
   {
       this->x = this->x / len;
       this->y = this->y / len;
       this->z = this->z / len;   
       this->w = this->w / len;
   }
}

Vector4 Vector4::normalized() const
{
   GLfloat len = this->length();
   Vector4 normalized; 

   if(len > 0.0)
   {
       normalized.x = this->x / len;
       normalized.y = this->y / len;
       normalized.z = this->z / len;
       normalized.w = this->w / len;
   }

   return normalized;
}

void Vector4::set(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   this->x = x;
   this->y = y;
   this->z = z;
   this->w = w;
}

void Vector4::set(GLfloat *vec)
{
   this->x = vec[0];
   this->y = vec[1];
   this->z = vec[2];
   this->w = vec[3];
}

Vector4 Vector4::mulMatrix(GLfloat * matrix)
{
	Vector4 result;

	result.x = matrix[0] * this->x + matrix[1] * this->y + matrix[2] * this->z + matrix[3] * this->w;
	result.y = matrix[4] * this->x + matrix[5] * this->y + matrix[6] * this->z + matrix[7] * this->w;
	result.z = matrix[8] * this->x + matrix[9] * this->y + matrix[10] * this->z + matrix[11] * this->w;
	result.w = matrix[12] * this->x + matrix[13] * this->y + matrix[14] * this->z + matrix[15] * this->w;

	return result;
}

Vector4 &Vector4::get()
{
    return *this;
}

void Vector4::print()
{
    printf("x: %f, y: %f, z: %f, w: %f\n", x, y, z, w);
}

Vector4 &Vector4::operator =(const Vector4 &a)
{
   this->x = a.x;
   this->y = a.y;
   this->z = a.z;
   this->w = a.w;

   return *this;
}

Vector4 &Vector4::operator +=(const Vector4 &a)
{
   this->x += a.x;
   this->y += a.y;
   this->z += a.z;
   this->w += a.w;

   return *this;
}

Vector4 &Vector4::operator +=(GLfloat s)
{
   this->x += s;
   this->y += s;
   this->z += s;
   this->w += s;

   return *this;
}

Vector4 &Vector4::operator -=(const Vector4 &a)
{
   this->x -= a.x;
   this->y -= a.y;
   this->z -= a.z;
   this->w -= a.w;

   return *this;
}

Vector4 &Vector4::operator -=(GLfloat s)
{
   this->x -= s;
   this->y -= s;
   this->z -= s;
   this->w -= s;

   return *this;
}

Vector4 &Vector4::operator *=(const Vector4 &a)
{
   this->x *= a.x;
   this->y *= a.y;
   this->z *= a.z;
   this->w *= a.w;

   return *this;
}

Vector4 &Vector4::operator *=(GLfloat s)
{
   this->x *= s;
   this->y *= s;
   this->z *= s;
   this->w *= s;

   return *this;
}

Vector4 &Vector4::operator /=(const Vector4 &a)
{
   this->x /= a.x;
   this->y /= a.y;
   this->z /= a.z;
   this->w /= a.w;
   
   return *this;
}

Vector4 &Vector4::operator /=(GLfloat s)
{
   this->x /= s;
   this->y /= s;
   this->z /= s;
   this->w /= s;

   return *this;
}

Vector4 operator +(const Vector4 &a, const Vector4 &b)
{
   Vector4 r;

   r.x = a.x + b.x;
   r.y = a.y + b.y;
   r.z = a.z + b.z;
   r.w = a.w + b.w;

   return r;
}

Vector4 operator +(const Vector4 &a, GLfloat s)
{
   Vector4 r;

   r.x = a.x + s;
   r.y = a.y + s;
   r.z = a.z + s;
   r.w = a.w + s;

   return r;
}

Vector4 operator +(GLfloat s, const Vector4 &a)
{
   Vector4 r;

   r.x = a.x + s;
   r.y = a.y + s;
   r.z = a.z + s;
   r.w = a.w + s;

   return r;
}

Vector4 operator -(const Vector4 &a, const Vector4 &b)
{
   Vector4 r;

   r.x = a.x - b.x;
   r.y = a.y - b.y;
   r.z = a.z - b.z;
   r.w = a.w - b.w;

   return r;
}

Vector4 operator -(const Vector4 &a, GLfloat s)
{
   Vector4 r;

   r.x = a.x - s;
   r.y = a.y - s;
   r.z = a.z - s;
   r.w = a.w - s;

   return r;
}


Vector4 operator -(const Vector4 &a)
{
    Vector4 r;
	
	r.x = -a.x;
	r.y = -a.y;
	r.z = -a.z;
    r.w = -a.w;
	
	return r;
}

Vector4 operator *(const Vector4 &a, GLfloat s)
{
   Vector4 r;

   r.x = a.x * s;
   r.y = a.y * s;
   r.z = a.z * s;
   r.w = a.w * s;

   return r;
}

Vector4 operator *(const Vector4 &a, const Vector4 &b)
{
   Vector4 r;

   r.x = a.x * b.x;
   r.y = a.y * b.y;
   r.z = a.z * b.z;
   r.w = a.w * b.w;

   return r;
}

Vector4 operator /(const Vector4 &a, GLfloat s)
{
   Vector4 r;

   r.x = a.x / s;
   r.y = a.y / s;
   r.z = a.z / s;
   r.w = a.w / s;

   return r;
}

Vector4 operator /(const Vector4 &a, const Vector4 &b)
{
   Vector4 r;

   r.x = a.x / b.x;
   r.y = a.y / b.y;
   r.z = a.z / b.z;
   r.w = a.w / b.w;

   return r;
}

bool operator == (const Vector4 &a, const Vector4 &b)
{
    return(a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
}

bool operator != (const Vector4 &a, const Vector4 &b)
{
    return(a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w);
}

bool operator <= (const Vector4 &a, const Vector4 &b)
{
    return(a.x <= b.x && a.y <= b.y && a.z <= b.z && a.w <= b.w);
}

bool operator < (const Vector4 &a, const Vector4 &b)
{
    return(a.x < b.x && a.y < b.y && a.z < b.z && a.w < b.w);
}

bool operator >= (const Vector4 &a, const Vector4 &b)
{
    return(a.x >= b.x && a.y >= b.y && a.z >= b.z && a.w >= b.w);
}

bool operator > (const Vector4 &a, const Vector4 &b)
{
    return(a.x > b.x && a.y > b.y && a.z > b.z && a.w > b.w);
}

Vector4 Vector4::cross(const Vector4 &a, const Vector4 &b)
{
   Vector4 c;

   c.x = (a.y * b.z) - (a.z * b.y);
   c.y = (a.z * b.x) - (a.x * b.z);
   c.z = (a.x * b.y) - (a.y * b.x);
   c.w = 1.0f;

   return c;
}

Vector4 Vector4::normalized(const Vector4 &v)
{
   GLfloat len = v.length();
   Vector4 normalized; 

   if(len > 0.0)
   {
       normalized.x = v.x / len;
       normalized.y = v.y / len;
       normalized.z = v.z / len;  
       normalized.w = v.w / len;
   }

   return normalized;
}

GLfloat Vector4::dot(const Vector4 &a, const Vector4 &b)
{
   GLfloat dot = 0.0f;

   dot = (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);

   return dot;
}

GLfloat Vector4::length(const Vector4 &v)
{
   GLfloat length = 0.0f;

   length = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));

   return length;
}

Vector4 operator *(GLfloat s, const Vector4 &v)
{
    return (v * s);
}
