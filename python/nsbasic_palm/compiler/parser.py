"""Recursive-descent parser for BASIC statements."""

from typing import List, Optional

from .ast_nodes import (
    Assignment,
    BinaryExpression,
    Expression,
    Literal,
    PrintStatement,
    Program,
    Statement,
    VariableReference,
)
from .errors import CompilationError
from .lexer import Token, TokenType


class BasicParser:
    """Parses BASIC tokens into an abstract syntax tree."""

    def __init__(self, tokens: List[Token]):
        self.tokens = tokens
        self.current = 0

    def parse(self) -> Program:
        statements: List[Statement] = []
        while not self._check(TokenType.EOF):
            if self._match(TokenType.NEWLINE, TokenType.COLON):
                continue
            statements.append(self._parse_statement())
            self._consume_separators()
        return Program(statements)

    def _parse_statement(self) -> Statement:
        if self._match_keyword("PRINT"):
            return PrintStatement(self._parse_expression())
        if self._match_keyword("LET"):
            return self._parse_assignment()
        if self._check(TokenType.IDENTIFIER) and self._check_next(TokenType.OPERATOR, "="):
            return self._parse_assignment()
        token = self._peek()
        raise CompilationError("Expected statement", token.line, token.column)

    def _parse_assignment(self) -> Assignment:
        name_token = self._consume(TokenType.IDENTIFIER, "Expected variable name")
        self._consume_operator("=")
        value = self._parse_expression()
        return Assignment(name_token.value, value)

    def _parse_expression(self) -> Expression:
        expr = self._parse_term()
        while self._match_operator("+", "-"):
            operator = self._previous().value
            right = self._parse_term()
            expr = BinaryExpression(expr, operator, right)
        return expr

    def _parse_term(self) -> Expression:
        expr = self._parse_factor()
        while self._match_operator("*", "/"):
            operator = self._previous().value
            right = self._parse_factor()
            expr = BinaryExpression(expr, operator, right)
        return expr

    def _parse_factor(self) -> Expression:
        if self._match(TokenType.NUMBER):
            token = self._previous()
            value = float(token.value) if "." in token.value else int(token.value)
            return Literal(value)
        if self._match(TokenType.STRING):
            return Literal(self._previous().value)
        if self._match(TokenType.IDENTIFIER):
            return VariableReference(self._previous().value)
        if self._match(TokenType.LPAREN):
            expr = self._parse_expression()
            self._consume(TokenType.RPAREN, "Expected ')' after expression")
            return expr
        token = self._peek()
        raise CompilationError("Expected expression", token.line, token.column)

    def _consume_separators(self) -> None:
        while self._match(TokenType.NEWLINE, TokenType.COLON):
            continue

    def _match_operator(self, *operators: str) -> bool:
        if self._check(TokenType.OPERATOR) and self._peek().value in operators:
            self._advance()
            return True
        return False

    def _consume_operator(self, operator: str) -> None:
        if self._check(TokenType.OPERATOR) and self._peek().value == operator:
            self._advance()
            return
        token = self._peek()
        raise CompilationError(f"Expected '{operator}'", token.line, token.column)

    def _match_keyword(self, keyword: str) -> bool:
        if self._check(TokenType.KEYWORD) and self._peek().value.upper() == keyword:
            self._advance()
            return True
        return False

    def _consume(self, token_type: TokenType, message: str) -> Token:
        if self._check(token_type):
            return self._advance()
        token = self._peek()
        raise CompilationError(message, token.line, token.column)

    def _match(self, *token_types: TokenType) -> bool:
        for token_type in token_types:
            if self._check(token_type):
                self._advance()
                return True
        return False

    def _check(self, token_type: TokenType) -> bool:
        if self._is_at_end():
            return token_type == TokenType.EOF
        return self._peek().token_type == token_type

    def _check_next(self, token_type: TokenType, value: Optional[str] = None) -> bool:
        if self.current + 1 >= len(self.tokens):
            return False
        next_token = self.tokens[self.current + 1]
        if next_token.token_type != token_type:
            return False
        if value is not None:
            return next_token.value == value
        return True

    def _advance(self) -> Token:
        if not self._is_at_end():
            self.current += 1
        return self._previous()

    def _is_at_end(self) -> bool:
        return self._peek().token_type == TokenType.EOF

    def _peek(self) -> Token:
        return self.tokens[self.current]

    def _previous(self) -> Token:
        return self.tokens[self.current - 1]
