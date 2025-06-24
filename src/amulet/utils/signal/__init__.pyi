from __future__ import annotations

import collections.abc
import typing

from amulet.utils.signal._connection_mode import ConnectionMode

from . import _connection_mode

__all__ = ["ConnectionMode", "Signal", "SignalToken"]

class Signal(typing.Protocol):
    def __init__(self, *args, **kwargs: typing.Any): ...
    def connect(
        self,
        callback: collections.abc.Callable[[*_Args], None],
        mode: ConnectionMode = ...,
    ) -> SignalToken[*_Args,]:
        """

        Connect a callback to this signal and return a token.
        The token must be kept alive for the callback to work.
        The token is used to disconnect the callback when it is not needed.
        Thread safe.

        """

    def disconnect(self, token: SignalToken[*_Args,]) -> None:
        """

        Disconnect a callback.
        Token is the value returned by connect.
        Thread safe.

        """

    def emit(self, *args: *_Args) -> None:
        """

        Call all callbacks with the given arguments from this thread.
        Blocks until all callbacks are processed.
        Thread safe.

        """

class SignalToken(typing.Protocol):
    def __init__(self, *args, **kwargs: typing.Any): ...

_Args: typing.TypeVarTuple  # value = _Args
