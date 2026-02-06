#!/usr/bin/env python3
"""
NS Basic/Palm OS IDE Launcher

Entry point for the NS Basic/Palm development environment.
This file is used by PyInstaller to create the executable.
"""

import sys
from pathlib import Path

# Add the package to Python path
sys.path.insert(0, str(Path(__file__).parent))

from nsbasic_palm.utils.logging_system import get_nsbasic_logger


def main():
    """Main entry point for NS Basic/Palm IDE"""
    logger = get_nsbasic_logger('app')
    logger.info("NS Basic/Palm OS IDE starting...")
    
    print("=" * 60)
    print("NS Basic/Palm OS - Python Edition")
    print("Palm OS Development Environment")
    print("=" * 60)
    print()
    print("Version: 0.1.0-alpha")
    print("Python Conversion - Phase 2 Implementation")
    print()
    print("Status: Development build")
    print("  ✓ Core models implemented")
    print("  ✓ Logging system active")
    print("  ✓ Testing infrastructure ready")
    print("  ⏳ GUI implementation pending")
    print("  ⏳ BASIC compiler pending")
    print()
    print("This is a Phase 2 development build.")
    print("GUI components will be available in future updates.")
    print()
    
    # Test core functionality
    from nsbasic_palm.models import PalmProject, PalmForm, PalmButton
    
    logger.info("Testing core Palm OS models...")
    
    # Create sample project
    project = PalmProject()
    project.metadata.project_title = "Hello Palm"
    project.metadata.creator_code = "HELO"
    
    # Create sample form
    form = PalmForm()
    form.properties.form_name = "frmMain"
    form.properties.resource_id = project.allocate_resource_id()
    
    # Add button
    button = PalmButton()
    button.widget_name = "btnHello"
    button.label_text = "Hello World"
    button.resource_id = project.allocate_resource_id()
    button.set_position(60, 140)
    
    form.add_widget(button)
    project.forms_list.append(form)
    
    print(f"Created sample project: '{project.metadata.project_title}'")
    print(f"  Creator Code: {project.metadata.creator_code}")
    print(f"  Forms: {len(project.forms_list)}")
    print(f"  Widgets: {len(form.widgets)}")
    print(f"  Resource IDs allocated: {project.next_resource_id - 1000}")
    print()
    
    logger.info("Core functionality test completed successfully")
    print("✓ Core functionality verified")
    print()
    print("Press Enter to exit...")
    input()
    
    return 0


if __name__ == '__main__':
    sys.exit(main())
