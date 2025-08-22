#include <pybind11/pybind11.h>
#include <pybind11/typing.h>

#include <numbers>
#include <vector>

#include <amulet/test_utils/test_utils.hpp>
#include <amulet/utils/matrix.hpp>

namespace py = pybind11;

static py::typing::List<py::typing::Callable<void()>> get_matrix_tests()
{
    py::list tests;

    tests.append(
        py::cpp_function(
            []() {
                Amulet::Matrix4x4 m;
                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        m.data[i][j] = i + j * 4;
                    }
                }
                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        ASSERT_ALMOST_EQUAL(double, m.data[i][j], i + j * 4);
                    }
                }
            },
            py::name("test_constructor_default")));

    tests.append(
        py::cpp_function(
            []() {
                double arr[4][4];
                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        arr[i][j] = i + j * 4;
                    }
                }

                Amulet::Matrix4x4 m(arr);

                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        ASSERT_ALMOST_EQUAL(double, m.data[i][j], i + j * 4);
                    }
                }
            },
            py::name("test_constructor_array")));

    tests.append(
        py::cpp_function(
            []() {
                Amulet::Matrix4x4 m1;
                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        m1.data[i][j] = i + j * 4;
                    }
                }

                Amulet::Matrix4x4 m2(m1);

                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        ASSERT_ALMOST_EQUAL(double, m2.data[i][j], i + j * 4);
                    }
                }
            },
            py::name("test_constructor_copy")));

    tests.append(
        py::cpp_function(
            []() {
                Amulet::Matrix4x4 m1;
                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        m1.data[i][j] = i + j * 4;
                    }
                }

                Amulet::Matrix4x4 m2;
                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        m2.data[i][j] = i * 4 + j;
                    }
                }

                Amulet::Matrix4x4 m3(m1);

                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        ASSERT_ALMOST_EQUAL(double, m3.data[i][j], i + j * 4);
                    }
                }

                m3 = m2;

                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        ASSERT_ALMOST_EQUAL(double, m3.data[i][j], i * 4 + j);
                    }
                }
            },
            py::name("test_assign_copy")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::scale_matrix(1, 2, 3);
                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        if (i == j) {
                            if (i == 3) {
                                ASSERT_ALMOST_EQUAL(double, m.get_element(i, j), 1);
                            } else {
                                ASSERT_ALMOST_EQUAL(double, m.get_element(i, j), i + 1);
                            }
                        } else {
                            ASSERT_ALMOST_EQUAL(double, m.get_element(i, j), 0);
                        }
                    }
                }
            },
            py::name("test_scale_matrix")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::translation_matrix(10, 20, 30);
                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        if (i == j) {
                            ASSERT_ALMOST_EQUAL(double, m.get_element(i, j), 1);
                        } else if (j == 3) {
                            ASSERT_ALMOST_EQUAL(double, m.get_element(i, j), (i + 1) * 10);
                        } else {
                            ASSERT_ALMOST_EQUAL(double, m.get_element(i, j), 0);
                        }
                    }
                }
            },
            py::name("test_displacement_matrix")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::identity_matrix();
                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        if (i == j) {
                            ASSERT_ALMOST_EQUAL(double, m.get_element(i, j), 1);
                        } else {
                            ASSERT_ALMOST_EQUAL(double, m.get_element(i, j), 0);
                        }
                    }
                }
            },
            py::name("test_identity_matrix")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::identity_matrix();
                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        if (i == j) {
                            ASSERT_ALMOST_EQUAL(double, m.get_element(i, j), 1);
                        } else {
                            ASSERT_ALMOST_EQUAL(double, m.get_element(i, j), 0);
                        }
                    }
                }
            },
            py::name("test_rotationidentity_matrix")));

    tests.append(
        py::cpp_function(
            []() {
                Amulet::Matrix4x4 m;
                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        m.data[i][j] = i + j * 4;
                    }
                }
                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        ASSERT_ALMOST_EQUAL(double, m.get_element(i, j), i + j * 4);
                    }
                }
                ASSERT_RAISES(std::runtime_error, m.get_element(4, 0));
                ASSERT_RAISES(std::runtime_error, m.get_element(0, 4));
            },
            py::name("test_accessor")));

    tests.append(
        py::cpp_function(
            []() {
                Amulet::Matrix4x4 m1 { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } } };
                Amulet::Matrix4x4 m2 { { { 17, 18, 19, 20 }, { 21, 22, 23, 24 }, { 25, 26, 27, 28 }, { 29, 30, 31, 32 } } };
                auto m3 = m1 * m2;

                ASSERT_ALMOST_EQUAL(double, m3.get_element(0, 0), 1 * 17 + 2 * 21 + 3 * 25 + 4 * 29);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(0, 1), 1 * 18 + 2 * 22 + 3 * 26 + 4 * 30);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(0, 2), 1 * 19 + 2 * 23 + 3 * 27 + 4 * 31);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(0, 3), 1 * 20 + 2 * 24 + 3 * 28 + 4 * 32);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(1, 0), 5 * 17 + 6 * 21 + 7 * 25 + 8 * 29);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(1, 1), 5 * 18 + 6 * 22 + 7 * 26 + 8 * 30);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(1, 2), 5 * 19 + 6 * 23 + 7 * 27 + 8 * 31);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(1, 3), 5 * 20 + 6 * 24 + 7 * 28 + 8 * 32);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(2, 0), 9 * 17 + 10 * 21 + 11 * 25 + 12 * 29);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(2, 1), 9 * 18 + 10 * 22 + 11 * 26 + 12 * 30);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(2, 2), 9 * 19 + 10 * 23 + 11 * 27 + 12 * 31);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(2, 3), 9 * 20 + 10 * 24 + 11 * 28 + 12 * 32);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(3, 0), 13 * 17 + 14 * 21 + 15 * 25 + 16 * 29);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(3, 1), 13 * 18 + 14 * 22 + 15 * 26 + 16 * 30);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(3, 2), 13 * 19 + 14 * 23 + 15 * 27 + 16 * 31);
                ASSERT_ALMOST_EQUAL(double, m3.get_element(3, 3), 13 * 20 + 14 * 24 + 15 * 28 + 16 * 32);
            },
            py::name("test_multiply_matrix")));

    tests.append(
        py::cpp_function(
            []() {
                std::vector<std::array<double, 3>> vectors(
                    { { 1, 1, 1 },
                        { 1, 2, 3 },
                        { 1, 0, 0 },
                        { 0, 1, 0 },
                        { 0, 0, 1 },
                        { -1, 0, 0 },
                        { 0, -1, 0 },
                        { 0, 0, -1 } });

                auto m1 = Amulet::Matrix4x4::scale_matrix(10, 20, 30);

                auto transformed_vectors = m1 * vectors;

                std::vector<std::array<double, 3>> expected_vectors(
                    { { 10, 20, 30 },
                        { 10, 40, 90 },
                        { 10, 0, 0 },
                        { 0, 20, 0 },
                        { 0, 0, 30 },
                        { -10, 0, 0 },
                        { 0, -20, 0 },
                        { 0, 0, -30 } });

                for (auto v = 0; v < vectors.size(); v++) {
                    for (auto i = 0; i < 3; i++) {
                        ASSERT_ALMOST_EQUAL(double, expected_vectors[v][i], transformed_vectors[v][i]);
                    }
                }
            },
            py::name("test_multiply_vectors_scale")));

    tests.append(
        py::cpp_function(
            []() {
                std::vector<std::array<double, 3>> vectors(
                    { { 1, 1, 1 },
                        { 1, 2, 3 },
                        { 1, 0, 0 },
                        { 0, 1, 0 },
                        { 0, 0, 1 },
                        { -1, 0, 0 },
                        { 0, -1, 0 },
                        { 0, 0, -1 } });

                auto m1 = Amulet::Matrix4x4::translation_matrix(10, 20, 30);

                auto transformed_vectors = m1 * vectors;

                std::vector<std::array<double, 3>> expected_vectors(
                    { { 11, 21, 31 },
                        { 11, 22, 33 },
                        { 11, 20, 30 },
                        { 10, 21, 30 },
                        { 10, 20, 31 },
                        { 9, 20, 30 },
                        { 10, 19, 30 },
                        { 10, 20, 29 } });

                for (auto v = 0; v < vectors.size(); v++) {
                    for (auto i = 0; i < 3; i++) {
                        ASSERT_ALMOST_EQUAL(double, expected_vectors[v][i], transformed_vectors[v][i]);
                    }
                }
            },
            py::name("test_multiply_vectors_translate")));

    tests.append(
        py::cpp_function(
            []() {
                std::vector<std::array<double, 3>> vectors(
                    { { 1, 0, 0 },
                        { 0, 1, 0 },
                        { 0, 0, 1 },
                        { -1, 0, 0 },
                        { 0, -1, 0 },
                        { 0, 0, -1 } });

                auto m1 = Amulet::Matrix4x4::rotation_x_matrix(std::numbers::pi / 2);

                auto transformed_vectors = m1 * vectors;

                std::vector<std::array<double, 3>> expected_vectors(
                    { { 1, 0, 0 },
                        { 0, 0, 1 },
                        { 0, -1, 0 },
                        { -1, 0, 0 },
                        { 0, 0, -1 },
                        { 0, 1, 0 } });

                for (auto v = 0; v < vectors.size(); v++) {
                    for (auto i = 0; i < 3; i++) {
                        ASSERT_ALMOST_EQUAL(double, expected_vectors[v][i], transformed_vectors[v][i]);
                    }
                }
            },
            py::name("test_multiply_vectors_rotate_x")));

    tests.append(
        py::cpp_function(
            []() {
                std::vector<std::array<double, 3>> vectors(
                    { { 1, 0, 0 },
                        { 0, 1, 0 },
                        { 0, 0, 1 },
                        { -1, 0, 0 },
                        { 0, -1, 0 },
                        { 0, 0, -1 } });

                auto m1 = Amulet::Matrix4x4::rotation_y_matrix(std::numbers::pi / 2);

                auto transformed_vectors = m1 * vectors;

                std::vector<std::array<double, 3>> expected_vectors(
                    { { 0, 0, -1 },
                        { 0, 1, 0 },
                        { 1, 0, 0 },
                        { 0, 0, 1 },
                        { 0, -1, 0 },
                        { -1, 0, 0 } });

                for (auto v = 0; v < vectors.size(); v++) {
                    for (auto i = 0; i < 3; i++) {
                        ASSERT_ALMOST_EQUAL(double, expected_vectors[v][i], transformed_vectors[v][i]);
                    }
                }
            },
            py::name("test_multiply_vectors_rotate_y")));

    tests.append(
        py::cpp_function(
            []() {
                std::vector<std::array<double, 3>> vectors(
                    { { 1, 0, 0 },
                        { 0, 1, 0 },
                        { 0, 0, 1 },
                        { -1, 0, 0 },
                        { 0, -1, 0 },
                        { 0, 0, -1 } });

                auto m1 = Amulet::Matrix4x4::rotation_z_matrix(std::numbers::pi / 2);

                auto transformed_vectors = m1 * vectors;

                std::vector<std::array<double, 3>> expected_vectors(
                    { { 0, 1, 0 },
                        { -1, 0, 0 },
                        { 0, 0, 1 },
                        { 0, -1, 0 },
                        { 1, 0, 0 },
                        { 0, 0, -1 } });

                for (auto v = 0; v < vectors.size(); v++) {
                    for (auto i = 0; i < 3; i++) {
                        ASSERT_ALMOST_EQUAL(double, expected_vectors[v][i], transformed_vectors[v][i]);
                    }
                }
            },
            py::name("test_multiply_vectors_rotate_z")));

    tests.append(
        py::cpp_function(
            []() {
                std::vector<std::array<double, 3>> vectors(
                    { { 1, 0, 0 },
                        { 0, 1, 0 },
                        { 0, 0, 1 },
                        { -1, 0, 0 },
                        { 0, -1, 0 },
                        { 0, 0, -1 } });

                auto m = Amulet::Matrix4x4::transformation_matrix(1, 2, 3, std::numbers::pi / 2, 0, 0, 10, 20, 30);

                auto transformed_vectors = m * vectors;

                std::vector<std::array<double, 3>> expected_vectors(
                    { { 11, 20, 30 },
                        { 10, 20, 32 },
                        { 10, 17, 30 },
                        { 9, 20, 30 },
                        { 10, 20, 28 },
                        { 10, 23, 30 } });

                for (auto v = 0; v < vectors.size(); v++) {
                    for (auto i = 0; i < 3; i++) {
                        ASSERT_ALMOST_EQUAL(double, expected_vectors[v][i], transformed_vectors[v][i]);
                    }
                }
            },
            py::name("test_transformation_matrix")));

    tests.append(
        py::cpp_function(
            []() {
                auto m1 = Amulet::Matrix4x4::identity_matrix();

                auto m2 = m1.translate(10, 20, 30);

                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        if (i == j) {
                            ASSERT_ALMOST_EQUAL(double, m2.get_element(i, j), 1);
                        } else if (j == 3) {
                            ASSERT_ALMOST_EQUAL(double, m2.get_element(i, j), (i + 1) * 10);
                        } else {
                            ASSERT_ALMOST_EQUAL(double, m2.get_element(i, j), 0);
                        }
                    }
                }
            },
            py::name("test_translate")));

    tests.append(
        py::cpp_function(
            []() {
                auto m1 = Amulet::Matrix4x4::identity_matrix();

                auto m2 = m1.scale(10, 20, 30);

                for (auto i = 0; i < 4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        if (i == j) {
                            if (i == 3) {
                                ASSERT_ALMOST_EQUAL(double, m2.get_element(i, j), 1);
                            } else {
                                ASSERT_ALMOST_EQUAL(double, m2.get_element(i, j), (i + 1) * 10);
                            }
                        } else {
                            ASSERT_ALMOST_EQUAL(double, m2.get_element(i, j), 0);
                        }
                    }
                }
            },
            py::name("test_scale")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::identity_matrix();
                auto [scale, rotation, displacement] = m.decompose();

                ASSERT_ALMOST_EQUAL(double, std::get<0>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(displacement), 0);
            },
            py::name("test_decompose_identity")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::scale_matrix(10, 20, 30);
                auto [scale, rotation, displacement] = m.decompose();

                ASSERT_ALMOST_EQUAL(double, std::get<0>(scale), 10);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(scale), 20);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(scale), 30);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(displacement), 0);
            },
            py::name("test_decompose_scale")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::translation_matrix(10, 20, 30);
                auto [scale, rotation, displacement] = m.decompose();

                ASSERT_ALMOST_EQUAL(double, std::get<0>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(displacement), 10);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(displacement), 20);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(displacement), 30);
            },
            py::name("test_decompose_displacement")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::rotation_x_matrix(1);
                auto [scale, rotation, displacement] = m.decompose();

                ASSERT_ALMOST_EQUAL(double, std::get<0>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(rotation), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(displacement), 0);
            },
            py::name("test_decompose_rotation_x")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::rotation_y_matrix(1);
                auto [scale, rotation, displacement] = m.decompose();

                ASSERT_ALMOST_EQUAL(double, std::get<0>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(rotation), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(displacement), 0);
            },
            py::name("test_decompose_rotation_y")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::rotation_z_matrix(1);
                auto [scale, rotation, displacement] = m.decompose();

                ASSERT_ALMOST_EQUAL(double, std::get<0>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(rotation), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(displacement), 0);
            },
            py::name("test_decompose_rotation_z")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::identity_matrix().rotate_x(1);
                auto [scale, rotation, displacement] = m.decompose();

                ASSERT_ALMOST_EQUAL(double, std::get<0>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(rotation), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(displacement), 0);
            },
            py::name("test_decompose_rotation_x2")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::identity_matrix().rotate_y(1);
                auto [scale, rotation, displacement] = m.decompose();

                ASSERT_ALMOST_EQUAL(double, std::get<0>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(rotation), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(displacement), 0);
            },
            py::name("test_decompose_rotation_y2")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::identity_matrix().rotate_z(1);
                auto [scale, rotation, displacement] = m.decompose();

                ASSERT_ALMOST_EQUAL(double, std::get<0>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(rotation), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(rotation), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(displacement), 0);
            },
            py::name("test_decompose_rotation_z2")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::identity_matrix().rotate_x(0.1).rotate_y(0.2).rotate_z(0.3);
                auto [scale, rotation, displacement] = m.decompose();

                ASSERT_ALMOST_EQUAL(double, std::get<0>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(scale), 1);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(rotation), 0.1);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(rotation), 0.2);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(rotation), 0.3);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(displacement), 0);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(displacement), 0);
            },
            py::name("test_decompose_rotation")));

    tests.append(
        py::cpp_function(
            []() {
                auto m = Amulet::Matrix4x4::identity_matrix().scale(2, 3, 4).rotate_x(0.1).rotate_y(0.2).rotate_z(0.3).translate(10, 20, 30);
                auto [scale, rotation, displacement] = m.decompose();

                ASSERT_ALMOST_EQUAL(double, std::get<0>(scale), 2);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(scale), 3);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(scale), 4);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(rotation), 0.1);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(rotation), 0.2);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(rotation), 0.3);
                ASSERT_ALMOST_EQUAL(double, std::get<0>(displacement), 10);
                ASSERT_ALMOST_EQUAL(double, std::get<1>(displacement), 20);
                ASSERT_ALMOST_EQUAL(double, std::get<2>(displacement), 30);
            },
            py::name("test_decompose_full")));

    return tests;
}

void init_test_matrix(py::module m_parent)
{
    auto m = m_parent.def_submodule("test_matrix_");
    m.def("get_cpp_tests", &get_matrix_tests);
}
