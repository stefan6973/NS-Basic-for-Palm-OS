#!/usr/bin/env python3
"""
NS Basic/Palm OS IDE Launcher

Entry point for the NS Basic/Palm development environment.
This file is used by PyInstaller to create the executable.
"""

import sys
from pathlib import Path

from PySide6.QtWidgets import QApplication

# Add the package to Python path
sys.path.insert(0, str(Path(__file__).parent))

from nsbasic_palm.gui import MainWindow
from nsbasic_palm.utils.logging_system import get_nsbasic_logger


def main():
    """Main entry point for NS Basic/Palm IDE"""
    logger = get_nsbasic_logger('app')
    logger.info("NS Basic/Palm OS IDE starting...")

    app = QApplication(sys.argv)
    app.setApplicationName("NS Basic/Palm OS")

    window = MainWindow()
    window.show()

    logger.info("GUI launched successfully")
    return app.exec()


if __name__ == '__main__':
    sys.exit(main())
