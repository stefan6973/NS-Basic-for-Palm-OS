"""
Unit tests for Palm project models.

Tests the Python equivalents of VB6 classes to ensure
they maintain expected behavior.
"""

import pytest
from nsbasic_palm.models import (
    PalmProject,
    PalmProjectMetadata,
    PalmForm,
    PalmButton,
    PalmField,
    PalmList,
)


class TestPalmProjectMetadata:
    """Test project metadata handling"""
    
    def test_default_values(self):
        """Check default initialization"""
        meta = PalmProjectMetadata()
        assert meta.project_title == "Untitled"
        assert meta.creator_code == "NSBS"
        assert meta.major_version == 1
    
    def test_creator_code_validation(self):
        """Ensure creator code is 4 characters"""
        meta = PalmProjectMetadata(creator_code="TEST")
        assert meta.validate_creator_code()
        
        meta.creator_code = "TOO LONG"
        assert not meta.validate_creator_code()
    
    def test_version_string(self):
        """Check version formatting"""
        meta = PalmProjectMetadata(major_version=2, minor_version=3, patch_version=4)
        assert meta.get_version_string() == "2.3.4"


class TestPalmProject:
    """Test project container"""
    
    def test_project_creation(self):
        """Create empty project"""
        project = PalmProject()
        assert len(project.forms_list) == 0
        assert project.metadata.project_title == "Untitled"
    
    def test_resource_id_allocation(self):
        """Ensure unique resource IDs"""
        project = PalmProject()
        id1 = project.allocate_resource_id()
        id2 = project.allocate_resource_id()
        assert id2 == id1 + 1


class TestPalmForm:
    """Test form/screen handling"""
    
    def test_widget_management(self):
        """Add and remove widgets"""
        form = PalmForm()
        button = PalmButton()
        
        form.add_widget(button)
        assert len(form.widgets) == 1
        assert button.parent_form == form
        
        form.remove_widget(button)
        assert len(form.widgets) == 0
        assert button.parent_form is None
    
    def test_find_widget_by_id(self):
        """Locate widgets by ID"""
        form = PalmForm()
        button = PalmButton()
        button.resource_id = 1001
        form.add_widget(button)
        
        found = form.find_widget_by_id(1001)
        assert found == button
        
        not_found = form.find_widget_by_id(9999)
        assert not_found is None


class TestPalmWidgets:
    """Test UI widget classes"""
    
    def test_button_defaults(self):
        """Check button initialization"""
        button = PalmButton()
        assert button.label_text == "OK"
        assert button.width == 36
        assert button.frame_style is True
    
    def test_field_properties(self):
        """Check field widget"""
        field = PalmField()
        assert field.max_characters == 255
        assert field.editable is True
    
    def test_list_operations(self):
        """Test list widget item management"""
        list_widget = PalmList()
        
        list_widget.add_item("Item 1")
        list_widget.add_item("Item 2")
        assert len(list_widget.items) == 2
        
        list_widget.remove_item(0)
        assert len(list_widget.items) == 1
        assert list_widget.items[0] == "Item 2"
        
        list_widget.clear_items()
        assert len(list_widget.items) == 0
    
    def test_widget_positioning(self):
        """Test widget position/size"""
        button = PalmButton()
        button.set_position(10, 20)
        assert button.x_position == 10
        assert button.y_position == 20
        
        button.set_size(50, 15)
        assert button.width == 50
        assert button.height == 15
        
        assert button.get_bounding_box() == (10, 20, 50, 15)
    
    def test_contains_point(self):
        """Test point-in-widget detection"""
        button = PalmButton()
        button.set_position(10, 10)
        button.set_size(30, 20)
        
        assert button.contains_point(15, 15) is True
        assert button.contains_point(5, 5) is False
        assert button.contains_point(50, 50) is False


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
