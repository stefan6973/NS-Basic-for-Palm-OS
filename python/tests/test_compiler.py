"""Unit tests for BASIC compiler scaffolding."""

import json
from pathlib import Path

from nsbasic_palm.compiler import (
    Assignment,
    BasicCompiler,
    BasicLexer,
    BasicParser,
    TokenType,
)
from nsbasic_palm.models import PalmProject


def test_lexer_tokens() -> None:
    lexer = BasicLexer()
    tokens = lexer.tokenize('LET X = 42\nPRINT "Hi"')

    assert tokens[0].token_type == TokenType.KEYWORD
    assert tokens[0].value.upper() == "LET"
    assert tokens[1].token_type == TokenType.IDENTIFIER
    assert tokens[1].value == "X"
    assert tokens[3].token_type == TokenType.NUMBER
    assert tokens[3].value == "42"


def test_parser_assignment_and_print() -> None:
    lexer = BasicLexer()
    tokens = lexer.tokenize('LET total = 3 + 4\nPRINT total')
    parser = BasicParser(tokens)
    program = parser.parse()

    assert len(program.statements) == 2
    assert isinstance(program.statements[0], Assignment)
    assert program.statements[0].name == "total"


def test_compiler_bytecode_output() -> None:
    compiler = BasicCompiler()
    program = compiler.compile_script('LET count = 1\nPRINT count')

    assert program.instructions[0].opcode == "LOAD_CONST"
    assert program.instructions[-1].opcode == "PRINT"

    payload = json.loads(program.to_bytes().decode("utf-8"))
    assert payload["constants"] == [1]
    assert payload["instructions"][0]["opcode"] == "LOAD_CONST"


def test_project_compile_to_prc(tmp_path: Path) -> None:
    project = PalmProject()
    project.initialization_script = 'PRINT "Ready"'

    output_path = tmp_path / "sample.prc"
    assert project.compile_to_prc(str(output_path)) is True
    assert output_path.read_bytes() == project.compiled_bytecode
