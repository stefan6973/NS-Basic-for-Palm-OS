"""High-level compiler entry points for BASIC scripts."""

from typing import List

from nsbasic_palm.utils.logging_system import get_nsbasic_logger

from .codegen import BasicCodeGenerator, BytecodeProgram
from .errors import CompilationError
from .lexer import BasicLexer
from .parser import BasicParser


class BasicCompiler:
    """Compiles BASIC source into bytecode."""

    def __init__(self) -> None:
        self.logger = get_nsbasic_logger("compiler")

    def compile_script(self, script: str) -> BytecodeProgram:
        self.logger.debug("Starting BASIC compilation")
        lexer = BasicLexer()
        tokens = lexer.tokenize(script)
        parser = BasicParser(tokens)
        program = parser.parse()
        generator = BasicCodeGenerator()
        bytecode = generator.generate(program)
        self.logger.debug("Compilation completed")
        return bytecode

    def compile_scripts(self, scripts: List[str]) -> BytecodeProgram:
        combined_script = "\n".join(scripts)
        return self.compile_script(combined_script)

    def compile_to_bytes(self, scripts: List[str]) -> bytes:
        program = self.compile_scripts(scripts)
        return program.to_bytes()
