import unittest

from test_amulet_utils.test_threading.test_mutex_ import get_cpp_tests


class MatrixTestCase(unittest.TestCase):
    def test_cpp(self) -> None:
        for test in get_cpp_tests():
            with self.subTest(test=test.__name__):
                test()


if __name__ == "__main__":
    unittest.main()
