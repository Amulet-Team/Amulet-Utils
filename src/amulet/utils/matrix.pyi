from __future__ import annotations

import collections.abc
import typing

import PySide6.QtGui

__all__ = ["Matrix4x4"]

class Matrix4x4:
    """
    A 4x4 transformation matrix.
    """

    @staticmethod
    def identity_matrix() -> Matrix4x4:
        """
        Construct a new identity matrix.
        """

    @staticmethod
    def rotation_x_matrix(angle: typing.SupportsFloat) -> Matrix4x4:
        """
        Construct a new rotation matrix in the x axis.
        """

    @staticmethod
    def rotation_y_matrix(angle: typing.SupportsFloat) -> Matrix4x4:
        """
        Construct a new rotation matrix in the y axis.
        """

    @staticmethod
    def rotation_z_matrix(angle: typing.SupportsFloat) -> Matrix4x4:
        """
        Construct a new rotation matrix in the z axis.
        """

    @staticmethod
    def scale_matrix(
        sx: typing.SupportsFloat, sy: typing.SupportsFloat, sz: typing.SupportsFloat
    ) -> Matrix4x4:
        """
        Construct a new scale matrix.
        """

    @staticmethod
    def translation_matrix(
        dx: typing.SupportsFloat, dy: typing.SupportsFloat, dz: typing.SupportsFloat
    ) -> Matrix4x4:
        """
        Construct a new translation matrix.
        """

    @typing.overload
    def __init__(self) -> None:
        """
        Construct with uninitialised memory.
        """

    @typing.overload
    def __init__(
        self,
        arg0: tuple[
            tuple[
                typing.SupportsFloat,
                typing.SupportsFloat,
                typing.SupportsFloat,
                typing.SupportsFloat,
            ],
            tuple[
                typing.SupportsFloat,
                typing.SupportsFloat,
                typing.SupportsFloat,
                typing.SupportsFloat,
            ],
            tuple[
                typing.SupportsFloat,
                typing.SupportsFloat,
                typing.SupportsFloat,
                typing.SupportsFloat,
            ],
            tuple[
                typing.SupportsFloat,
                typing.SupportsFloat,
                typing.SupportsFloat,
                typing.SupportsFloat,
            ],
        ],
    ) -> None:
        """
        Construct from tuples.
        """

    @typing.overload
    def __init__(self, other: Matrix4x4) -> None:
        """
        Copy from another matrix.
        """

    @typing.overload
    def __init__(self, arg0: collections.abc.Buffer) -> None: ...
    @typing.overload
    def __init__(self, arg0: PySide6.QtGui.QMatrix4x4) -> None: ...
    @typing.overload
    def __mul__(self, other: Matrix4x4) -> Matrix4x4:
        """
        Multiply this matrix with another matrix.
        """

    @typing.overload
    def __mul__(
        self,
        other: list[
            tuple[typing.SupportsFloat, typing.SupportsFloat, typing.SupportsFloat]
        ],
    ) -> list[tuple[float, float, float]]:
        """
        Multiply this matrix with a sequence of vectors.
        """

    def __repr__(self) -> str: ...
    def decompose(
        self,
    ) -> tuple[
        tuple[float, float, float],
        tuple[float, float, float],
        tuple[float, float, float],
    ]:
        """
        Decompose the matrix into scale, rotation and displacement tuples.
        """

    def get_element(self, i: typing.SupportsInt, j: typing.SupportsInt) -> float:
        """
        Get an element in the matrix.
        """

    def rotate_x(self, rx: typing.SupportsFloat) -> Matrix4x4:
        """
        Rotate this matrix by the specified amount in the x axis.
        """

    def rotate_y(self, ry: typing.SupportsFloat) -> Matrix4x4:
        """
        Rotate this matrix by the specified amount in the y axis.
        """

    def rotate_z(self, rz: typing.SupportsFloat) -> Matrix4x4:
        """
        Rotate this matrix by the specified amount in the z axis.
        """

    def scale(
        self,
        sx: typing.SupportsFloat,
        sy: typing.SupportsFloat,
        sz: typing.SupportsFloat,
    ) -> Matrix4x4:
        """
        Scale this matrix by the specified amount.
        """

    def set_element(
        self, i: typing.SupportsInt, j: typing.SupportsInt, value: typing.SupportsFloat
    ) -> None:
        """
        Set an element in the matrix.
        """

    def to_qt(self) -> PySide6.QtGui.QMatrix4x4: ...
    def translate(
        self,
        dx: typing.SupportsFloat,
        dy: typing.SupportsFloat,
        dz: typing.SupportsFloat,
    ) -> Matrix4x4:
        """
        Translate this matrix by the specified amount.
        """
