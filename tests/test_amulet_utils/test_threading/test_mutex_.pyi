from __future__ import annotations

import collections.abc

__all__: list[str] = ["get_cpp_tests"]

def get_cpp_tests() -> list[collections.abc.Callable[[], None]]: ...
