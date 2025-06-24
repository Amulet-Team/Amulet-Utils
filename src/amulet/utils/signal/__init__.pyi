from __future__ import annotations

import typing
from collections.abc import Callable
from typing import Protocol, TypeVarTuple, runtime_checkable

from amulet.utils.signal._connection_mode import ConnectionMode

from . import _connection_mode

__all__ = [
    "Args",
    "Callable",
    "ConnectionMode",
    "Protocol",
    "Signal",
    "SignalToken",
    "TypeVarTuple",
    "runtime_checkable",
]

class Signal(typing.Protocol):
    def __init__(self, *args, **kwargs: typing.Any): ...
    def connect(
        self, callback: typing.Callable[[*Args], None], mode: ConnectionMode = ...
    ) -> SignalToken[*Args,]:
        """

        Connect a callback to this signal and return a token.
        The token must be kept alive for the callback to work.
        The token is used to disconnect the callback when it is not needed.
        Thread safe.

        """

    def disconnect(self, token: SignalToken[*Args,]) -> None:
        """

        Disconnect a callback.
        Token is the value returned by connect.
        Thread safe.

        """

    def emit(self, *args: *Args) -> None:
        """

        Call all callbacks with the given arguments from this thread.
        Blocks until all callbacks are processed.
        Thread safe.

        """

class SignalToken(typing.Protocol):
    def __init__(self, *args, **kwargs: typing.Any): ...

Args: typing.TypeVarTuple  # value = Args
