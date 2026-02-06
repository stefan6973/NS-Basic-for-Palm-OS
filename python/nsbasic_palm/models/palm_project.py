"""
Palm OS Project Model

Represents a complete NS Basic/Palm project including:
- Application metadata (name, creator ID, version)
- Forms (Palm OS screens)
- Code modules (BASIC scripts)
- Resources (bitmaps, databases, menus)
- Compilation settings

Converted from VB6 CProject.cls
"""

from dataclasses import dataclass, field
from typing import List
from enum import Enum


class PalmResolution(Enum):
    """Palm screen resolution modes"""
    STANDARD_160x160 = (160, 160)
    HIRES_320x320 = (320, 320)
    DANA_560x160 = (560, 160)


@dataclass
class PalmProjectMetadata:
    """
    Core project settings and metadata.
    Maps to VB6 CProject properties.
    """
    
    #Project identification
    project_title: str = "Untitled"
    creator_code: str = "NSBS"  # 4-char Palm OS creator ID
    app_type_code: str = "appl"  # 4-char application type
    
    # Versioning
    major_version: int = 1
    minor_version: int = 0
    patch_version: int = 0
    
    # Display properties
    launcher_display_name: str = ""
    
    # Icon resources (various bit depths)
    # Small icons
    icon_1bit_small: bytes = b""
    icon_2bit_small: bytes = b""
    icon_4bit_small: bytes = b""
    icon_8bit_small: bytes = b""
    
    # Large icons
    icon_1bit_large: bytes = b""
    icon_2bit_large: bytes = b""
    icon_4bit_large: bytes = b""
    icon_8bit_large: bytes = b""
    
    # High-res variants
    icon_1bit_small_hires: bytes = b""
    icon_2bit_small_hires: bytes = b""
    icon_1bit_large_hires: bytes = b""
    icon_2bit_large_hires: bytes = b""
    
    # Build settings
    target_resolution: PalmResolution = PalmResolution.STANDARD_160x160
    enable_copy_protection: bool = False
    include_debug_symbols: bool = False
    
    # UI Theme
    theme_database_path: str = ""
    
    # File location
    project_file_path: str = ""
    
    # State tracking
    has_unsaved_changes: bool = False
    
    def validate_creator_code(self) -> bool:
        """Ensure creator code is exactly 4 ASCII characters"""
        return (
            len(self.creator_code) == 4 and
            all(32 <= ord(c) <= 126 for c in self.creator_code)
        )
    
    def get_version_string(self) -> str:
        """Format version as string"""
        return f"{self.major_version}.{self.minor_version}.{self.patch_version}"


@dataclass
class PalmProject:
    """
    Complete Palm OS project container.
    Equivalent to VB6 CProject class.
    """
    
    metadata: PalmProjectMetadata = field(default_factory=PalmProjectMetadata)
    
    # Startup/shutdown scripts
    initialization_script: str = ""
    termination_script: str = ""
    
    # Project components (to be populated with actual objects)
    forms_list: List = field(default_factory=list)  # PalmForm instances
    code_modules_list: List = field(default_factory=list)  # PalmCodeModule instances
    bitmap_resources: List = field(default_factory=list)  # PalmBitmap instances
    database_schemas: List = field(default_factory=list)  # PalmDatabase instances
    menu_definitions: List = field(default_factory=list)  # PalmMenu instances
    type_definitions: List = field(default_factory=list)  # Custom type defs
    external_libraries: List = field(default_factory=list)  # Linked libraries
    
    # Compilation artifacts (generated during build)
    compiled_bytecode: bytes = b""
    resource_database: bytes = b""
    
    # Internal tracking
    next_resource_id: int = 1000
    
    def allocate_resource_id(self) -> int:
        """Generate unique resource ID for forms, bitmaps, etc."""
        current_id = self.next_resource_id
        self.next_resource_id += 1
        return current_id
    
    def mark_as_modified(self):
        """Flag project as having unsaved changes"""
        self.metadata.has_unsaved_changes = True
    
    def get_all_code_scripts(self) -> List[str]:
        """Collect all BASIC code from project (for compilation)"""
        scripts = [self.initialization_script, self.termination_script]
        
        # Add form scripts
        for form in self.forms_list:
            # Collect form event handlers
            pass  # To be implemented
        
        # Add code module scripts  
        for module in self.code_modules_list:
            # Collect module code
            pass  # To be implemented
            
        return [s for s in scripts if s.strip()]
    
    def save_to_file(self, filepath: str):
        """
        Serialize project to disk.
        VB6 used custom binary format; Python will use JSON or similar.
        """
        self.metadata.project_file_path = filepath
        # Implementation pending
        raise NotImplementedError("Project serialization not yet implemented")
    
    @classmethod
    def load_from_file(cls, filepath: str) -> 'PalmProject':
        """
        Deserialize project from disk.
        Must be compatible with VB6 .nsb files eventually.
        """
        # Implementation pending
        raise NotImplementedError("Project deserialization not yet implemented")
    
    def compile_to_prc(self, output_path: str) -> bool:
        """
        Compile project to Palm OS .prc executable.
        This is the main build operation.
        """
        # Implementation pending - will interface with compiler module
        raise NotImplementedError("Compilation not yet implemented")
