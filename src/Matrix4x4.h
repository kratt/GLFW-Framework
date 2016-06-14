// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#ifndef MATRIX4X4
#define MATRIX4X4

#include <math.h>
#include <stdio.h>

#include "Global.h"
#include "Vector4.h"


class Matrix4x4;
class Matrix3x3;

Matrix4x4 operator *(GLfloat s, const Matrix4x4 &m);
Vector4 operator *(const Vector4 &v, const Matrix4x4 &m);
Vector4 operator *(const Matrix4x4 &m, const Vector4 &v);
Matrix4x4 transpose(const Matrix4x4 &m);
Matrix4x4 inverse(const Matrix4x4 &m);

class Matrix4x4
{   

public:
    Matrix4x4();
    Matrix4x4(GLfloat a11, GLfloat a12, GLfloat a13, GLfloat a14, GLfloat a21, GLfloat a22, GLfloat a23, GLfloat a24, 
              GLfloat a31, GLfloat a32, GLfloat a33, GLfloat a34, GLfloat a41, GLfloat a42, GLfloat a43, GLfloat a44);
    Matrix4x4(const Matrix4x4 &m);
    Matrix4x4(const Matrix3x3 &m);

    Matrix4x4(const GLfloat *vec);
    ~Matrix4x4();

    void set(GLfloat a11, GLfloat a12, GLfloat a13, GLfloat a14, GLfloat a21, GLfloat a22, GLfloat a23, GLfloat a24, 
             GLfloat a31, GLfloat a32, GLfloat a33, GLfloat a34, GLfloat a41, GLfloat a42, GLfloat a43, GLfloat a44);
    void set(GLfloat *mat);

    Matrix4x4 &get();
    void data(GLfloat *vec) const ;

	// Returns the inverse matrix.
    Matrix4x4 inverse();
	// Returns the transposed matrix.
    Matrix4x4 transpose();

	// Sets the current matrix to an identity matrix.
    void setToIdentity();
	// Sets the current matrix to a zero matrix.
    void setToZero();
	// Returns the determinant of the matrix.
    GLfloat determinant();

    void print();

    //Assignment
    Matrix4x4 &operator =  (const Matrix4x4 &a);
    Matrix4x4 &operator += (const Matrix4x4 &a);
    Matrix4x4 &operator += (GLfloat s);
    Matrix4x4 &operator -= (const Matrix4x4 &a);
    Matrix4x4 &operator -= (GLfloat s);
    Matrix4x4 &operator *= (const Matrix4x4 &m);
    Matrix4x4 &operator *= (GLfloat s);    
    Matrix4x4 &operator /= (GLfloat s);
	  
    //Arithmetic
    friend Matrix4x4 operator + (const Matrix4x4 &a, const Matrix4x4 &b);
    friend Matrix4x4 operator + (const Matrix4x4 &a, GLfloat s);	
    friend Matrix4x4 operator + (GLfloat s, const Matrix4x4 &a);	
    friend Matrix4x4 operator - (const Matrix4x4 &a, const Matrix4x4 &b);
    friend Matrix4x4 operator - (const Matrix4x4 &a, GLfloat s);
    friend Matrix4x4 operator - (const Matrix4x4 &a);
	friend Matrix4x4 operator * (const Matrix4x4 &a, const Matrix4x4 &b);    
    friend Matrix4x4 operator * (const Matrix4x4 &a, GLfloat s);
    friend Matrix4x4 operator * (GLfloat s, const Matrix4x4 &a);
    friend Matrix4x4 operator / (const Matrix4x4 &a, GLfloat s);

    //Comparison
    friend bool operator == (const Matrix4x4 &a, const Matrix4x4 &b);
    friend bool operator != (const Matrix4x4 &a, const Matrix4x4 &b);
    
    
    //OpenGL

	// Zero matrix.
    static const Matrix4x4 zero();
	// Identity matrix.
    static const Matrix4x4 identitiy();
	// Rotation matrix around x axis.
    static const Matrix4x4 rotateX(GLfloat angle);
	// Rotation matrix around y axis.
    static const Matrix4x4 rotateY(GLfloat angle);
	// Rotation matrix around z axis.
    static const Matrix4x4 rotateZ(GLfloat angle);
	// Rotation matrix around arbitrary axis.
    static const Matrix4x4 rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
	// Rotation matrix around arbitrary axis.
    static const Matrix4x4 rotate(GLfloat angle, Vector3 &n);
	// Scaling matrix.
    static const Matrix4x4 scale(const Vector3 &s);
	// Scaling matrix.
    static const Matrix4x4 scale(GLfloat x, GLfloat y, GLfloat z);
	// Translation matrix.
    static const Matrix4x4 translate(const Vector3 &t);
	// Translation matrix.
    static const Matrix4x4 translate(GLfloat x, GLfloat y, GLfloat z);
	// Perspective projection matrix.
    static const Matrix4x4 perspective(GLfloat fov, GLfloat ratio, GLfloat zNear, GLfloat zFar);
	// Viewing matrix.
    static const Matrix4x4 lookAt(const Vector3 &position, const Vector3 &center, const Vector3 &up);
	// Viewing matrix.
    static const Matrix4x4 lookAt(GLfloat px, GLfloat py, GLfloat pz, GLfloat cx, GLfloat cy, GLfloat cz, GLfloat ux, GLfloat uy, GLfloat uz);  
	// Creates a normal matrix from a given matrix.
    static const Matrix3x3 normalMatrix(const Matrix4x4 &model);

    GLfloat a11;
    GLfloat a21;
    GLfloat a31;  
    GLfloat a41;

    GLfloat a12;
    GLfloat a22;
    GLfloat a32;
    GLfloat a42;

    GLfloat a13;
    GLfloat a23;
    GLfloat a33;
    GLfloat a43;

    GLfloat a14;
    GLfloat a24;
    GLfloat a34;
    GLfloat a44;

private:
    GLfloat vec[16];     
};


#endif
