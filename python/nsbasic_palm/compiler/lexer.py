"""Lexical analysis for NS Basic/Palm BASIC source code."""

from dataclasses import dataclass
from enum import Enum
from typing import List

from .errors import CompilationError


class TokenType(Enum):
    """Token types produced by the BASIC lexer."""

    KEYWORD = "keyword"
    IDENTIFIER = "identifier"
    NUMBER = "number"
    STRING = "string"
    OPERATOR = "operator"
    NEWLINE = "newline"
    EOF = "eof"
    LPAREN = "lparen"
    RPAREN = "rparen"
    COMMA = "comma"
    COLON = "colon"


@dataclass(frozen=True)
class Token:
    """Represents a single BASIC token."""

    token_type: TokenType
    value: str
    line: int
    column: int


class BasicLexer:
    """Converts BASIC source text into tokens."""

    KEYWORDS = {
        "LET",
        "PRINT",
        "REM",
    }

    def tokenize(self, source: str) -> List[Token]:
        tokens: List[Token] = []
        index = 0
        line = 1
        column = 1
        length = len(source)

        def add_token(token_type: TokenType, value: str, token_line: int, token_column: int) -> None:
            tokens.append(Token(token_type, value, token_line, token_column))

        def skip_line_comment() -> None:
            nonlocal index, column
            while index < length and source[index] != "\n":
                index += 1
                column += 1

        while index < length:
            char = source[index]

            if char in " \t\r":
                index += 1
                column += 1
                continue

            if char == "\n":
                add_token(TokenType.NEWLINE, "\n", line, column)
                index += 1
                line += 1
                column = 1
                continue

            if char == "'":
                skip_line_comment()
                continue

            if char == ":":
                add_token(TokenType.COLON, ":", line, column)
                index += 1
                column += 1
                continue

            if char == ",":
                add_token(TokenType.COMMA, ",", line, column)
                index += 1
                column += 1
                continue

            if char == "(":
                add_token(TokenType.LPAREN, "(", line, column)
                index += 1
                column += 1
                continue

            if char == ")":
                add_token(TokenType.RPAREN, ")", line, column)
                index += 1
                column += 1
                continue

            if char in "+-*/=":
                add_token(TokenType.OPERATOR, char, line, column)
                index += 1
                column += 1
                continue

            if char == '"':
                start_line = line
                start_column = column
                index += 1
                column += 1
                value_chars = []
                while index < length:
                    current = source[index]
                    if current == '"':
                        break
                    if current == "\\" and index + 1 < length:
                        next_char = source[index + 1]
                        if next_char in ['"', "\\"]:
                            value_chars.append(next_char)
                            index += 2
                            column += 2
                            continue
                    if current == "\n":
                        raise CompilationError("Unterminated string literal", start_line, start_column)
                    value_chars.append(current)
                    index += 1
                    column += 1
                if index >= length or source[index] != '"':
                    raise CompilationError("Unterminated string literal", start_line, start_column)
                add_token(TokenType.STRING, "".join(value_chars), start_line, start_column)
                index += 1
                column += 1
                continue

            if char.isdigit():
                start_line = line
                start_column = column
                number_chars = [char]
                index += 1
                column += 1
                has_decimal = False
                while index < length:
                    current = source[index]
                    if current == "." and not has_decimal:
                        if index + 1 >= length or not source[index + 1].isdigit():
                            raise CompilationError(
                                "Expected digit after decimal point",
                                start_line,
                                start_column,
                            )
                        has_decimal = True
                        number_chars.append(current)
                        index += 1
                        column += 1
                        continue
                    if not current.isdigit():
                        break
                    number_chars.append(current)
                    index += 1
                    column += 1
                add_token(TokenType.NUMBER, "".join(number_chars), start_line, start_column)
                continue

            if char.isalpha() or char == "_":
                start_line = line
                start_column = column
                identifier_chars = [char]
                index += 1
                column += 1
                while index < length:
                    current = source[index]
                    if not (current.isalnum() or current == "_"):
                        break
                    identifier_chars.append(current)
                    index += 1
                    column += 1
                identifier = "".join(identifier_chars)
                upper_identifier = identifier.upper()
                if upper_identifier == "REM":
                    skip_line_comment()
                    continue
                token_type = TokenType.KEYWORD if upper_identifier in self.KEYWORDS else TokenType.IDENTIFIER
                add_token(token_type, identifier, start_line, start_column)
                continue

            raise CompilationError(f"Unexpected character: {char}", line, column)

        add_token(TokenType.EOF, "", line, column)
        return tokens
