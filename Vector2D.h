#ifndef _Vector2D_
#define _Vector2D_

#include <iostream>
#include <math.h>

class Vector2D
{
public:

Vector2D(float x, float y): m_x(x), m_y(y) {}

  float getX() {return m_x;}
  float getY() {return m_y;}

  void setX(float x) {m_x = x;}
  void setY(float y) {m_y = y;}  
  
  //returns length of the vector
  float magnitude() {return sqrt(m_x * m_x + m_y * m_y); }

  float squareMagnitude()
  {
    return (m_x*m_x) + (m_y*m_y);
  }
  
 //addition of vectors
  Vector2D operator+(const Vector2D& v2) const
  {
    return Vector2D(m_x + v2.m_x, m_y + v2.m_y);
  }
  
  void operator+=(const Vector2D& v)
  {
    m_x += v.m_x;
    m_y += v.m_y;
  }
  
/*
  friend Vector2D& operator+=(Vector2D& v1, const Vector2D v2)
  {
    v1.m_x += v2.m_x;
    v1.m_y += v2.m_y;

    return v1;
  }*/
  
  //subtraction of vectors
  Vector2D operator-(const Vector2D& v2) {
    return Vector2D(m_x - v2.m_x, m_y - v2.m_y);
  }

  void operator-=(const Vector2D& v)
  {
    m_x -= v.m_x;
    m_y -= v.m_y;
  }
  
  /*friend Vector2D& operator-=(Vector2D v1, const Vector2D v2) {

    v1.m_x -= v2.m_x;
    v1.m_y -= v2.m_y;
    
    return v1;
    }*/
  
  //dot product - scalar product (a . b)
  Vector2D operator*(float val) {
    return Vector2D(m_x * val, m_y * val);
  }

 void operator*=(float scalar) {
    m_x *= scalar;
    m_y *= scalar;
 }

  //division of vectors

  Vector2D operator/(float scalar) {
    return Vector2D(m_x/scalar, m_y/scalar);
  }

  Vector2D& operator/=(float scalar) {
    m_x /= scalar;
    m_y /= scalar;

    return *this;
  }

  //normalise

  void normalise()
  {
    float l = magnitude();
    if (l > 0) {
      (*this) *= 1 / l;
    }
  }


  void addScaledVector(/*const*/ Vector2D v1, float scale)
  {
    m_x += v1.getX() * scale;
    m_y += v1.getY() * scale;
  }

  Vector2D componentProduct(/*const*/ Vector2D &v1)
  {
    return Vector2D(m_x * v1.getX(), m_y * v1.getY());
  }

  Vector2D componentProductUpdate(/*const*/ Vector2D &v1)
  {
    m_x *= v1.getX();
    m_y *= v1.getY();
  }

  //vector product for 2D
  float crossproduct(/*const*/ Vector2D &v2) const
  {
    return (m_x * v2.getY()) - (m_y * v2.getX());
  }


  void clear()
  {
    m_x = 0;
    m_y = 0;
  }

  
private:

  float m_x;
  float m_y;


};

#endif
