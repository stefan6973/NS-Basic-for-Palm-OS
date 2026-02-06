# NS Basic/Palm - Development Guide

## Phase 2 Components

This document describes the Phase 2 infrastructure added to the NS Basic/Palm Python conversion project.

### Package Configuration

The project uses modern Python packaging with `pyproject.toml`:

```bash
cd python
pip install -e .   # Installs in development mode
```

### Logging System

Application-wide logging for tracking IDE operations:

```python
from nsbasic_palm.utils.logging_system import get_nsbasic_logger, PalmOSErrorReporter

# Get logger for specific subsystem
logger = get_nsbasic_logger('compiler')
logger.info("Starting BASIC compilation")

# Report user-friendly errors
error_msg = PalmOSErrorReporter.report_error('invalid_creator_code')
print(error_msg)
```

**Subsystems:**
- `app` - General application events
- `compiler` - BASIC compilation process
- `designer` - Form designer operations
- `project` - Project file I/O

**Log Location:** `~/.nsbasic/logs/`

### Continuous Integration

GitHub Actions workflow validates:
- Unit tests pass on all commits
- Palm OS constraints are enforced
- Package builds correctly

See `.github/workflows/palm-os-ci.yml`

### Testing

Run tests with:

```bash
cd python
python -m pytest tests/ -v
```

With coverage:

```bash
python -m pytest tests/ --cov=nsbasic_palm --cov-report=term
```

### Building Distribution

Create distributable package:

```bash
cd python
pip install build
python -m build
```

This generates:
- `dist/nsbasic_palm-0.1.0-py3-none-any.whl`
- `dist/nsbasic-palm-0.1.0.tar.gz`

### Development Workflow

1. Make changes to Python code
2. Run tests locally: `python -m pytest tests/`
3. Check imports: `python -c "from nsbasic_palm.models import *"`
4. Commit changes - CI will run automatically
5. Review CI results in GitHub Actions tab

### Palm OS Specific Validation

The CI pipeline includes Palm OS-specific checks:

- **Resource ID Uniqueness**: Ensures no ID conflicts
- **Creator Code Format**: Validates 4-character codes
- **Screen Boundaries**: Checks 160x160 pixel constraints
- **Widget Positioning**: Verifies coordinate accuracy

### Next Steps for Phase 2

Still needed:
- [ ] PyInstaller configuration for Windows .exe
- [ ] Sphinx documentation generation
- [ ] Performance profiling tools
- [ ] Code quality pre-commit hooks

---

For overall project status, see [PHASE2_PROGRESS.md](../PHASE2_PROGRESS.md)
