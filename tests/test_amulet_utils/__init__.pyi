from __future__ import annotations

import faulthandler as faulthandler

from . import (
    _test_amulet_utils,
    test_lock_,
    test_lock_file_,
    test_logging_,
    test_signal_,
    test_task_manager_,
    test_temp_,
    test_weak_,
)

__all__ = [
    "compiler_config",
    "faulthandler",
    "test_lock_",
    "test_lock_file_",
    "test_logging_",
    "test_signal_",
    "test_task_manager_",
    "test_temp_",
    "test_weak_",
]

def _init() -> None: ...

compiler_config: dict
