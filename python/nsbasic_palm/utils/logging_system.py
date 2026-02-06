"""
NS Basic/Palm Logging System

Application-wide logging for the Palm OS IDE conversion.
Tracks compilation, form design, and runtime events.
"""

import logging
import sys
from pathlib import Path
from datetime import datetime
from typing import Optional


class NSBasicLogger:
    """Centralized logging for NS Basic/Palm application"""
    
    _instance: Optional['NSBasicLogger'] = None
    
    def __init__(self, log_directory: Optional[Path] = None):
        if NSBasicLogger._instance is not None:
            raise RuntimeError("NSBasicLogger is a singleton. Use get_logger() instead.")
        
        self.log_dir = log_directory or Path.home() / '.nsbasic' / 'logs'
        self.log_dir.mkdir(parents=True, exist_ok=True)
        
        self._setup_loggers()
        NSBasicLogger._instance = self
    
    def _setup_loggers(self):
        """Configure specialized loggers for different subsystems"""
        
        # Main application logger
        self.app_logger = self._create_logger(
            'nsbasic.app',
            self.log_dir / f'app_{datetime.now().strftime("%Y%m%d")}.log'
        )
        
        # Compilation logger (for BASIC compilation process)
        self.compiler_logger = self._create_logger(
            'nsbasic.compiler',
            self.log_dir / f'compiler_{datetime.now().strftime("%Y%m%d")}.log'
        )
        
        # Form designer logger (for visual form editing)
        self.designer_logger = self._create_logger(
            'nsbasic.designer',
            self.log_dir / f'designer_{datetime.now().strftime("%Y%m%d")}.log'
        )
        
        # Project logger (for project file operations)
        self.project_logger = self._create_logger(
            'nsbasic.project',
            self.log_dir / f'project_{datetime.now().strftime("%Y%m%d")}.log'
        )
    
    def _create_logger(self, name: str, log_file: Path) -> logging.Logger:
        """Create a configured logger instance"""
        logger = logging.getLogger(name)
        logger.setLevel(logging.DEBUG)
        
        # File handler with detailed formatting
        file_handler = logging.FileHandler(log_file)
        file_handler.setLevel(logging.DEBUG)
        file_formatter = logging.Formatter(
            '%(asctime)s | %(name)s | %(levelname)-8s | %(funcName)s:%(lineno)d | %(message)s',
            datefmt='%Y-%m-%d %H:%M:%S'
        )
        file_handler.setFormatter(file_formatter)
        
        # Console handler with simpler formatting
        console_handler = logging.StreamHandler(sys.stdout)
        console_handler.setLevel(logging.INFO)
        console_formatter = logging.Formatter(
            '%(levelname)s: %(message)s'
        )
        console_handler.setFormatter(console_formatter)
        
        logger.addHandler(file_handler)
        logger.addHandler(console_handler)
        
        return logger
    
    @classmethod
    def get_logger(cls, subsystem: str = 'app') -> logging.Logger:
        """Get logger for specific subsystem"""
        if cls._instance is None:
            cls._instance = NSBasicLogger()
        
        logger_map = {
            'app': cls._instance.app_logger,
            'compiler': cls._instance.compiler_logger,
            'designer': cls._instance.designer_logger,
            'project': cls._instance.project_logger,
        }
        
        return logger_map.get(subsystem, cls._instance.app_logger)
    
    @classmethod
    def log_compilation_start(cls, project_name: str, output_path: str):
        """Log start of BASIC compilation"""
        logger = cls.get_logger('compiler')
        logger.info(f"Starting compilation: {project_name} -> {output_path}")
    
    @classmethod
    def log_compilation_error(cls, line_number: int, error_message: str):
        """Log BASIC compilation error with line number"""
        logger = cls.get_logger('compiler')
        logger.error(f"Compilation error at line {line_number}: {error_message}")
    
    @classmethod
    def log_form_modification(cls, form_name: str, widget_count: int):
        """Log form designer modification"""
        logger = cls.get_logger('designer')
        logger.debug(f"Form '{form_name}' modified ({widget_count} widgets)")
    
    @classmethod
    def log_project_save(cls, filepath: str, form_count: int):
        """Log project file save operation"""
        logger = cls.get_logger('project')
        logger.info(f"Project saved: {filepath} ({form_count} forms)")
    
    @classmethod
    def log_resource_allocation(cls, resource_type: str, resource_id: int):
        """Log Palm OS resource ID allocation"""
        logger = cls.get_logger('project')
        logger.debug(f"Allocated {resource_type} resource ID: {resource_id}")


class PalmOSErrorReporter:
    """User-friendly error reporting for Palm OS specific issues"""
    
    ERROR_MESSAGES = {
        'invalid_creator_code': (
            "Invalid Creator Code",
            "Palm OS creator codes must be exactly 4 ASCII characters.\n"
            "Example: 'MYAP', 'TEST', 'CALC'\n"
            "Avoid using codes reserved by Palm Inc."
        ),
        'widget_out_of_bounds': (
            "Widget Outside Screen Boundaries",
            "One or more widgets extend beyond the Palm screen dimensions.\n"
            "Standard Palm OS screens are 160x160 pixels.\n"
            "Please adjust widget positions in the form designer."
        ),
        'compilation_failed': (
            "BASIC Compilation Failed",
            "The BASIC compiler encountered errors in your code.\n"
            "Check the compilation log for specific line numbers and error details."
        ),
        'resource_id_conflict': (
            "Resource ID Conflict",
            "Multiple resources are using the same ID number.\n"
            "Palm OS requires each form, widget, and bitmap to have a unique ID.\n"
            "Use the project inspector to review resource allocations."
        ),
        'project_file_corrupted': (
            "Project File Corrupted",
            "The .nsb project file appears to be damaged or in an invalid format.\n"
            "Try opening a backup copy if available."
        ),
    }
    
    @classmethod
    def report_error(cls, error_code: str, details: Optional[str] = None) -> str:
        """Generate user-friendly error message"""
        if error_code not in cls.ERROR_MESSAGES:
            return f"An error occurred: {error_code}"
        
        title, message = cls.ERROR_MESSAGES[error_code]
        
        full_message = f"{title}\n{'=' * len(title)}\n\n{message}"
        
        if details:
            full_message += f"\n\nAdditional Information:\n{details}"
        
        return full_message
    
    @classmethod
    def log_and_report(cls, error_code: str, details: Optional[str] = None, 
                      subsystem: str = 'app') -> str:
        """Log error and return user-friendly message"""
        message = cls.report_error(error_code, details)
        logger = NSBasicLogger.get_logger(subsystem)
        logger.error(f"{error_code}: {details if details else 'No additional details'}")
        return message


# Convenience function for application code
def get_nsbasic_logger(subsystem: str = 'app') -> logging.Logger:
    """Get NS Basic logger for specified subsystem"""
    return NSBasicLogger.get_logger(subsystem)
