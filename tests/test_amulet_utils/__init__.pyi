from __future__ import annotations

import faulthandler as _faulthandler

from . import (
    _test_amulet_utils,
    test_event_,
    test_lock_,
    test_lock_file_,
    test_logging_,
    test_matrix_,
    test_task_manager_,
    test_temp_,
    test_weak_,
)

__all__: list[str] = [
    "compiler_config",
    "test_event_",
    "test_lock_",
    "test_lock_file_",
    "test_logging_",
    "test_matrix_",
    "test_task_manager_",
    "test_temp_",
    "test_weak_",
]

def _init() -> None: ...

compiler_config: dict
