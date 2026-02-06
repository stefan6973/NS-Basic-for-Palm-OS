import os

import pytest

os.environ.setdefault("QT_QPA_PLATFORM", "offscreen")

QtWidgets = pytest.importorskip("PySide6.QtWidgets", exc_type=ImportError)

QApplication = QtWidgets.QApplication
QWidget = QtWidgets.QWidget

from nsbasic_palm.gui import MainWindow


def _get_app() -> QApplication:
    app = QApplication.instance()
    if app is None:
        app = QApplication([])
    return app


def test_main_window_components() -> None:
    _get_app()
    window = MainWindow()

    assert "Form Designer" in window.windowTitle()
    assert window.findChild(QWidget, "form_designer_canvas") is not None
    assert window.findChild(QWidget, "widget_palette") is not None
    assert window.findChild(QWidget, "project_tree") is not None
    assert window.findChild(QWidget, "properties_panel") is not None

    window.close()
