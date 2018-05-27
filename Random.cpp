#include "Random.h"

Random::Random()
{
    seed(0);
}

Random::Random(unsigned seed)
{
    Random::seed(seed);
}

void Random::seed(unsigned s)
{
    if (s == 0) {
        s = (unsigned)clock();
    }

    // Fill the buffer with some basic random numbers
    for (unsigned i = 0; i < 17; i++)
    {
        // Simple linear congruential generator
        s = s * 2891336453 + 1;
        buffer[i] = s;
    }

    // Initialize pointers into the buffer
    p1 = 0;  p2 = 10;
}

unsigned Random::rotl(unsigned n, unsigned r)
{
          return        (n << r) |
                          (n >> (32 - r));
}

unsigned Random::rotr(unsigned n, unsigned r)
{
          return        (n >> r) |
                                (n << (32 - r));
}

unsigned Random::randomBits()
{
    unsigned result;

    // Rotate the buffer and store it back to itself
    result = buffer[p1] = rotl(buffer[p2], 13) + rotl(buffer[p1], 9);

    // Rotate pointers
    if (--p1 < 0) p1 = 16;
    if (--p2 < 0) p2 = 16;

    // Return result
    return result;
}

#ifdef SINGLE_PRECISION
float Random::randomReal()
{
    // Get the random number
    unsigned bits = randomBits();

    // Set up a reinterpret structure for manipulation
    union {
        float value;
        unsigned word;
    } convert;

    // Now assign the bits to the word. This works by fixing the ieee
    // sign and exponent bits (so that the size of the result is 1-2)
    // and using the bits to create the fraction part of the float.
    convert.word = (bits >> 9) | 0x3f800000;

    // And return the value
    return convert.value - 1.0f;
}
#else
float Random::randomReal()
{
    // Get the random number
    unsigned bits = randomBits();

    // Set up a reinterpret structure for manipulation
    union {
        float value;
        unsigned words[2];
    } convert;

    // Now assign the bits to the words. This works by fixing the ieee
    // sign and exponent bits (so that the size of the result is 1-2)
    // and using the bits to create the fraction part of the float. Note
    // that bits are used more than once in this process.
    convert.words[0] =  bits << 20; // Fill in the top 16 bits
    convert.words[1] = (bits >> 12) | 0x3FF00000; // And the bottom 20

    // And return the value
    return convert.value - 1.0;
}
#endif

float Random::randomReal(float min, float max)
{
    return randomReal() * (max-min) + min;
}

float Random::randomReal(float scale)
{
    return randomReal() * scale;
}

unsigned Random::randomInt(unsigned max)
{
    return randomBits() % max;
}

float Random::randomBinomial(float scale)
{
    return (randomReal()-randomReal())*scale;
}

/*Quaternion Random::randomQuaternion()
{
    Quaternion q(
        randomReal(),
        randomReal(),
        randomReal(),
        randomReal()
        );
    q.normalise();
    return q;
}*/

Vector2D Random::randomVector(float scale)
{
    return Vector2D(
        randomBinomial(scale),
        randomBinomial(scale)
       );
}

/*Vector2D Random::randomXZVector(float scale)
{
    return Vector2D(
        randomBinomial(scale),
        0,
        randomBinomial(scale)
        );
        }*/

Vector2D Random::randomVector(const Vector2D &scale)
{
    return Vector2D(
        randomBinomial(scale.getX()),
        randomBinomial(scale.getY())
       );
}

Vector2D Random::randomVector(const Vector2D &min, const Vector2D &max)
{
    return Vector2D(
        randomReal(min.getX(), max.getX()),
        randomReal(min.getY(), max.getY())
       );
}
