// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#ifndef VECTOR4
#define VECTOR4

#include "Global.h"

class Vector4
{   

public:
    Vector4();
    Vector4(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    Vector4(const Vector4 &vec);
    Vector4(const GLfloat *vec);
    ~Vector4();

	// Returns the length of the vector.
    GLfloat length() const;
	// Returns the dot prouct between this vector and a given vector.
    GLfloat dot(const Vector4 &vec) const;
	// Returns angle between this vector and a given vector.
    GLfloat angle(const Vector4 &vec) const;
	// Returns the cross product between this vector and a given vector.
    Vector4 cross(const Vector4 &vec) const; 
	// Returns a normalized version of this vector.
    Vector4 normalized() const;
	// Normalizes this vector.
    void normalize();
    void set(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void set(GLfloat *vec);

	Vector4 mulMatrix(GLfloat *matrix);

    Vector4 &get();

    void print();

	// static functions
	static Vector4 normalized(const Vector4 & v);
	static Vector4 cross(const Vector4 & a, const Vector4 & b);
	static GLfloat dot(const Vector4 & a, const Vector4 & b);
	static GLfloat length(const Vector4 & v);

    // Assignment
    Vector4 &operator =  (const Vector4 &a);
    Vector4 &operator += (const Vector4 &a);
    Vector4 &operator += (GLfloat s);
    Vector4 &operator -= (const Vector4 &a);
    Vector4 &operator -= (GLfloat s);
    Vector4 &operator *= (const Vector4 &a);
    Vector4 &operator *= (GLfloat s);    
    Vector4 &operator /= (const Vector4 &a);
    Vector4 &operator /= (GLfloat s);
	  
    // Arithmetic
    friend Vector4 operator + (const Vector4 &a, const Vector4 &b);
    friend Vector4 operator + (const Vector4 &a, GLfloat s);	
    friend Vector4 operator + (GLfloat s, const Vector4 &a);	
    friend Vector4 operator - (const Vector4 &a, const Vector4 &b);
    friend Vector4 operator - (const Vector4 &a, GLfloat s);
    friend Vector4 operator - (const Vector4 &a);
	friend Vector4 operator * (const Vector4 &a, const Vector4 &b);
    friend Vector4 operator * (const Vector4 &a, GLfloat s);
    friend Vector4 operator * (GLfloat s, const Vector4 &a);
    friend Vector4 operator / (const Vector4 &a, const Vector4 &b);
    friend Vector4 operator / (const Vector4 &a, GLfloat s);

    //Comparison
    friend bool operator == (const Vector4 &a, const Vector4 &b);
    friend bool operator != (const Vector4 &a, const Vector4 &b);
    friend bool operator <= (const Vector4 &a, const Vector4 &b);
    friend bool operator <  (const Vector4 &a, const Vector4 &b);
    friend bool operator >= (const Vector4 &a, const Vector4 &b);
    friend bool operator >  (const Vector4 &a, const Vector4 &b);	
    
    GLfloat x;
    GLfloat y;
    GLfloat z;  
    GLfloat w;
};


#endif
