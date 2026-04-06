/**
 * @file Vector.h
 * @author Navya Malik
 * @brief Simple 2D vector class for representing position and velocity.
 *
 * This class provides basic 2D vector functionality for working with
 * positions, velocities, and other two-dimensional quantities. It supports
 * addition, subtraction, and scalar multiplication operations.
 */

#ifndef PROJECT1_VECTOR_H
#define PROJECT1_VECTOR_H

/**
 * @class Vector
 * @brief Represents a simple 2D vector.
 *
 * The Vector class encapsulates two double values (X and Y) that represent
 * a two-dimensional coordinate or direction. It provides basic arithmetic
 * operations and getter/setter methods.
 */
class Vector
{
private:
    /// X component of the vector
    double mX;

    /// Y component of the vector
    double mY;

public:
    /**
     * @brief Construct a new Vector object.
     * @param x X component (default 0)
     * @param y Y component (default 0)
     */
    Vector(double x = 0, double y = 0) : mX(x), mY(y) {}

    /**
     * @brief Get the X component.
     * @return The X component value.
     */
    double X() const { return mX; }

    /**
     * @brief Get the Y component.
     * @return The Y component value.
     */
    double Y() const { return mY; }

    /**
     * @brief Set the X component.
     * @param x New X value.
     */
    void SetX(double x) { mX = x; }

    /**
     * @brief Set the Y component.
     * @param y New Y value.
     */
    void SetY(double y) { mY = y; }

    /**
     * @brief Add two vectors.
     * @param other The vector to add.
     * @return A new Vector that is the sum of this and the other vector.
     */
    Vector operator+(const Vector& other) const { return {mX + other.mX, mY + other.mY}; }

    /**
     * @brief Subtract one vector from another.
     * @param other The vector to subtract.
     * @return A new Vector that is the difference of this and the other vector.
     */
    Vector operator-(const Vector& other) const { return {mX - other.mX, mY - other.mY}; }

    /**
     * @brief Scale the vector by a scalar value.
     * @param s The scalar multiplier.
     * @return A new Vector scaled by the given scalar.
     */
    Vector operator*(double s) const { return {mX * s, mY * s}; }
};

#endif // PROJECT1_VECTOR_H
