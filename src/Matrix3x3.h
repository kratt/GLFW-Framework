// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#ifndef MATRIX3X3
#define MATRIX3X3

#include "Global.h"
#include "Vector3.h"


class Matrix3x3;
class Matrix4x4;

Matrix3x3 operator *(GLfloat s, const Matrix3x3 &m);
Vector3 operator *(const Vector3 &v, const Matrix3x3 &m);
Vector3 operator *(const Matrix3x3 &m, const Vector3 &v);
Matrix3x3 transpose(const Matrix3x3 &m);
Matrix3x3 inverse(const Matrix3x3 &m);

class Matrix3x3
{   

public:
    Matrix3x3();
    Matrix3x3(GLfloat a11, GLfloat a12, GLfloat a13, GLfloat a21, GLfloat a22, GLfloat a23, GLfloat a31, GLfloat a32, GLfloat a33);
    Matrix3x3(const Matrix3x3 &m);
    Matrix3x3(const Matrix4x4 &m);
    Matrix3x3(const GLfloat *vec);
    ~Matrix3x3();

    void set(GLfloat a11, GLfloat a12, GLfloat a13, GLfloat a21, GLfloat a22, GLfloat a23, GLfloat a31, GLfloat a32, GLfloat a33);
    void set(GLfloat *mat);

    Matrix3x3 &get();
    void data(GLfloat *vec) const ;

	// Returns the inverse matrix.
    Matrix3x3 inverse();
	// Returns the transposed matrix.
    Matrix3x3 transpose();

	// Sets the current matrix to an identity matrix.
    void setToIdentity();
	// Sets the current matrix to a zero matrix.
    void setToZero();
	// Returns the determinant of the matrix.
    GLfloat determinant();

    void print();

    //Assignment
    Matrix3x3 &operator =  (const Matrix3x3 &a);
    Matrix3x3 &operator += (const Matrix3x3 &a);
    Matrix3x3 &operator += (GLfloat s);
    Matrix3x3 &operator -= (const Matrix3x3 &a);
    Matrix3x3 &operator -= (GLfloat s);
    Matrix3x3 &operator *= (const Matrix3x3 &m);
    Matrix3x3 &operator *= (GLfloat s);    
    Matrix3x3 &operator /= (GLfloat s);
	  
    //Arithmetic
    friend Matrix3x3 operator + (const Matrix3x3 &a, const Matrix3x3 &b);
    friend Matrix3x3 operator + (const Matrix3x3 &a, GLfloat s);	
    friend Matrix3x3 operator + (GLfloat s, const Matrix3x3 &a);	
    friend Matrix3x3 operator - (const Matrix3x3 &a, const Matrix3x3 &b);
    friend Matrix3x3 operator - (const Matrix3x3 &a, GLfloat s);
    friend Matrix3x3 operator - (const Matrix3x3 &a);
	friend Matrix3x3 operator * (const Matrix3x3 &a, const Matrix3x3 &b);    
    friend Matrix3x3 operator * (const Matrix3x3 &a, GLfloat s);
    friend Matrix3x3 operator * (GLfloat s, const Matrix3x3 &a);
    friend Matrix3x3 operator / (const Matrix3x3 &a, GLfloat s);

    //Comparison
    friend bool operator == (const Matrix3x3 &a, const Matrix3x3 &b);
    friend bool operator != (const Matrix3x3 &a, const Matrix3x3 &b);
    
    GLfloat a11;
    GLfloat a21;
    GLfloat a31;    

    GLfloat a12;
    GLfloat a22;
    GLfloat a32;

    GLfloat a13;
    GLfloat a23;
    GLfloat a33;

private:
    GLfloat vec[9];     
};


#endif
