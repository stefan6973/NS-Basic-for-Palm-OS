"""
Palm OS Screen Form Model

Represents a single screen/form in a Palm OS application.
Forms contain UI widgets and event handler scripts.

Converted from VB6 CForm.cls
"""

from dataclasses import dataclass, field
from typing import List, Optional
from enum import Enum


class FormEventType(Enum):
    """Types of form-level events"""
    BEFORE_OPEN = "before"  # Executed before form displays
    ON_EVENT = "event"      # Handles form events (buttons, etc.)
    AFTER_CLOSE = "after"   # Executed when form closes
    ON_HELP = "help"        # Help button handler


@dataclass
class PalmFormProperties:
    """Visual and behavioral properties of a Palm form"""
    
    form_name: str = "Form1"
    form_title: str = ""  # Displayed in title bar
    resource_id: int = 0
    
    # Dimensions (Palm OS constraints)
    width_pixels: int = 160
    height_pixels: int = 160
    
    # Associated resources
    menubar_resource_id: int = 0
    help_resource_id: int = 0
    default_button_id: int = 0  # Button activated by Enter key
    
    # Form behavior
    save_behind: bool = False  # Save screen content when form opens
    modal: bool = False
    
    # State
    is_modified: bool = False


@dataclass
class PalmForm:
    """
    Complete form definition for Palm OS screen.
    Equivalent to VB6 CForm class.
    """
    
    properties: PalmFormProperties = field(default_factory=PalmFormProperties)
    
    # Event handler scripts (BASIC code)
    before_open_script: str = ""
    event_handler_script: str = ""
    after_close_script: str = ""
    help_script: str = ""
    
    # UI widgets on this form
    widgets: List = field(default_factory=list)  # List of PalmWidget instances
    
    # Compilation output
    compiled_form_resource: bytes = b""
    
    def add_widget(self, widget) -> None:
        """
        Add a UI widget to this form.
        Widget positioning is relative to form bounds.
        """
        self.widgets.append(widget)
        widget.parent_form = self
        self.properties.is_modified = True
    
    def remove_widget(self, widget) -> bool:
        """Remove widget from form"""
        if widget in self.widgets:
            self.widgets.remove(widget)
            widget.parent_form = None
            self.properties.is_modified = True
            return True
        return False
    
    def find_widget_by_id(self, resource_id: int) -> Optional:
        """Locate widget by its resource ID"""
        for widget in self.widgets:
            if widget.resource_id == resource_id:
                return widget
        return None
    
    def find_widget_by_name(self, name: str) -> Optional:
        """Locate widget by its name property"""
        for widget in self.widgets:
            if widget.widget_name == name:
                return widget
        return None
    
    def get_event_script(self, event_type: FormEventType) -> str:
        """Retrieve script for specific event"""
        script_map = {
            FormEventType.BEFORE_OPEN: self.before_open_script,
            FormEventType.ON_EVENT: self.event_handler_script,
            FormEventType.AFTER_CLOSE: self.after_close_script,
            FormEventType.ON_HELP: self.help_script,
        }
        return script_map.get(event_type, "")
    
    def set_event_script(self, event_type: FormEventType, script: str) -> None:
        """Update script for specific event"""
        if event_type == FormEventType.BEFORE_OPEN:
            self.before_open_script = script
        elif event_type == FormEventType.ON_EVENT:
            self.event_handler_script = script
        elif event_type == FormEventType.AFTER_CLOSE:
            self.after_close_script = script
        elif event_type == FormEventType.ON_HELP:
            self.help_script = script
        
        self.properties.is_modified = True
    
    def validate_widget_positions(self) -> List[str]:
        """
        Check that all widgets are within form bounds.
        Returns list of error messages.
        """
        errors = []
        for widget in self.widgets:
            if widget.x_position < 0 or widget.y_position < 0:
                errors.append(f"Widget '{widget.widget_name}' has negative position")
            
            if widget.x_position + widget.width > self.properties.width_pixels:
                errors.append(f"Widget '{widget.widget_name}' extends beyond right edge")
            
            if widget.y_position + widget.height > self.properties.height_pixels:
                errors.append(f"Widget '{widget.widget_name}' extends beyond bottom edge")
        
        return errors
    
    def compile_to_resource(self) -> bytes:
        """
        Generate Palm OS form resource binary data.
        This creates the binary representation used at runtime.
        """
        # Implementation pending - needs Palm OS resource format knowledge
        raise NotImplementedError("Form resource compilation not yet implemented")
