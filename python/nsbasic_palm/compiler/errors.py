"""Compiler error types for NS Basic/Palm."""

from dataclasses import dataclass


@dataclass
class CompilationError(Exception):
    """Represents a BASIC compilation error with location information."""

    message: str
    line: int
    column: int = 0

    def __str__(self) -> str:
        location = f"line {self.line}"
        if self.column:
            location += f", column {self.column}"
        return f"{location}: {self.message}"
