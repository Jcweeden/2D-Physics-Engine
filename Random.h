#ifndef CYCLONE_RANDOM_H
#define CYCLONE_RANDOM_H
#include "Vector2D.h"
#include <time.h>


class Random
{
public:
  /**
   * left bitwise rotation
   */

  unsigned rotl(unsigned n, unsigned r);
  /**
   * right bitwise rotation
   */
  unsigned rotr(unsigned n, unsigned r);

  /**
   * Creates a new random number stream with a seed based on
   * timing data.
   */
  Random();

  /**
   * Creates a new random stream with the given seed.
   */
  Random(unsigned seed);

  /**
   * Sets the seed value for the random stream.
   */
  void seed(unsigned seed);

  /**
   * Returns the next random bitstring from the stream. This is
   * the fastest method.
   */
  unsigned randomBits();

  /**
   * Returns a random floating point number between 0 and 1.
   */
  float randomReal();

  /**
   * Returns a random floating point number between 0 and scale.
   */
  float randomReal(float scale);

  /**
   * Returns a random floating point number between min and max.
   */
  float randomReal(float min, float max);

  /**
   * Returns a random integer less than the given value.
   */
  unsigned randomInt(unsigned max);

  /**
   * Returns a random binomially distributed number between -scale
   * and +scale.
   */
  float randomBinomial(float scale);

  /**
   * Returns a random vector where each component is binomially
   * distributed in the range (-scale to scale) [mean = 0.0f].
   */
  Vector2D randomVector(float scale);

  /**
   * Returns a random vector where each component is binomially
   * distributed in the range (-scale to scale) [mean = 0.0f],
   * where scale is the corresponding component of the given
   * vector.
   */
  Vector2D randomVector(const Vector2D &scale);

  /**
   * Returns a random vector in the cube defined by the given
   * minimum and maximum vectors. The probability is uniformly
   * distributed in this region.
   */
  Vector2D randomVector(const Vector2D &min, const Vector2D &max);

  /**
   * Returns a random vector where each component is binomially
   * distributed in the range (-scale to scale) [mean = 0.0f],
   * except the y coordinate which is zero.
   */
  //Vector2D randomXZVector(float scale);

  /**
   * Returns a random orientation (i.e. normalized) quaternion.
   */
  //Quaternion randomQuaternion();

private:
  // Internal mechanics
  int p1, p2;
  unsigned buffer[17];
};

#endif // CYCLONE_BODY_H
