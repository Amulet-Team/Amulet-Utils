import unittest

from test_amulet_utils.test_bytes_ import (
    bytes_passthrough,
    get_bytes,
    get_len,
    test_cpp,
)


class BytesTestCase(unittest.TestCase):
    def test_cpp(self) -> None:
        test_cpp()

    def test_get_bytes(self) -> None:
        b = get_bytes()
        self.assertIsInstance(b, bytes)
        self.assertEqual(b"Hello World", b)

    def test_bytes_passthrough(self) -> None:
        with self.assertRaises(TypeError):
            bytes_passthrough(None)  # type: ignore
        with self.assertRaises(TypeError):
            bytes_passthrough("")  # type: ignore
        b = bytes_passthrough(b"Test")
        self.assertIsInstance(b, bytes)
        self.assertEqual(b"Test", b)

    def test_len(self) -> None:
        l = get_len(b"Test")
        self.assertEqual(4, l)
