#!/bin/bash

# Validation Test Runner Script
# This script simplifies running validation tests with customizable model paths

set -e

# Default values
BASE_PATH=""
VERBOSE=1
RESULT_DIR=""
CONFIG="test/release/validation_test/test_config/rt/whole_test.json"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print help message
print_help() {
    echo -e "${GREEN}Validation Test Runner${NC}"
    echo ""
    echo -e "${YELLOW}Usage:${NC}"
    echo "    $0 -b <base-path> [options]"
    echo ""
    echo -e "${YELLOW}Required Arguments:${NC}"
    echo "    -b, --base-path <path>     Base path for models directory"
    echo "                               Example: /mnt/public_storage/rt/rt_internal/release_test_models/july/v7/"
    echo ""
    echo -e "${YELLOW}Optional Arguments:${NC}"
    echo "    -r, --result-dir <path>    Directory to save result files (JSON and CSV)"
    echo "                               If not specified, results will not be saved"
    echo "                               Filename format: Valid_{config}_{YY.MM.DD.HH}.{json|csv}"
    echo "                               Example: Valid_Whole_25.10.20.13.json"
    echo "    -c, --config <path>        Config file path (default: whole_test.json)"
    echo "    -v, --verbose <level>      Verbosity level 0-3 (default: 1)"
    echo "    -h, --help                 Show this help message"
    echo ""
    echo -e "${YELLOW}Examples:${NC}"
    echo "    # Basic usage (no result files)"
    echo "    $0 -b ~/ci_models/"
    echo ""
    echo "    # Save results to directory"
    echo "    $0 -b ~/ci_models/ -r ~/dx_report/validation/"
    echo ""
    echo "    # With custom config"
    echo "    $0 -b ~/ci_models/ -c test/release/validation_test/test_config/rt/partial_test.json -r debug/"
    echo ""
    echo "    # With verbose output"
    echo "    $0 -b ~/ci_models/ -v 2 -r debug/"
    echo ""
    echo -e "${YELLOW}Description:${NC}"
    echo "    This script runs the DXRT validation test suite with the following command:"
    echo ""
    echo "    ./bin/validation_test -j <config> --base-path <base-path> \\"
    echo "                         -v <verbose> --random \\"
    echo "                         [-r <result-file>]"
    echo ""
    echo "    The --random flag ensures tests run in random order for better coverage."
    echo "    If -r option is provided, both JSON and CSV result files are generated."
    echo ""
    echo -e "${YELLOW}Available Configs:${NC}"
    echo "    - whole_test.json   : Full test suite (default)"
    echo "    - partial_test.json : Partial test suite"
    echo "    - aging_test.json   : Aging/stress test"
    echo ""
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -b|--base-path)
            BASE_PATH="$2"
            shift 2
            ;;
        -r|--result-dir)
            RESULT_DIR="$2"
            shift 2
            ;;
        -c|--config)
            CONFIG="$2"
            shift 2
            ;;
        -v|--verbose)
            VERBOSE="$2"
            shift 2
            ;;
        -h|--help)
            print_help
            exit 0
            ;;
        *)
            echo -e "${RED}Error: Unknown option: $1${NC}"
            echo "Use -h or --help for usage information"
            exit 1
            ;;
    esac
done

# Show help if no arguments provided
if [[ $# -eq 0 ]] && [[ -z "$BASE_PATH" ]]; then
    print_help
    exit 0
fi

# Validate required arguments
if [[ -z "$BASE_PATH" ]]; then
    echo -e "${RED}Error: Base path (-b) is required${NC}"
    echo "Use -h or --help for usage information"
    exit 1
fi

# Expand tilde in paths
BASE_PATH="${BASE_PATH/#\~/$HOME}"
if [[ -n "$RESULT_DIR" ]]; then
    RESULT_DIR="${RESULT_DIR/#\~/$HOME}"
fi

# Validate paths
if [[ ! -d "$BASE_PATH" ]]; then
    echo -e "${RED}Error: Base path does not exist: $BASE_PATH${NC}"
    exit 1
fi

# Validate and create result directory if specified
if [[ -n "$RESULT_DIR" ]]; then
    if [[ ! -d "$RESULT_DIR" ]]; then
        echo -e "${YELLOW}Creating result directory: $RESULT_DIR${NC}"
        mkdir -p "$RESULT_DIR"
        if [[ $? -ne 0 ]]; then
            echo -e "${RED}Error: Failed to create result directory: $RESULT_DIR${NC}"
            exit 1
        fi
    fi
fi

# Check if validation_test binary exists
VALIDATION_TEST_BIN="$PROJECT_ROOT/bin/validation_test"
if [[ ! -f "$VALIDATION_TEST_BIN" ]]; then
    echo -e "${RED}Error: validation_test binary not found: $VALIDATION_TEST_BIN${NC}"
    echo -e "${YELLOW}Please build the project first:${NC}"
    echo "  cd $PROJECT_ROOT"
    echo "  ./build.sh"
    exit 1
fi

# Check if config file exists
# If config is relative path, check from current directory first, then project root
if [[ "$CONFIG" = /* ]]; then
    # Absolute path
    CONFIG_FULL_PATH="$CONFIG"
else
    # Relative path - check current directory first
    if [[ -f "$CONFIG" ]]; then
        CONFIG_FULL_PATH="$(realpath "$CONFIG")"
    elif [[ -f "$PROJECT_ROOT/$CONFIG" ]]; then
        CONFIG_FULL_PATH="$PROJECT_ROOT/$CONFIG"
    else
        CONFIG_FULL_PATH=""
    fi
fi

if [[ ! -f "$CONFIG_FULL_PATH" ]]; then
    echo -e "${RED}Error: Config file does not exist: $CONFIG${NC}"
    echo -e "${YELLOW}Searched in:${NC}"
    echo -e "  1. Current directory: $(pwd)/$CONFIG"
    echo -e "  2. Project root: $PROJECT_ROOT/$CONFIG"
    exit 1
fi

# Generate result filename if result directory is specified
RESULT_FILE=""
if [[ -n "$RESULT_DIR" ]]; then
    # Extract config type from config path (e.g., whole_test.json -> Whole)
    CONFIG_BASENAME=$(basename "$CONFIG" .json)
    CONFIG_TYPE=$(echo "$CONFIG_BASENAME" | sed 's/_test$//' | sed 's/\b\(.\)/\u\1/g')
    
    # Get current time in KST (UTC+9)
    # Format: YY.MM.DD.HH.MM.SS
    TIMESTAMP=$(TZ='Asia/Seoul' date '+%y.%m.%d.%H.%M')
    
    # Construct result filename
    RESULT_FILENAME="Valid_${CONFIG_TYPE}_${TIMESTAMP}.json"
    RESULT_FILE="$RESULT_DIR/$RESULT_FILENAME"
    
    echo -e "${BLUE}Result file will be saved to:${NC} $RESULT_FILE"
fi

# Print configuration
echo -e "${GREEN}================================${NC}"
echo -e "${GREEN}Validation Test Configuration${NC}"
echo -e "${GREEN}================================${NC}"
echo -e "${BLUE}Project Root:${NC}  $PROJECT_ROOT"
echo -e "${BLUE}Config File:${NC}   $CONFIG"
echo -e "${BLUE}Base Path:${NC}     $BASE_PATH"
echo -e "${BLUE}Verbose Level:${NC} $VERBOSE"
if [[ -n "$RESULT_FILE" ]]; then
    echo -e "${BLUE}Result File:${NC}   $RESULT_FILE"
    echo -e "${BLUE}CSV File:${NC}      ${RESULT_FILE%.json}.csv"
else
    echo -e "${BLUE}Result File:${NC}   Not saving results"
fi
echo -e "${GREEN}================================${NC}"
echo ""

# Change to project root
cd "$PROJECT_ROOT"

# Run validation test
echo -e "${YELLOW}Running validation test...${NC}"
echo ""

# Build command with optional result file
CMD="./bin/validation_test -j \"$CONFIG_FULL_PATH\" --base-path \"$BASE_PATH\" -v $VERBOSE --random --only-csv"

if [[ -n "$RESULT_FILE" ]]; then
    CMD="$CMD -r \"$RESULT_FILE\" -l 3"
fi

# Execute command
eval $CMD

# Capture exit code
EXIT_CODE=$?

echo ""
if [[ $EXIT_CODE -eq 0 ]]; then
    echo -e "${GREEN}✓ Validation test completed successfully${NC}"
    if [[ -n "$RESULT_FILE" ]]; then
        echo -e "${GREEN}✓ Results saved:${NC}"
        echo -e "  ${BLUE}CSV:${NC}  ${RESULT_FILE%.json}.csv"
    fi
else
    echo -e "${RED}✗ Validation test failed with exit code: $EXIT_CODE${NC}"
fi

exit $EXIT_CODE
