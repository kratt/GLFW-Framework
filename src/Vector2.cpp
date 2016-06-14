// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#include "Vector2.h"
#include <cmath>
#include <cstdio>

Vector2::Vector2() 
: x(0.0f),
  y(0.0f)
{
}

Vector2::Vector2(GLfloat x, GLfloat y)
{
   this->x = x;
   this->y = y;
}

Vector2::Vector2(const Vector2 &v)
{
   this->x = v.x;
   this->y = v.y;
}

Vector2::Vector2(const GLfloat *v)
{
   this->x = v[0];
   this->y = v[1];
}

Vector2::~Vector2() 
{
}

GLfloat Vector2::length() const
{
   GLfloat length = 0.0f;

   length = sqrtf((this->x * this->x) + (this->y * this->y));

   return length;
}

GLfloat Vector2::dot(const Vector2 &vec) const
{
   GLfloat dot = 0.0f;

   dot = (this->x * vec.x) + (this->y * vec.y);

   return dot;
}

GLfloat Vector2::angle(const Vector2 &vec) const
{
    GLfloat d = this->dot(vec);

    GLfloat a = this->length();    
    GLfloat b = vec.length();

    GLfloat s = d / (a*b);
    GLfloat angle = (GLfloat)acos((double)s);

    return angle;
}

GLfloat Vector2::cross(const Vector2 &vec) const
{
   return (this->x * vec.y - this->y * vec.x);
}

void Vector2::normalize()
{
   GLfloat len = this->length();

   if(len > 0.0f)
   {
       this->x = this->x / len;
       this->y = this->y / len;  
   }
}

Vector2 Vector2::normalized() const
{
   GLfloat len = this->length();
   Vector2 normalized; 

   if(len > 0.0)
   {
       normalized.x = this->x / len;
       normalized.y = this->y / len;  
   }

   return normalized;
}

void Vector2::set(GLfloat x, GLfloat y)
{
   this->x = x;
   this->y = y;
}

void Vector2::set(GLfloat *vec)
{
   this->x = vec[0];
   this->y = vec[1];
}

Vector2 &Vector2::get()
{
    return *this;
}

void Vector2::print()
{
    printf("x: %f, y: %f\n", x, y);
}

Vector2 &Vector2::operator =(const Vector2 &a)
{
   this->x = a.x;
   this->y = a.y;

   return *this;
}

Vector2 &Vector2::operator +=(const Vector2 &a)
{
   this->x += a.x;
   this->y += a.y;

   return *this;
}

Vector2 &Vector2::operator +=(GLfloat s)
{
   this->x += s;
   this->y += s;

   return *this;
}

Vector2 &Vector2::operator -=(const Vector2 &a)
{
   this->x -= a.x;
   this->y -= a.y;

   return *this;
}

Vector2 &Vector2::operator -=(GLfloat s)
{
   this->x -= s;
   this->y -= s;

   return *this;
}

Vector2 &Vector2::operator *=(const Vector2 &a)
{
   this->x *= a.x;
   this->y *= a.y;

   return *this;
}

Vector2 &Vector2::operator *=(GLfloat s)
{
   this->x *= s;
   this->y *= s;

   return *this;
}

Vector2 &Vector2::operator /=(const Vector2 &a)
{
   this->x /= a.x;
   this->y /= a.y;
   
   return *this;
}

Vector2 &Vector2::operator /=(GLfloat s)
{
   this->x /= s;
   this->y /= s;

   return *this;
}

Vector2 operator +(const Vector2 &a, const Vector2 &b)
{
   Vector2 r;

   r.x = a.x + b.x;
   r.y = a.y + b.y;

   return r;
}

Vector2 operator +(const Vector2 &a, GLfloat s)
{
   Vector2 r;

   r.x = a.x + s;
   r.y = a.y + s;

   return r;
}

Vector2 operator +(GLfloat s, const Vector2 &a)
{
   Vector2 r;

   r.x = a.x + s;
   r.y = a.y + s;

   return r;
}

Vector2 operator -(const Vector2 &a, const Vector2 &b)
{
   Vector2 r;

   r.x = a.x - b.x;
   r.y = a.y - b.y;

   return r;
}

Vector2 operator -(const Vector2 &a, GLfloat s)
{
   Vector2 r;

   r.x = a.x - s;
   r.y = a.y - s;

   return r;
}


Vector2 operator -(const Vector2 &a)
{
    Vector2 r;
	
	r.x = -a.x;
	r.y = -a.y;
	
	return r;
}

Vector2 operator *(const Vector2 &a, GLfloat s)
{
   Vector2 r;

   r.x = a.x * s;
   r.y = a.y * s;

   return r;
}

Vector2 operator *(const Vector2 &a, const Vector2 &b)
{
   Vector2 r;

   r.x = a.x * b.x;
   r.y = a.y * b.y;

   return r;
}

Vector2 operator /(const Vector2 &a, GLfloat s)
{
   Vector2 r;

   r.x = a.x / s;
   r.y = a.y / s;

   return r;
}

Vector2 operator /(const Vector2 &a, const Vector2 &b)
{
   Vector2 r;

   r.x = a.x / b.x;
   r.y = a.y / b.y;

   return r;
}

bool operator == (const Vector2 &a, const Vector2 &b)
{
    return(a.x == b.x && a.y == b.y);
}

bool operator != (const Vector2 &a, const Vector2 &b)
{
    return(a.x != b.x || a.y != b.y);
}

bool operator <= (const Vector2 &a, const Vector2 &b)
{
    return(a.x <= b.x && a.y <= b.y);
}

bool operator < (const Vector2 &a, const Vector2 &b)
{
    return(a.x < b.x && a.y < b.y);
}

bool operator >= (const Vector2 &a, const Vector2 &b)
{
    return(a.x >= b.x && a.y >= b.y);
}

bool operator > (const Vector2 &a, const Vector2 &b)
{
    return(a.x > b.x && a.y > b.y);
}

Vector2 Vector2::mulMatrix(GLfloat *matrix)
{
    Vector2 result;

	//not implemented

    return result;
}



GLfloat Vector2::cross(const Vector2 &a, const Vector2 &b)
{
	return (a.x * b.y - a.y * b.x);
}

Vector2 Vector2::normalized(const Vector2 &v)
{
   GLfloat len = v.length();
   Vector2 normalized; 

   if(len > 0.0)
   {
       normalized.x = v.x / len;
       normalized.y = v.y / len;
   }

   return normalized;
}

GLfloat Vector2::dot(const Vector2 &a, const Vector2 &b)
{
   GLfloat dot = 0.0f;

   dot = (a.x * b.x) + (a.y * b.y);

   return dot;
}

GLfloat Vector2::length(const Vector2 &v)
{
   GLfloat length = 0.0f;

   length = sqrtf((v.x * v.x) + (v.y * v.y));

   return length;
}

Vector2 operator *(GLfloat s, const Vector2 &v)
{
    return (v * s);
}
