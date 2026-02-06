"""Abstract syntax tree nodes for the BASIC compiler."""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import List, Union


@dataclass
class Program:
    """Root node containing BASIC statements."""

    statements: List[Statement] = field(default_factory=list)


class Statement:
    """Base class for BASIC statements."""


class Expression:
    """Base class for BASIC expressions."""


@dataclass
class Assignment(Statement):
    """Assignment statement (LET x = expr or x = expr)."""

    name: str
    value: Expression


@dataclass
class PrintStatement(Statement):
    """PRINT statement."""

    value: Expression


@dataclass
class Literal(Expression):
    """Literal value (number/string)."""

    value: Union[int, float, str]


@dataclass
class VariableReference(Expression):
    """Reference to a variable by name."""

    name: str


@dataclass
class BinaryExpression(Expression):
    """Binary operator expression."""

    left: Expression
    operator: str
    right: Expression
