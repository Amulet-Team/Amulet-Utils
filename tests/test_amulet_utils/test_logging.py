from unittest import TestCase


class LoggingTestCase(TestCase):
    def test_cpp(self) -> None:
        from amulet.utils.logging import get_logger
        from test_amulet_utils.test_logging_ import logging_tests

        messages = []

        def on_msg(level: int, msg: str):
            messages.append((level, msg))

        token = get_logger().connect(on_msg)

        logging_tests()

        self.assertEqual(
            [
                (10, "debug msg"),
                (20, "info msg"),
                (30, "warning msg"),
                (40, "error msg"),
                (50, "critical msg"),
                *[(i, str(i)) for i in range(10, 60, 5)],
            ],
            messages,
        )

        get_logger().disconnect(token)

    def test_py(self) -> None:
        from amulet.utils.logging import get_min_log_level, set_min_log_level

        # this is set to 0 for tests
        self.assertEqual(10, get_min_log_level())
        set_min_log_level(0)
        self.assertEqual(0, get_min_log_level())
        set_min_log_level(10)
        self.assertEqual(10, get_min_log_level())
