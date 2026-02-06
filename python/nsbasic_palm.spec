# -*- mode: python ; coding: utf-8 -*-
"""
PyInstaller Specification for NS Basic/Palm OS IDE

Builds standalone Windows executable for the Palm OS development environment.
Includes all Python modules, PySide6 dependencies, and Palm OS resources.
"""

block_cipher = None

# Collect all NS Basic/Palm modules
nsbasic_palm_modules = [
    'nsbasic_palm',
    'nsbasic_palm.models',
    'nsbasic_palm.gui',
    'nsbasic_palm.compiler',
    'nsbasic_palm.palm',
    'nsbasic_palm.utils',
]

# Hidden imports needed for PySide6 and Palm OS components
hidden_imports = [
    'PySide6.QtCore',
    'PySide6.QtGui',
    'PySide6.QtWidgets',
    'nsbasic_palm.utils.logging_system',
]

# Data files to include (UI files, icons, Palm OS resources when available)
datas = [
    ('../README_CONVERSION.md', '.'),
    ('../LICENSE', '.'),
]

a = Analysis(
    ['nsbasic_palm_launcher.py'],
    pathex=[],
    binaries=[],
    datas=datas,
    hiddenimports=hidden_imports,
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=['matplotlib', 'numpy', 'pandas'],  # Exclude unused large packages
    win_no_prefer_redirects=False,
    win_private_assemblies=False,
    cipher=block_cipher,
    noarchive=False,
)

pyz = PYZ(a.pure, a.zipped_data, cipher=block_cipher)

exe = EXE(
    pyz,
    a.scripts,
    [],
    exclude_binaries=True,
    name='NSBasicPalm',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    console=False,  # Windows GUI application
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon='nsbasic_icon.ico' if os.path.exists('nsbasic_icon.ico') else None,
)

coll = COLLECT(
    exe,
    a.binaries,
    a.zipfiles,
    a.datas,
    strip=False,
    upx=True,
    upx_exclude=[],
    name='NSBasicPalm',
)
