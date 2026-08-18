import logging as _logging

from . import _version

__version__ = _version.get_versions()["version"]

# init a default logger
_logging.basicConfig(level=_logging.INFO, format="%(levelname)s - %(message)s")


def _init() -> None:
    import os
    import sys
    import platformdirs

    if os.environ.get("AMULET_SKIP_COMPILE", None):
        return

    try:
        os.add_dll_directory(__path__[0])
    except AttributeError:
        pass

    from ._amulet_utils import init

    init(sys.modules[__name__])

    from .temp import set_temp_dir

    cache_dir = os.environ.get("CACHE_DIR")
    if cache_dir is None:
        cache_dir = platformdirs.user_cache_dir("AmuletTeam", "AmuletTeam")
    os.makedirs(cache_dir, exist_ok=True)
    set_temp_dir(cache_dir)


_init()
del _init
