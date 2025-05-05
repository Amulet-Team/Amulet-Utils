from unittest import TestCase


class TempDirTestCase(TestCase):
    def test_temp_dir(self) -> None:
        from test_amulet_utils.test_temp_ import test_temp

        test_temp()
