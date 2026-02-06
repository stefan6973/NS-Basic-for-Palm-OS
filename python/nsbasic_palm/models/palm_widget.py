"""
Palm OS UI Widget Base Classes

Base classes for all Palm OS user interface controls (buttons, fields, lists, etc.)
Each widget type inherits from PalmWidgetBase and adds specific properties.

Converted from VB6 CUIObject.cls and subclasses (CUIButton, CUIField, etc.)
"""

from dataclasses import dataclass
from typing import Optional
from enum import Enum


class PalmFontID(Enum):
    """Standard Palm OS font identifiers"""
    STANDARD = 0
    BOLD = 1
    LARGE = 2
    SYMBOL = 3
    SYMBOL_11 = 4
    SYMBOL_7 = 5
    LED = 6
    LARGE_BOLD = 7


class PalmWidgetType(Enum):
    """Types of Palm OS UI widgets"""
    BUTTON = "button"
    PUSH_BUTTON = "pushbutton"
    FIELD = "field"
    LABEL = "label"
    LIST = "list"
    POPUP = "popup"
    CHECKBOX = "checkbox"
    SELECTOR = "selector"
    REPEATER = "repeater"
    SCROLLBAR = "scrollbar"
    SLIDER = "slider"
    TABLE = "table"
    GRID = "grid"
    GADGET = "gadget"
    BITMAP = "bitmap"


@dataclass
class PalmWidgetBase:
    """
    Base class for all Palm OS UI widgets.
    Equivalent to VB6 CUIObject.
    """
    
    # Identification
    widget_name: str = "widget"
    widget_type: PalmWidgetType = PalmWidgetType.BUTTON
    resource_id: int = 0
    
    # Position and size (in pixels)
    x_position: int = 0
    y_position: int = 0
    width: int = 40
    height: int = 12
    
    # Visual properties
    visible: bool = True
    usable: bool = True  # Palm OS specific - can widget be interacted with
    font_id: PalmFontID = PalmFontID.STANDARD
    
    # Text content
    label_text: str = ""
    
    # Behavior
    event_handler_script: str = ""  # BASIC code executed on interaction
    
    # Palm OS navigation (5-way navigator support)
    navigation_flags: int = 0
    nav_focus_id_above: int = 0
    nav_focus_id_below: int = 0
    nav_focus_id_left: int = 0
    nav_focus_id_right: int = 0
    
    # Parent relationship
    parent_form: Optional = None
    
    # IDE state (not compiled into app)
    is_selected_in_designer: bool = False
    
    # Compilation
    compiled_resource_data: bytes = b""
    
    def get_bounding_box(self) -> tuple:
        """Return (x, y, width, height) tuple"""
        return (self.x_position, self.y_position, self.width, self.height)
    
    def set_position(self, x: int, y: int) -> None:
        """Move widget to new position"""
        self.x_position = x
        self.y_position = y
        if self.parent_form:
            self.parent_form.properties.is_modified = True
    
    def set_size(self, width: int, height: int) -> None:
        """Resize widget"""
        self.width = width
        self.height = height
        if self.parent_form:
            self.parent_form.properties.is_modified = True
    
    def contains_point(self, x: int, y: int) -> bool:
        """Check if point is within widget bounds (for designer)"""
        return (
            self.x_position <= x < self.x_position + self.width and
            self.y_position <= y < self.y_position + self.height
        )
    
    def compile_to_resource(self) -> bytes:
        """
        Generate Palm OS resource binary for this widget.
        Different widget types have different resource formats.
        """
        # Implementation pending
        raise NotImplementedError(f"Resource compilation for {self.widget_type} not implemented")


# Specific widget types (examples - full set would include all 14+ types)

@dataclass
class PalmButton(PalmWidgetBase):
    """
    Standard Palm OS button widget.
    Equivalent to VB6 CUIButton.
    """
    
    frame_style: bool = True  # Draw border around button
    bold_frame: bool = True   # Bold border
    anchor_left: bool = True  # Positioning hint
    
    def __post_init__(self):
        self.widget_type = PalmWidgetType.BUTTON
        self.width = 36
        self.height = 12
        self.label_text = "OK"


@dataclass
class PalmField(PalmWidgetBase):
    """
    Text input field widget.
    Equivalent to VB6 CUIField.
    """
    
    max_characters: int = 255
    auto_shift: bool = True  # Auto-capitalize
    numeric_only: bool = False
    editable: bool = True
    underlined: bool = True
    single_line: bool = True
    
    def __post_init__(self):
        self.widget_type = PalmWidgetType.FIELD
        self.width = 60
        self.height = 12


@dataclass  
class PalmList(PalmWidgetBase):
    """
    List widget for displaying multiple items.
    Equivalent to VB6 CUIList.
    """
    
    visible_item_count: int = 5
    items: list = None  # List of string items
    selected_index: int = -1
    
    def __post_init__(self):
        if self.items is None:
            self.items = []
        self.widget_type = PalmWidgetType.LIST
        self.width = 80
        self.height = 40
    
    def add_item(self, item_text: str) -> None:
        """Add item to list"""
        self.items.append(item_text)
    
    def remove_item(self, index: int) -> bool:
        """Remove item at index"""
        if 0 <= index < len(self.items):
            self.items.pop(index)
            if self.selected_index == index:
                self.selected_index = -1
            return True
        return False
    
    def clear_items(self) -> None:
        """Remove all items"""
        self.items.clear()
        self.selected_index = -1
