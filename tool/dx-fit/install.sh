#!/bin/bash
# DX-Fit Tool Installation Script

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=========================================="
echo "DX-Fit Tool Installation"
echo "=========================================="

# Detect Python command
if command -v python3 &> /dev/null; then
    PYTHON_CMD="python3"
    PIP_CMD="pip3"
elif command -v python &> /dev/null; then
    PYTHON_CMD="python"
    PIP_CMD="pip"
else
    echo "Error: Python not found. Please install Python 3.6+"
    exit 1
fi

PYTHON_VERSION=$($PYTHON_CMD --version 2>&1 | awk '{print $2}')
echo "Using Python: $PYTHON_CMD ($PYTHON_VERSION)"
echo ""

# Check if we're in the correct directory
if [ ! -f "$SCRIPT_DIR/dx-fit" ]; then
    echo "Error: dx-fit script not found"
    echo "Expected location: $SCRIPT_DIR/dx-fit"
    exit 1
fi

# Update shebang in dx-fit scripts to match detected Python
echo "Updating scripts for your Python installation..."
sed -i "1s|.*|#!$(command -v $PYTHON_CMD)|" "$SCRIPT_DIR/dx-fit"
sed -i "1s|.*|#!$(command -v $PYTHON_CMD)|" "$SCRIPT_DIR/dx-fit-analyze"

# Make scripts executable
echo "Making scripts executable..."
chmod +x "$SCRIPT_DIR/dx-fit"
chmod +x "$SCRIPT_DIR/dx-fit-analyze"

# Check for required Python packages
echo ""
echo "Checking Python dependencies..."
MISSING_PACKAGES=()

for package in pandas yaml matplotlib seaborn; do
    if ! $PYTHON_CMD -c "import $package" 2>/dev/null; then
        MISSING_PACKAGES+=("$package")
    fi
done

if [ ${#MISSING_PACKAGES[@]} -gt 0 ]; then
    echo "Missing Python packages: ${MISSING_PACKAGES[*]}"
    echo "Installing required Python packages..."
    
    # Map package names for pip
    PIP_PACKAGES=()
    for pkg in "${MISSING_PACKAGES[@]}"; do
        if [ "$pkg" == "yaml" ]; then
            PIP_PACKAGES+=("pyyaml")
        else
            PIP_PACKAGES+=("$pkg")
        fi
    done
    
    $PIP_CMD install "${PIP_PACKAGES[@]}"
else
    echo "✓ All core Python dependencies satisfied"
fi

# Check for optional Bayesian optimization support
echo ""
echo "Checking optional dependencies..."
if ! $PYTHON_CMD -c "import skopt" 2>/dev/null; then
    echo "⚠ scikit-optimize not installed (required for Bayesian Optimization)"
    echo "  Install with: $PIP_CMD install scikit-optimize"
    echo "  Note: Grid and Random search strategies will still work"
    
    # Offer to install
    read -p "Install scikit-optimize now? (y/n) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        $PIP_CMD install scikit-optimize
        echo "✓ scikit-optimize installed"
    fi
else
    echo "✓ scikit-optimize available (Bayesian Optimization enabled)"
fi

# Create symbolic links in bin directory if it exists
BIN_DIR="$SCRIPT_DIR/../../bin"
if [ -d "$BIN_DIR" ]; then
    echo ""
    echo "Creating symbolic links in bin directory..."
    ln -sf "$SCRIPT_DIR/dx-fit" "$BIN_DIR/dx-fit"
    ln -sf "$SCRIPT_DIR/dx-fit-analyze" "$BIN_DIR/dx-fit-analyze"
    echo "✓ Symbolic links created"
fi

echo ""
echo "=========================================="
echo "Installation completed!"
echo "=========================================="
echo ""
echo "Python command: $PYTHON_CMD"
echo "Scripts configured to use: $(command -v $PYTHON_CMD)"
echo ""
echo "Quick Start:"
echo "  ./dx-fit examples/04_bayesian_standard.yaml    # Bayesian (recommended)"
echo "  ./dx-fit examples/02_quick_random.yaml         # Quick test"
echo "  ./dx-fit-analyze results_*.csv -o analysis     # Analyze results"
echo ""
echo "Or use with Python directly:"
echo "  $PYTHON_CMD dx-fit examples/04_bayesian_standard.yaml"
echo ""
echo "Available examples:"
echo "  01_template.yaml              - Configuration template"
echo "  02_quick_random.yaml          - Quick random exploration"
echo "  03_bayesian_quick.yaml        - Bayesian quick sweep"
echo "  04_bayesian_standard.yaml     - Bayesian standard ⭐"
echo "  05_grid_small.yaml            - Small grid search"
echo "  06_thermal_bayesian.yaml      - Bayesian + thermal management"
echo "  07_thermal_fixed_cooldown.yaml - Fixed cooldown"
echo "  08_grid_full.yaml             - Full grid (slow)"
echo ""
echo "For more information:"
echo "  README.md              - Project overview"
echo "  docs/QUICKSTART.md     - Quick start guide"
echo "  docs/README.md         - Complete guide"
echo ""