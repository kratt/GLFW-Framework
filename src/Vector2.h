// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#ifndef VECTOR2
#define VECTOR2

#include "Global.h"

class Vector2
{   

public:
    Vector2();
    Vector2(GLfloat x, GLfloat y);
    Vector2(const Vector2 &v);
    Vector2(const GLfloat *v);
    ~Vector2();

	// Returns the length of the vector.
    GLfloat length() const;
	// Returns the dot prouct between this vector and a given vector.
    GLfloat dot(const Vector2 &vec) const;
	// Returns angle between this vector and a given vector.
    GLfloat angle(const Vector2 &vec) const;
	// Returns the cross product between this vector and a given vector.
    GLfloat cross(const Vector2 &vec) const; 
	// Returns a normalized version of this vector.
    Vector2 normalized() const;
	// Normalizes this vector.
    void normalize();
    void set(GLfloat x, GLfloat y);
    void set(GLfloat *vec);

    Vector2 mulMatrix(GLfloat *matrix);
    Vector2 &get();

    void print();

	// static functions
	static Vector2 normalized(const Vector2 & v);
	static GLfloat cross(const Vector2 & a, const Vector2 & b);
	static GLfloat dot(const Vector2 & a, const Vector2 & b);
	static GLfloat length(const Vector2 & v);

    // Assignment
    Vector2 &operator =  (const Vector2 &a);
    Vector2 &operator += (const Vector2 &a);
    Vector2 &operator += (GLfloat s);
    Vector2 &operator -= (const Vector2 &a);
    Vector2 &operator -= (GLfloat s);
    Vector2 &operator *= (const Vector2 &a);
    Vector2 &operator *= (GLfloat s);    
    Vector2 &operator /= (const Vector2 &a);
    Vector2 &operator /= (GLfloat s);
	  
    // Arithmetic
    friend Vector2 operator + (const Vector2 &a, const Vector2 &b);
    friend Vector2 operator + (const Vector2 &a, GLfloat s);	
    friend Vector2 operator + (GLfloat s, const Vector2 &a);	
    friend Vector2 operator - (const Vector2 &a, const Vector2 &b);
    friend Vector2 operator - (const Vector2 &a, GLfloat s);
    friend Vector2 operator - (const Vector2 &a);
	friend Vector2 operator * (const Vector2 &a, const Vector2 &b);
    friend Vector2 operator * (const Vector2 &a, GLfloat s);
    friend Vector2 operator * (GLfloat s, const Vector2 &a);
    friend Vector2 operator / (const Vector2 &a, const Vector2 &b);
    friend Vector2 operator / (const Vector2 &a, GLfloat s);

    //Comparison
    friend bool operator == (const Vector2 &a, const Vector2 &b);
    friend bool operator != (const Vector2 &a, const Vector2 &b);
    friend bool operator <= (const Vector2 &a, const Vector2 &b);
    friend bool operator <  (const Vector2 &a, const Vector2 &b);
    friend bool operator >= (const Vector2 &a, const Vector2 &b);
    friend bool operator >  (const Vector2 &a, const Vector2 &b);	
    
    GLfloat x;
    GLfloat y;   
};


#endif
