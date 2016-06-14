// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#ifndef VECTOR3
#define VECTOR3

#include "Global.h"

class Vector3
{   

public:
    Vector3();
    Vector3(GLfloat x, GLfloat y, GLfloat z);
    Vector3(const Vector3 &v);
    Vector3(const GLfloat *v);
    ~Vector3();

	// Returns the length of the vector.
    GLfloat length() const;
	// Returns the dot prouct between this vector and a given vector.
    GLfloat dot(const Vector3 &vec) const;
	// Returns angle between this vector and a given vector.
    GLfloat angle(const Vector3 &vec) const;
	// Returns the cross product between this vector and a given vector.
    Vector3 cross(const Vector3 &vec) const; 
	// Returns a normalized version of this vector.
    Vector3 normalized() const;
	// Normalizes this vector.
    void normalize();
    void set(GLfloat x, GLfloat y, GLfloat z);
    void set(GLfloat *vec);

    Vector3 mulMatrix(GLfloat *matrix);

    Vector3 &get();

    void print();

	// static functions
	static Vector3 normalized(const Vector3 & v);
	static Vector3 cross(const Vector3 & a, const Vector3 & b);
	static GLfloat dot(const Vector3 & a, const Vector3 & b);
	static GLfloat length(const Vector3 & v);

    // Assignment
    Vector3 &operator =  (const Vector3 &a);
    Vector3 &operator += (const Vector3 &a);
    Vector3 &operator += (GLfloat s);
    Vector3 &operator -= (const Vector3 &a);
    Vector3 &operator -= (GLfloat s);
    Vector3 &operator *= (const Vector3 &a);
    Vector3 &operator *= (GLfloat s);    
    Vector3 &operator /= (const Vector3 &a);
    Vector3 &operator /= (GLfloat s);
	  
    // Arithmetic
    friend Vector3 operator + (const Vector3 &a, const Vector3 &b);
    friend Vector3 operator + (const Vector3 &a, GLfloat s);	
    friend Vector3 operator + (GLfloat s, const Vector3 &a);	
    friend Vector3 operator - (const Vector3 &a, const Vector3 &b);
    friend Vector3 operator - (const Vector3 &a, GLfloat s);
    friend Vector3 operator - (const Vector3 &a);
	friend Vector3 operator * (const Vector3 &a, const Vector3 &b);
    friend Vector3 operator * (const Vector3 &a, GLfloat s);
    friend Vector3 operator * (GLfloat s, const Vector3 &a);
    friend Vector3 operator / (const Vector3 &a, const Vector3 &b);
    friend Vector3 operator / (const Vector3 &a, GLfloat s);

    // Comparison
    friend bool operator == (const Vector3 &a, const Vector3 &b);
    friend bool operator != (const Vector3 &a, const Vector3 &b);
    friend bool operator <= (const Vector3 &a, const Vector3 &b);
    friend bool operator <  (const Vector3 &a, const Vector3 &b);
    friend bool operator >= (const Vector3 &a, const Vector3 &b);
    friend bool operator >  (const Vector3 &a, const Vector3 &b);	
    
    GLfloat x;
    GLfloat y;
    GLfloat z;    
};


#endif
