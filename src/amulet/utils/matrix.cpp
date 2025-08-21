#include <span>

#include "matrix.hpp"

namespace Amulet {

// Uninitialised
Matrix4x4::Matrix4x4() { };

// Construct from raw array
Matrix4x4::Matrix4x4(const double (&matrix)[4][4])
{
    for (auto i = 0; i < 4; i++) {
        for (auto j = 0; j < 4; j++) {
            data[i][j] = matrix[i][j];
        }
    }
}

// Copy constructor
Matrix4x4::Matrix4x4(const Matrix4x4& other)
{
    for (auto i = 0; i < 4; i++) {
        for (auto j = 0; j < 4; j++) {
            data[i][j] = other.data[i][j];
        }
    }
}

// Copy assign
Matrix4x4& Matrix4x4::operator=(const Matrix4x4& other)
{
    for (auto i = 0; i < 4; i++) {
        for (auto j = 0; j < 4; j++) {
            data[i][j] = other.data[i][j];
        }
    }
    return *this;
}

Matrix4x4 Matrix4x4::identity_matrix()
{
    return scale_matrix(1, 1, 1);
}

Matrix4x4 Matrix4x4::scale_matrix(double sx, double sy, double sz)
{
    return Matrix4x4 { {
        { sx, 0, 0, 0 },
        { 0, sy, 0, 0 },
        { 0, 0, sz, 0 },
        { 0, 0, 0, 1 },
    } };
}

Matrix4x4 Matrix4x4::translation_matrix(double dx, double dy, double dz)
{
    return Matrix4x4 { {
        { 1, 0, 0, dx },
        { 0, 1, 0, dy },
        { 0, 0, 1, dz },
        { 0, 0, 0, 1 },
    } };
}

Matrix4x4 Matrix4x4::rotation_x_matrix(double angle)
{
    auto s = std::sin(angle);
    auto c = std::cos(angle);

    return Matrix4x4 { {
        { 1, 0, 0, 0 },
        { 0, c, -s, 0 },
        { 0, s, c, 0 },
        { 0, 0, 0, 1 },
    } };
}

Matrix4x4 Matrix4x4::rotation_y_matrix(double angle)
{
    auto s = std::sin(angle);
    auto c = std::cos(angle);

    return Matrix4x4 { {
        { c, 0, s, 0 },
        { 0, 1, 0, 0 },
        { -s, 0, c, 0 },
        { 0, 0, 0, 1 },
    } };
}

Matrix4x4 Matrix4x4::rotation_z_matrix(double angle)
{
    auto s = std::sin(angle);
    auto c = std::cos(angle);

    return Matrix4x4 { {
        { c, -s, 0, 0 },
        { s, c, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 },
    } };
}

// Accessor with bounds checking
double Matrix4x4::get_element(std::uint8_t i, std::uint8_t j) const
{
    if (i > 3 || j > 3) {
        throw std::runtime_error("Matrix index is out of bounds");
    }
    return data[i][j];
}

void Matrix4x4::set_element(std::uint8_t i, std::uint8_t j, double value)
{
    if (i > 3 || j > 3) {
        throw std::runtime_error("Matrix index is out of bounds");
    }
    data[i][j] = value;
}

// Multiply with another matrix and return the result
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
    Matrix4x4 matrix;
    for (auto i = 0; i < 4; i++) {
        for (auto j = 0; j < 4; j++) {
            matrix.data[i][j] = 0;

            for (auto k = 0; k < 4; k++) {
                matrix.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return matrix;
}

std::vector<std::array<double, 3>> Matrix4x4::operator*(
    const std::vector<std::array<double, 3>>& vectors) const
{
    std::vector<std::array<double, 3>> out(vectors.size());

    for (size_t v = 0; v < vectors.size(); v++) {
        for (auto i = 0; i < 3; i++) {
            out[v][i] = vectors[v][0] * data[i][0] + vectors[v][1] * data[i][1] + vectors[v][2] * data[i][2] + data[i][3];
        }
    }

    return out;
}

Matrix4x4 Matrix4x4::translate(double dx, double dy, double dz) const
{
    return translation_matrix(dx, dy, dz) * (*this);
}

Matrix4x4 Matrix4x4::scale(double dx, double dy, double dz) const
{
    return scale_matrix(dx, dy, dz) * (*this);
}

Matrix4x4 Matrix4x4::rotate_x(double rx) const
{
    return rotation_x_matrix(rx) * (*this);
}

Matrix4x4 Matrix4x4::rotate_y(double ry) const
{
    return rotation_y_matrix(ry) * (*this);
}

Matrix4x4 Matrix4x4::rotate_z(double rz) const
{
    return rotation_z_matrix(rz) * (*this);
}

static double dot_product(const std::array<double, 3>& a, const std::array<double, 3>& b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static std::array<double, 3> cross_product(const std::array<double, 3>& a, const std::array<double, 3>& b)
{
    return {
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    };
}

// Decompose into scale, rotation and displacement tuples
std::tuple<
    std::tuple<double, double, double>,
    std::tuple<double, double, double>,
    std::tuple<double, double, double>>
Matrix4x4::decompose() const
{
    // https://gist.github.com/Aerilius/0cbc46271c163746717902b36bea8fd4

    // Create a temporary matrix
    double m[4][4];
    for (auto i = 0; i < 4; i++) {
        for (auto j = 0; j < 4; j++) {
            m[i][j] = data[i][j];
        }
    }

    // Extract translation
    std::tuple<double, double, double> displacement(
        m[0][3],
        m[1][3],
        m[2][3]);
    // Remove translation
    m[0][3] = 0;
    m[1][3] = 0;
    m[2][3] = 0;

    // Extract scale
    double scale[3];
    scale[0] = m[3][3] * std::pow(std::pow(m[0][0], 2) + std::pow(m[1][0], 2) + std::pow(m[2][0], 2), 0.5);
    scale[1] = m[3][3] * std::pow(std::pow(m[0][1], 2) + std::pow(m[1][1], 2) + std::pow(m[2][1], 2), 0.5);
    scale[2] = m[3][3] * std::pow(std::pow(m[0][2], 2) + std::pow(m[1][2], 2) + std::pow(m[2][2], 2), 0.5);
    // Remove scale
    for (auto j = 0; j < 3; j++) {
        if (auto s = scale[j]) {
            for (auto i = 0; i < 3; i++) {
                m[i][j] /= s;
            }
        }
    }
    m[3][3] = 1;

    if (
        dot_product(
            cross_product(
                { m[0][0], m[1][0], m[2][0] },
                { m[0][1], m[1][1], m[2][1] }),
            { m[0][2], m[1][2], m[2][2] })
        < 0) {
        scale[0] *= -1;
        m[0][0] *= -1;
        m[1][0] *= -1;
        m[2][0] *= -1;
    }

    std::tuple<double, double, double> rotation(
        std::atan2(m[2][1], m[2][2]),
        std::atan2(-m[2][0], std::pow(std::pow(m[2][1], 2) + std::pow(m[2][2], 2), 0.5)),
        std::atan2(m[1][0], m[0][0]));

    return std::make_tuple(
        std::make_tuple(scale[0], scale[1], scale[2]),
        rotation,
        displacement);
}

} // namespace Amulet
