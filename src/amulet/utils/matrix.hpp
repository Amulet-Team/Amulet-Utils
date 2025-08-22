#pragma once

#include <array>
#include <cmath>
#include <stdexcept>
#include <tuple>
#include <vector>

#include <amulet/utils/dll.hpp>

// A simple 4x4 transformation matrix class

class QMatrix4x4;

namespace Amulet {

class AMULET_UTILS_EXPORT Matrix4x4 {
public:
    double data[4][4];

    // Uninitialised
    Matrix4x4();

    // Construct from raw array
    Matrix4x4(const double (&matrix)[4][4]);

    // Copy constructor
    Matrix4x4(const Matrix4x4& other);

    // Copy assign
    Matrix4x4& operator=(const Matrix4x4& other);

    // Static construction functions
    static Matrix4x4 identity_matrix();
    static Matrix4x4 scale_matrix(double sx, double sy, double sz);
    static Matrix4x4 translation_matrix(double dx, double dy, double dz);
    static Matrix4x4 rotation_x_matrix(double angle);
    static Matrix4x4 rotation_y_matrix(double angle);
    static Matrix4x4 rotation_z_matrix(double angle);

    // Construct from QMatrix4x4
    template <typename T = QMatrix4x4>
        requires std::is_same_v<T, QMatrix4x4>
    Matrix4x4(const T& matrix)
    {
        const float* data = matrix.data();
        for (auto i = 0; i < 4; i++) {
            for (auto j = 0; j < 4; j++) {
                *(data + i * 4 + j) = matrix(i, j);
            }
        }
    }

    // Convert to QMatrix4x4
    template <typename T = QMatrix4x4>
        requires std::is_same_v<T, QMatrix4x4>
    T get_qt_matrix() const
    {
        return T(
            data[0][0],
            data[0][1],
            data[0][2],
            data[0][3],
            data[1][0],
            data[1][1],
            data[1][2],
            data[1][3],
            data[2][0],
            data[2][1],
            data[2][2],
            data[2][3],
            data[3][0],
            data[3][1],
            data[3][2],
            data[3][3]);
    }

    // Accessor with bounds checking
    double get_element(std::uint8_t i, std::uint8_t j) const;
    void set_element(std::uint8_t i, std::uint8_t j, double value);

    // Multiply with another matrix and return the result
    Matrix4x4 operator*(const Matrix4x4& other) const;

    std::vector<std::array<double, 3>> operator*(
        const std::vector<std::array<double, 3>>& vectors) const;

    // Transform
    Matrix4x4 translate(double dx, double dy, double dz) const;
    Matrix4x4 scale(double dx, double dy, double dz) const;
    Matrix4x4 rotate_x(double rx) const;
    Matrix4x4 rotate_y(double ry) const;
    Matrix4x4 rotate_z(double rz) const;

    // Decompose into scale, rotation and displacement tuples
    std::tuple<
        std::tuple<double, double, double>,
        std::tuple<double, double, double>,
        std::tuple<double, double, double>>
    decompose() const;
};

} // namespace Amulet
