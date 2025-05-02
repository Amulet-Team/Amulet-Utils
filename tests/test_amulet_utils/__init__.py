if __name__ != "test_amulet_utils":
    raise RuntimeError(
        f"Module name is incorrect. Expected: 'test_amulet_utils' got '{__name__}'"
    )


import faulthandler

faulthandler.enable()


def _init() -> None:
    import sys
    import logging

    logging.basicConfig(level=logging.DEBUG, format="%(levelname)s - %(message)s")

    # Import dependencies
    import amulet.utils.logging

    # Enable debug logging when running tests.
    amulet.utils.logging.set_min_log_level(logging.DEBUG)

    # This needs to be an absolute path otherwise it may get called twice
    # on different module objects and crashes when the interpreter shuts down.
    from test_amulet_utils._test_amulet_utils import init

    init(sys.modules[__name__])


_init()
