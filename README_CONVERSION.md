# NS Basic/Palm - Python Conversion Project

## Overview

This repository contains **NS Basic/Palm**, a historic development environment for creating Palm OS applications using BASIC. The original application was developed in Visual Basic 6.0 from 2000-2009. This project aims to modernize the codebase by converting it to Python 3.8+ with PySide6.

**Status:** Conversion planning and architecture phase

## What is NS Basic/Palm?

NS Basic/Palm was a complete IDE (Integrated Development Environment) for developing Palm OS applications using a BASIC programming language. It provided:

- Visual form designer for Palm OS screens
- BASIC code editor with syntax highlighting  
- Compiler generating Palm OS executables (.prc files)
- Integrated debugger
- Project management system
- Resource editors (menus, bitmaps, databases)

The tool was used by thousands of developers to create Palm OS applications during the Palm platform's heyday.

## Repository Contents

### Original VB6 Source (`/src`)

The complete Visual Basic 6.0 source code:
- **57 VB6 files** totaling **56,000+ lines of code**
- **30+ forms** (UI windows and dialogs)
- **40+ class modules** (data models and business logic)
- **15+ code modules** (utilities and compilers)

Key files:
- `NSBasic.vbp` - Main VB6 project file
- `frmMain.frm` - Primary MDI window
- `MNSBasicCompile.bas` - 16,500 line BASIC compiler
- `CProject.cls` - Project management
- Various `CUI*.cls` - Palm OS UI widget definitions

### C/C++ Runtime (`/any`)

Palm OS runtime components and libraries written in C/C++ using CodeWarrior. These handle:
- Bytecode execution on Palm OS devices
- Resource management
- System integration

### Python Conversion (Planned)

The Python version will be developed in a new `/python` directory with modern package structure.

## Conversion Approach

This is not a simple "port" - it's a complete modernization. See [CONVERSION_STRATEGY.md](CONVERSION_STRATEGY.md) for the detailed technical plan.

### Why Python + PySide6?

**Python Benefits:**
- Modern, maintainable language
- Extensive standard library
- Active community and ecosystem
- Easier testing and debugging than VB6

**PySide6 Benefits:**
- Cross-platform (Windows, macOS, Linux)
- Comprehensive widget set for IDE features
- Good documentation and support
- Professional-grade GUI framework

**Alternatives Considered:**
- Electron/Web - Too heavy for desktop IDE
- C++/Qt - More complex than needed
- C# - Still Windows-centric

### Phased Conversion Plan

**Phase 1: Foundation** (Current)
- Document conversion strategy ✓
- Set up Python package structure
- Create minimal proof-of-concept
- Convert 2-3 core data models

**Phase 2: Core Functionality**
- Project file I/O
- Basic code editor
- Project tree navigation

**Phase 3: Form Designer**
- Visual Palm screen editor
- Widget palette and placement
- Property editor

**Phase 4: Compiler**
- BASIC parser
- Bytecode generation
- Integration with C++ runtime

**Phase 5: Advanced Features**
- Debugger
- Resource editors
- Emulator integration

## Current Status

### Completed
- ✅ Repository cloned and analyzed
- ✅ Codebase statistics gathered
- ✅ Conversion strategy documented
- ✅ Technical decisions made

### In Progress
- 🔄 Python package structure design
- 🔄 Dependencies specification
- 🔄 Model class conversion examples

### Not Started
- ⏳ GUI implementation
- ⏳ Compiler porting
- ⏳ Testing infrastructure
- ⏳ Documentation

## Getting Started (When Available)

Once the Python version reaches MVP status:

```bash
# Clone repository
git clone https://github.com/stefan6973/NS-Basic-for-Palm-OS.git
cd NS-Basic-for-Palm-OS

# Set up Python environment
python3 -m venv venv
source venv/bin/activate  # Windows: venv\Scripts\activate

# Install dependencies
pip install -r requirements.txt

# Run IDE
python -m nsbasic_palm
```

## For Developers

### Prerequisites

- Python 3.8 or higher
- Basic understanding of:
  - Palm OS platform
  - BASIC programming language
  - GUI application development
  - Legacy code modernization

### Contributing

This is primarily an archival/preservation project, but contributions are welcome:

1. Fork the repository
2. Create a feature branch
3. Make changes with tests
4. Submit pull request

See [CONVERSION_STRATEGY.md](CONVERSION_STRATEGY.md) for technical guidelines.

### Development Principles

1. **Preserve Functionality** - Maintain compatibility with original
2. **Modernize Architecture** - Use current Python best practices
3. **Document Extensively** - Explain domain-specific logic
4. **Test Thoroughly** - Especially compilation output
5. **Stay Cross-Platform** - Avoid OS-specific code where possible

## Architecture Overview

### Data Flow

```
User Input (PySide6 GUI)
    ↓
Application Logic (Python)
    ↓
Project Models (Python classes)
    ↓
Compiler (Python → C++ via ctypes)
    ↓
Palm OS Executable (.prc file)
```

### Package Structure (Planned)

```
nsbasic_palm/
├── models/        # Data structures (Project, Form, Widgets)
├── gui/           # PySide6 UI components
├── compiler/      # BASIC parser and code generator
├── palm/          # Palm OS specific code
└── utils/         # Shared utilities
```

## Technical Challenges

1. **VB6 to Python** - Different paradigms (event-driven vs object-oriented)
2. **ActiveX Controls** - No direct Python equivalents
3. **Windows Registry** - Need cross-platform settings storage
4. **COM Integration** - Requires alternative approaches
5. **Complex Compiler** - 16,500 lines of intertwined logic
6. **Domain Knowledge** - Palm OS expertise is rare now

## Resources

- [Original NS Basic/Palm Website](https://www.nsbasic.com/palm/)
- [Palm OS Documentation](https://palmdb.net/)
- [PySide6 Documentation](https://doc.qt.io/qtforpython/)
- [VB6 Language Reference](https://docs.microsoft.com/en-us/previous-versions/visualstudio/)

## FAQ

**Q: Will this run on my Palm device?**
A: The Python version is the IDE only. It still generates Palm OS executables that run on Palm devices/emulators.

**Q: Is the original VB6 version functional?**
A: Possibly, but it requires Windows XP/Vista and is unsupported. The Python version aims to work on modern systems.

**Q: How compatible will project files be?**
A: Goal is 100% compatibility - open old projects, generate identical executables.

**Q: When will it be ready?**
A: This is a multi-month project. Phase 1 proof-of-concept expected in 2-3 weeks.

**Q: Can I help?**
A: Yes! Especially if you have Palm OS or VB6 experience. See Contributing section.

## License

See [LICENSE](LICENSE) file.

Copyright © 2009 NS BASIC Corporation

## Acknowledgments

- **Original Author:** George Henne and the NS BASIC Corporation team
- **Platform:** Palm OS and Palm, Inc.
- **Community:** Thousands of Palm developers who used NS Basic/Palm
- **Preservation:** Everyone helping keep legacy platforms accessible

## Contact

For questions about this conversion project:
- Open a GitHub issue
- See the original author's note in the repository

**Note:** This is an archival/preservation project. The original NS BASIC Corporation is not actively maintaining this code.

---

*Last Updated: 2024*
*Conversion Status: Planning Phase*
