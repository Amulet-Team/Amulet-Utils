import unittest
import gc
import weakref

import numpy
import numpy.testing

from PySide6.QtGui import QMatrix4x4

from amulet.utils.matrix import Matrix4x4

from test_amulet_utils.test_matrix_ import get_cpp_tests


class MatrixTestCase(unittest.TestCase):
    def test_cpp(self) -> None:
        for test in get_cpp_tests():
            with self.subTest(test=test.__name__):
                test()

    def test_construct(self) -> None:
        self.assertIsInstance(Matrix4x4(), Matrix4x4)

    def test_construct_tuple(self) -> None:
        m = Matrix4x4(((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16)))

        self.assertIsInstance(m, Matrix4x4)

        for i in range(4):
            for j in range(4):
                self.assertAlmostEqual(1.0 + i * 4 + j, m.get_element(i, j))

    def test_construct_tuple_err(self) -> None:
        with self.assertRaises(TypeError):
            Matrix4x4(())
        with self.assertRaises(TypeError):
            Matrix4x4(((), (), (), ()))
        with self.assertRaises(TypeError):
            Matrix4x4(((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15)))
        with self.assertRaises(RuntimeError):
            Matrix4x4(((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, "hi")))

    def test_construct_copy(self) -> None:
        m1 = Matrix4x4(((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16)))
        m2 = Matrix4x4(m1)

        self.assertIsInstance(m2, Matrix4x4)

        for i in range(4):
            for j in range(4):
                with self.subTest(i=i, j=j):
                    self.assertAlmostEqual(1.0 + i * 4 + j, m2.get_element(i, j))

    def test_construct_from_numpy(self) -> None:
        arr = numpy.array(
            ((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16)),
            dtype=numpy.float64,
        )

        m = Matrix4x4(arr)

        self.assertIsInstance(m, Matrix4x4)

        for i in range(4):
            for j in range(4):
                with self.subTest(i=i, j=j):
                    self.assertAlmostEqual(1.0 + i * 4 + j, m.get_element(i, j))

    def test_construct_from_numpy_slice(self) -> None:
        arr = numpy.arange(64, dtype=numpy.float64).reshape((8, 8))[::2, ::2]
        self.assertEqual(arr.shape, (4, 4))

        m = Matrix4x4(arr)

        self.assertIsInstance(m, Matrix4x4)

        for i in range(4):
            for j in range(4):
                with self.subTest(i=i, j=j):
                    self.assertAlmostEqual(float(i * 16 + j * 2), m.get_element(i, j))

    def test_construct_from_numpy_dtype(self) -> None:
        for dtype in (
            numpy.float32,
            numpy.float64,
            numpy.int8,
            numpy.int16,
            numpy.int32,
            numpy.int64,
            numpy.uint8,
            numpy.uint16,
            numpy.uint32,
            numpy.uint64,
        ):
            with self.subTest(dtype=dtype):
                m = Matrix4x4(numpy.arange(16, dtype=dtype).reshape((4, 4)))
                for i in range(4):
                    for j in range(4):
                        with self.subTest(i=i, j=j):
                            self.assertAlmostEqual(
                                float(i * 4 + j), m.get_element(i, j)
                            )

    def test_construct_from_numpy_err(self) -> None:
        with self.assertRaises(TypeError):
            Matrix4x4(numpy.arange(16, dtype=numpy.float64))
        with self.assertRaises(TypeError):
            Matrix4x4(numpy.arange(16, dtype=numpy.float64).reshape((4, 4, 1)))
        with self.assertRaises(TypeError):
            Matrix4x4(numpy.arange(8, dtype=numpy.float64).reshape((2, 4)))
        with self.assertRaises(TypeError):
            Matrix4x4(numpy.arange(8, dtype=numpy.float64).reshape((4, 2)))
        with self.assertRaises(TypeError):
            Matrix4x4(numpy.arange(16, dtype=numpy.complex64).reshape((4, 4)))

    def test_convert_to_numpy(self) -> None:
        m = Matrix4x4.identity_matrix().scale(1, 2, 3).translate(10, 20, 30)
        arr = numpy.asarray(m)

        self.assertIsInstance(arr, numpy.ndarray)
        self.assertEqual((4, 4), arr.shape)
        self.assertEqual(numpy.float64, arr.dtype)

        numpy.testing.assert_array_almost_equal(
            numpy.array(
                (
                    (1, 0, 0, 10),
                    (0, 2, 0, 20),
                    (0, 0, 3, 30),
                    (0, 0, 0, 1),
                )
            ),
            arr,
        )

        arr[0, 0] = 4.0
        self.assertAlmostEqual(4.0, m.get_element(0, 0))

    def test_numpy_keep_alive(self) -> None:
        m = Matrix4x4.identity_matrix()
        m_ref = weakref.ref(m)
        arr = numpy.asarray(m)

        del m
        gc.collect()

        self.assertIsNotNone(m_ref())

    def test_repr(self) -> None:
        m = Matrix4x4.identity_matrix()
        m_repr = repr(m)
        self.assertIsInstance(m_repr, str)

    def test_identity_matrix(self) -> None:
        m = Matrix4x4.identity_matrix()

        for i in range(4):
            for j in range(4):
                with self.subTest(i=i, j=j):
                    if i == j:
                        self.assertAlmostEqual(1.0, m.get_element(i, j))
                    else:
                        self.assertAlmostEqual(0.0, m.get_element(i, j))

    def test_scale_matrix(self) -> None:
        m = Matrix4x4.scale_matrix(1, 2, 3)

        for i in range(4):
            for j in range(4):
                with self.subTest(i=i, j=j):
                    if i == j:
                        if i == 3:
                            self.assertAlmostEqual(1.0, m.get_element(i, j))
                        else:
                            self.assertAlmostEqual(1.0 + i, m.get_element(i, j))
                    else:
                        self.assertAlmostEqual(0.0, m.get_element(i, j))

    def test_translation_matrix(self) -> None:
        m = Matrix4x4.translation_matrix(1, 2, 3)

        for i in range(4):
            for j in range(4):
                with self.subTest(i=i, j=j):
                    if i == j:
                        self.assertAlmostEqual(1.0, m.get_element(i, j))
                    elif j == 3:
                        self.assertAlmostEqual(1.0 + i, m.get_element(i, j))
                    else:
                        self.assertAlmostEqual(0.0, m.get_element(i, j))

    def test_rotation_matrx(self) -> None:
        self.assertIsInstance(Matrix4x4.rotation_x_matrix(1), Matrix4x4)
        self.assertIsInstance(Matrix4x4.rotation_y_matrix(1), Matrix4x4)
        self.assertIsInstance(Matrix4x4.rotation_z_matrix(1), Matrix4x4)

    def test_transformation_matrix(self) -> None:
        m = Matrix4x4.transformation_matrix(1, 2, 3, 0.1, 0.2, 0.3, 10, 20, 30)
        (sx, sy, sz), (rx, ry, rz), (dx, dy, dz) = m.decompose()
        self.assertAlmostEqual(sx, 1.0)
        self.assertAlmostEqual(sy, 2.0)
        self.assertAlmostEqual(sz, 3.0)
        self.assertAlmostEqual(rx, 0.1)
        self.assertAlmostEqual(ry, 0.2)
        self.assertAlmostEqual(rz, 0.3)
        self.assertAlmostEqual(dx, 10)
        self.assertAlmostEqual(dy, 20)
        self.assertAlmostEqual(dz, 30)

    def test_get_element(self) -> None:
        m = Matrix4x4(((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16)))

        for i in range(4):
            for j in range(4):
                self.assertAlmostEqual(1.0 + i * 4 + j, m.get_element(i, j))

        with self.assertRaises(RuntimeError):
            m.get_element(4, 0)
        with self.assertRaises(RuntimeError):
            m.get_element(0, 4)

    def test_set_element(self) -> None:
        m = Matrix4x4()

        for i in range(4):
            for j in range(4):
                m.set_element(i, j, 1.0 + i * 4 + j)

        for i in range(4):
            for j in range(4):
                self.assertAlmostEqual(1.0 + i * 4 + j, m.get_element(i, j))

        with self.assertRaises(RuntimeError):
            m.set_element(4, 0, 1.0)
        with self.assertRaises(RuntimeError):
            m.set_element(0, 4, 1.0)

    def test_mul_matrix(self) -> None:
        m1 = Matrix4x4.translation_matrix(1, 2, 3)
        m2 = Matrix4x4.scale_matrix(1, 2, 3)

        m3 = m1 * m2
        self.assertIsInstance(m3, Matrix4x4)

        numpy.testing.assert_array_almost_equal(
            numpy.matmul(numpy.asarray(m1), numpy.asarray(m2)), numpy.asarray(m3)
        )

    def test_mul_vector(self) -> None:
        m1 = Matrix4x4.translation_matrix(1, 2, 3)
        v1 = m1 * [(0, 0, 0), (10, 20, 30)]
        self.assertAlmostEqual(1.0, v1[0][0])
        self.assertAlmostEqual(2.0, v1[0][1])
        self.assertAlmostEqual(3.0, v1[0][2])
        self.assertAlmostEqual(11.0, v1[1][0])
        self.assertAlmostEqual(22.0, v1[1][1])
        self.assertAlmostEqual(33.0, v1[1][2])

        m2 = Matrix4x4.scale_matrix(1, 2, 3)
        v2 = m2 * [(0, 0, 0), (1, 1, 1)]
        self.assertAlmostEqual(0.0, v2[0][0])
        self.assertAlmostEqual(0.0, v2[0][1])
        self.assertAlmostEqual(0.0, v2[0][2])
        self.assertAlmostEqual(1.0, v2[1][0])
        self.assertAlmostEqual(2.0, v2[1][1])
        self.assertAlmostEqual(3.0, v2[1][2])

    def test_scale(self) -> None:
        m = Matrix4x4.identity_matrix().scale(1, 2, 3)

        for i in range(4):
            for j in range(4):
                with self.subTest(i=i, j=j):
                    if i == j:
                        if i == 3:
                            self.assertAlmostEqual(1.0, m.get_element(i, j))
                        else:
                            self.assertAlmostEqual(1.0 + i, m.get_element(i, j))
                    else:
                        self.assertAlmostEqual(0.0, m.get_element(i, j))

    def test_translate(self) -> None:
        m = Matrix4x4.identity_matrix().translate(1, 2, 3)

        for i in range(4):
            for j in range(4):
                with self.subTest(i=i, j=j):
                    if i == j:
                        self.assertAlmostEqual(1.0, m.get_element(i, j))
                    elif j == 3:
                        self.assertAlmostEqual(1.0 + i, m.get_element(i, j))
                    else:
                        self.assertAlmostEqual(0.0, m.get_element(i, j))

    def test_rotate(self) -> None:
        self.assertIsInstance(Matrix4x4.identity_matrix().rotate_x(1), Matrix4x4)
        self.assertIsInstance(Matrix4x4.identity_matrix().rotate_y(1), Matrix4x4)
        self.assertIsInstance(Matrix4x4.identity_matrix().rotate_z(1), Matrix4x4)

    def test_decompose(self) -> None:
        m = (
            Matrix4x4.identity_matrix()
            .scale(1, 2, 3)
            .rotate_x(0.1)
            .rotate_y(0.2)
            .rotate_z(0.3)
            .translate(10, 20, 30)
        )

        decomp = m.decompose()

        self.assertIsInstance(decomp, tuple)
        self.assertEqual(3, len(decomp))
        self.assertIsInstance(decomp[0], tuple)
        self.assertEqual(3, len(decomp[0]))
        self.assertIsInstance(decomp[1], tuple)
        self.assertEqual(3, len(decomp[1]))
        self.assertIsInstance(decomp[2], tuple)
        self.assertEqual(3, len(decomp[2]))

        self.assertAlmostEqual(1.0, decomp[0][0])
        self.assertAlmostEqual(2.0, decomp[0][1])
        self.assertAlmostEqual(3.0, decomp[0][2])
        self.assertAlmostEqual(0.1, decomp[1][0])
        self.assertAlmostEqual(0.2, decomp[1][1])
        self.assertAlmostEqual(0.3, decomp[1][2])
        self.assertAlmostEqual(10.0, decomp[2][0])
        self.assertAlmostEqual(20.0, decomp[2][1])
        self.assertAlmostEqual(30.0, decomp[2][2])

    def test_inverse(self) -> None:
        m1 = (
            Matrix4x4.identity_matrix()
            .scale(2, 3, 4)
            .rotate_x(0.1)
            .rotate_y(0.2)
            .rotate_z(0.3)
            .translate(10, 20, 30)
        )
        inv = m1.inverse()
        ident = inv * m1
        self.assertTrue(ident.almost_equal(Matrix4x4()))

        m2 = Matrix4x4.scale_matrix(0, 0, 0)
        with self.assertRaises(RuntimeError):
            m2.inverse()

    def test_almost_equal(self) -> None:
        m1 = Matrix4x4.translation_matrix(1, 2, 3)
        m2 = Matrix4x4.translation_matrix(1, 2, 3.00000001)
        self.assertTrue(m1.almost_equal(m2))

    def test_construct_from_qt(self) -> None:
        q = QMatrix4x4()
        q.translate(10, 20, 30)
        q.scale(1, 2, 3)

        m = Matrix4x4(q)

        self.assertIsInstance(m, Matrix4x4)

        for i in range(4):
            for j in range(4):
                with self.subTest(i=i, j=j):
                    if i == j:
                        if i == 3:
                            self.assertAlmostEqual(1.0, m.get_element(i, j))
                        else:
                            self.assertAlmostEqual(1.0 + i, m.get_element(i, j))
                    elif j == 3:
                        self.assertAlmostEqual(10.0 * (1.0 + i), m.get_element(i, j))
                    else:
                        self.assertAlmostEqual(0.0, m.get_element(i, j))

    def test_convert_to_qt(self) -> None:
        m = Matrix4x4.identity_matrix().scale(1, 2, 3).translate(10, 20, 30)

        for i in range(4):
            for j in range(4):
                with self.subTest(i=i, j=j):
                    if i == j:
                        if i == 3:
                            self.assertAlmostEqual(1.0, m.get_element(i, j))
                        else:
                            self.assertAlmostEqual(1.0 + i, m.get_element(i, j))
                    elif j == 3:
                        self.assertAlmostEqual(10.0 * (1.0 + i), m.get_element(i, j))
                    else:
                        self.assertAlmostEqual(0.0, m.get_element(i, j))

        q = m.to_qt()

        self.assertIsInstance(q, QMatrix4x4)

        for i in range(4):
            for j in range(4):
                with self.subTest(i=i, j=j):
                    if i == j:
                        if i == 3:
                            self.assertAlmostEqual(1.0, q[i, j])
                        else:
                            self.assertAlmostEqual(1.0 + i, q[i, j])
                    elif j == 3:
                        self.assertAlmostEqual(10.0 * (1.0 + i), q[i, j])
                    else:
                        self.assertAlmostEqual(0.0, q[i, j])


if __name__ == "__main__":
    unittest.main()
