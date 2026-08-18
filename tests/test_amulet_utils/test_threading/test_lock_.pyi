from __future__ import annotations

import typing

import amulet.utils.lock

__all__: list[str] = ["lock_ordered_mutex", "lock_shared_mutex", "throw_deadlock"]

def lock_ordered_mutex(
    arg0: amulet.utils.lock.OrderedLock, arg1: typing.SupportsInt | typing.SupportsIndex
) -> None: ...
def lock_shared_mutex(
    arg0: amulet.utils.lock.SharedLock, arg1: typing.SupportsInt | typing.SupportsIndex
) -> None: ...
def throw_deadlock() -> None: ...
