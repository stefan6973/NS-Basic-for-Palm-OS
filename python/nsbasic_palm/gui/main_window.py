"""
NS Basic/Palm OS - Phase 3 GUI Scaffolding

Initial PySide6-based GUI layout for the form designer workflow.
"""

from typing import Optional

from PySide6.QtCore import Qt, QSize
from PySide6.QtGui import QAction
from PySide6.QtWidgets import (
    QDockWidget,
    QFrame,
    QLabel,
    QListWidget,
    QListWidgetItem,
    QMainWindow,
    QStatusBar,
    QToolBar,
    QTreeWidget,
    QTreeWidgetItem,
    QVBoxLayout,
    QWidget,
)

from nsbasic_palm.utils.logging_system import get_nsbasic_logger


class FormDesignerCanvas(QFrame):
    """Placeholder canvas for the Palm OS form designer."""

    def __init__(self, parent: Optional[QWidget] = None) -> None:
        super().__init__(parent)
        self.setObjectName("form_designer_canvas")
        self.setFrameStyle(QFrame.Panel | QFrame.Sunken)
        self.setFixedSize(160, 160)
        self.setStyleSheet("background-color: #f7f7f7;")
        self.setToolTip("Palm OS Form Designer (160x160)")


class WidgetPalette(QListWidget):
    """Palette listing Palm OS widgets for placement."""

    def __init__(self, parent: Optional[QWidget] = None) -> None:
        super().__init__(parent)
        self.setObjectName("widget_palette")
        for label in ["Button", "Field", "List", "Checkbox", "Label"]:
            QListWidgetItem(label, self)


class ProjectTree(QTreeWidget):
    """Project tree view placeholder."""

    def __init__(self, parent: Optional[QWidget] = None) -> None:
        super().__init__(parent)
        self.setObjectName("project_tree")
        self.setHeaderLabel("Project")
        root = QTreeWidgetItem(["Sample Project"])
        QTreeWidgetItem(root, ["Form1"])
        self.addTopLevelItem(root)
        self.expandAll()


class PropertiesPanel(QWidget):
    """Properties panel placeholder."""

    def __init__(self, parent: Optional[QWidget] = None) -> None:
        super().__init__(parent)
        self.setObjectName("properties_panel")
        layout = QVBoxLayout(self)
        layout.addWidget(QLabel("Properties panel (Phase 3)"))
        layout.addStretch()


class MainWindow(QMainWindow):
    """Main IDE window for Phase 3 GUI work."""

    def __init__(self) -> None:
        super().__init__()
        self.logger = get_nsbasic_logger("designer")
        self.logger.info("Initializing Phase 3 GUI scaffolding")
        self.setWindowTitle("NS Basic/Palm OS - Form Designer (Phase 3)")
        self.resize(1100, 720)

        self._create_actions()
        self._create_menus()
        self._create_toolbar()
        self._create_status_bar()
        self._create_docks()
        self._create_central_widget()

    def _create_actions(self) -> None:
        self.action_new = QAction("New Project", self)
        self.action_open = QAction("Open Project...", self)
        self.action_save = QAction("Save Project", self)
        self.action_exit = QAction("Exit", self)
        self.action_exit.triggered.connect(self.close)

    def _create_menus(self) -> None:
        file_menu = self.menuBar().addMenu("&File")
        file_menu.addAction(self.action_new)
        file_menu.addAction(self.action_open)
        file_menu.addAction(self.action_save)
        file_menu.addSeparator()
        file_menu.addAction(self.action_exit)

    def _create_toolbar(self) -> None:
        toolbar = QToolBar("Main Toolbar", self)
        toolbar.setIconSize(QSize(16, 16))
        toolbar.addAction(self.action_new)
        toolbar.addAction(self.action_open)
        toolbar.addAction(self.action_save)
        self.addToolBar(toolbar)

    def _create_status_bar(self) -> None:
        status_bar = QStatusBar(self)
        status_bar.showMessage("Ready")
        self.setStatusBar(status_bar)

    def _create_docks(self) -> None:
        project_dock = QDockWidget("Project", self)
        project_dock.setObjectName("project_dock")
        project_dock.setWidget(ProjectTree(self))
        project_dock.setAllowedAreas(Qt.LeftDockWidgetArea | Qt.RightDockWidgetArea)
        self.addDockWidget(Qt.LeftDockWidgetArea, project_dock)

        palette_dock = QDockWidget("Widgets", self)
        palette_dock.setObjectName("palette_dock")
        palette_dock.setWidget(WidgetPalette(self))
        palette_dock.setAllowedAreas(Qt.LeftDockWidgetArea | Qt.RightDockWidgetArea)
        self.addDockWidget(Qt.LeftDockWidgetArea, palette_dock)
        self.tabifyDockWidget(project_dock, palette_dock)

        properties_dock = QDockWidget("Properties", self)
        properties_dock.setObjectName("properties_dock")
        properties_dock.setWidget(PropertiesPanel(self))
        properties_dock.setAllowedAreas(Qt.LeftDockWidgetArea | Qt.RightDockWidgetArea)
        self.addDockWidget(Qt.RightDockWidgetArea, properties_dock)

    def _create_central_widget(self) -> None:
        container = QWidget(self)
        container.setObjectName("form_designer_container")
        layout = QVBoxLayout(container)
        layout.addWidget(QLabel("Form Designer"))
        layout.addWidget(FormDesignerCanvas(container), alignment=Qt.AlignCenter)
        layout.addStretch()
        self.setCentralWidget(container)
