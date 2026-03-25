#!/bin/bash
# DMA Abort Recovery Test Script
# Tests single-client and multi-client recovery scenarios.
#
# ─── Prerequisites ───────────────────────────────────────────
#  1. dxrt_driver module loaded with fault_inject_skip_addr_check support
#       insmod dxrt_driver.ko
#     Verify: cat /sys/module/dxrt_driver/parameters/fault_inject_skip_addr_check
#
#  2. dxrtd service running (systemd or manual)
#       systemctl start dxrtd   OR   dxrtd &
#
#  3. RT library built with DXRT_FAULT_INJECT_OUTPUT env var support
#     (no special build flag needed — env var checked at runtime)
#
#  4. Model files present in working directory:
#       AD01FP32_1-AD01FP32-1/AD01FP32_1.dxnn
#       YOLOV5S_1-YOLOV5S-3/YOLOV5S_1.dxnn
#
#  5. Run as root (or with sudo) — needed for driver sysfs write
# ─────────────────────────────────────────────────────────────
#
# Fault Injection Mechanism:
#   [Driver] fault_inject_skip_addr_check=1
#            → DMA에 잘못된 주소가 들어와도 -EINVAL 대신 bypass하여
#              실제 DMA를 진행 → HW abort 발생
#   [RT]     DXRT_FAULT_INJECT_OUTPUT=N
#            → N번째 output Read 시 base address의 상위 32비트를 0으로
#              만들어 잘못된 주소로 DMA Read 요청
#
# Usage:
#   ./test_dma_recovery.sh                    # default: run all tests
#   ./test_dma_recovery.sh -t single           # single-client test only
#   ./test_dma_recovery.sh -t multi            # multi-client test only
#   ./test_dma_recovery.sh -t all              # both (same as default)
#   ./test_dma_recovery.sh -f 5000             # fault injection loop count
#   ./test_dma_recovery.sh -n 1000             # fault at Nth output read
#   ./test_dma_recovery.sh -v 200              # verification loop count
#   ./test_dma_recovery.sh -t single -f 1001 -v 1
#   ./test_dma_recovery.sh -t single -i 10      # single test 10 iterations
#   ./test_dma_recovery.sh -t multi -i 10       # multi test 10 iterations
#   ./test_dma_recovery.sh -i 5                 # both tests, 5 iterations each

set -euo pipefail

# ─── Defaults ───
FAULT_LOOPS=10000
FAULT_AT=1000
VERIFY_LOOPS=100
SLEEP_SEC=10
ITERATIONS=1
RUN_SINGLE=0
RUN_MULTI=0

# Log directory: next to the script so sudo and regular user both can write
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
LOG_DIR="${LOG_DIR:-${SCRIPT_DIR}/dma_recovery_logs}"

MODEL_SMALL="AD01FP32_1-AD01FP32-1/AD01FP32_1.dxnn"
MODEL_LARGE="YOLOV5S_1-YOLOV5S-3/YOLOV5S_1.dxnn"

DRV_FAULT_SYSFS="/sys/module/dxrt_driver/parameters/fault_inject_skip_addr_check"

# ─── Parse options ───
usage() {
    cat <<EOF
Usage: $0 [OPTIONS]

Options:
  -t TYPE     Test type: single, multi, all (default: all)
  -f COUNT    Fault injection loop count (default: $FAULT_LOOPS)
  -n N        Fault at Nth output read (default: $FAULT_AT)
  -v COUNT    Verification loop count (default: $VERIFY_LOOPS)
  -s SEC      Sleep seconds between stages (default: $SLEEP_SEC)
  -i COUNT    Number of iterations (default: $ITERATIONS)
  -h          Show this help

Examples:
  $0 -t single -f 2000 -n 1000 -v 10
  $0 -t multi -i 10
  $0 -f 5000 -n 1000
EOF
    exit 0
}

while getopts "f:n:v:s:t:i:h" opt; do
    case $opt in
        f) FAULT_LOOPS=$OPTARG ;;
        n) FAULT_AT=$OPTARG ;;
        v) VERIFY_LOOPS=$OPTARG ;;
        s) SLEEP_SEC=$OPTARG ;;
        i) ITERATIONS=$OPTARG ;;
        h) usage ;;
        t) case "$OPTARG" in
               single) RUN_SINGLE=1 ;;
               multi)  RUN_MULTI=1 ;;
               all)    RUN_SINGLE=1; RUN_MULTI=1 ;;
               *) echo "Invalid test: $OPTARG (use: single, multi, all)"; exit 1 ;;
           esac ;;
        *) usage ;;
    esac
done

# Default: run all tests if -t not specified
if [[ $RUN_SINGLE -eq 0 && $RUN_MULTI -eq 0 ]]; then
    RUN_SINGLE=1
    RUN_MULTI=1
fi

# ─── Colors ───
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

pass() { echo -e "${GREEN}[PASS]${NC} $1"; }
fail() { echo -e "${RED}[FAIL]${NC} $1"; }
info() { echo -e "${CYAN}[INFO]${NC} $1"; }
warn() { echo -e "${YELLOW}[WARN]${NC} $1"; }

# ─── Pre-check: dxrtd service running ───
echo ""
echo "============================================================"
echo " DMA Abort Recovery Test"
echo "============================================================"
TESTS=""
[[ $RUN_SINGLE -eq 1 ]] && TESTS="${TESTS}single "
[[ $RUN_MULTI -eq 1 ]]  && TESTS="${TESTS}multi "
echo " Tests to run          : $TESTS"
echo " Iterations            : $ITERATIONS"
echo " Fault injection loops : $FAULT_LOOPS"
echo " Fault at output read  : #$FAULT_AT"
echo " Verification loops    : $VERIFY_LOOPS"
echo " Sleep between stages  : ${SLEEP_SEC}s"
echo "============================================================"
echo ""

# ─── Prerequisites Check ───
info "Checking prerequisites..."
PREREQ_OK=1

# 1. Check dxrtd service
DXRTD_PID=$(pgrep -x dxrtd 2>/dev/null || true)
if [[ -z "$DXRTD_PID" ]]; then
    fail "[Prereq] dxrtd is NOT running."
    echo "         Start with: systemctl start dxrtd  OR  dxrtd &"
    PREREQ_OK=0
else
    pass "[Prereq] dxrtd is running (PID: $DXRTD_PID)"
fi

# 2. Check driver module loaded & sysfs parameter exists
if [[ ! -f "$DRV_FAULT_SYSFS" ]]; then
    fail "[Prereq] Driver sysfs not found: $DRV_FAULT_SYSFS"
    echo "         dxrt_driver module might not be loaded."
    echo "         Load with: insmod dxrt_driver.ko"
    echo "         Or with fault injection pre-enabled:"
    echo "           insmod dxrt_driver.ko fault_inject_skip_addr_check=1"
    PREREQ_OK=0
else
    DRV_FAULT_CUR=$(cat "$DRV_FAULT_SYSFS" 2>/dev/null || echo "?") 
    pass "[Prereq] Driver fault_inject_skip_addr_check sysfs found (current=$DRV_FAULT_CUR)"
fi

# 3. Check write permission to driver sysfs (need root)
if [[ -f "$DRV_FAULT_SYSFS" ]] && ! echo 0 > "$DRV_FAULT_SYSFS" 2>/dev/null; then
    fail "[Prereq] Cannot write to $DRV_FAULT_SYSFS (need root/sudo)"
    echo "         Run this script with: sudo $0 $*"
    PREREQ_OK=0
else
    if [[ -f "$DRV_FAULT_SYSFS" ]]; then
        pass "[Prereq] Driver sysfs is writable"
    fi
fi

# 4. Check model files exist
for model in "$MODEL_SMALL" "$MODEL_LARGE"; do
    if [[ ! -f "$model" ]]; then
        fail "[Prereq] Model file not found: $model"
        PREREQ_OK=0
    fi
done
if [[ $PREREQ_OK -eq 1 ]]; then
    pass "[Prereq] Model files found"
fi

# 5. Check FAULT_AT < FAULT_LOOPS
if [[ $FAULT_AT -ge $FAULT_LOOPS ]]; then
    fail "[Prereq] FAULT_AT ($FAULT_AT) must be < FAULT_LOOPS ($FAULT_LOOPS)"
    echo "         Fault won't trigger if loop count is too low."
    PREREQ_OK=0
else
    pass "[Prereq] FAULT_AT=$FAULT_AT < FAULT_LOOPS=$FAULT_LOOPS"
fi

if [[ $PREREQ_OK -eq 0 ]]; then
    echo ""
    fail "Prerequisites not met. Fix the above issues and retry."
    exit 1
fi
echo ""
pass "All prerequisites met."

# Create log directory and ensure it is writable
mkdir -p "$LOG_DIR"
chmod 777 "$LOG_DIR"
info "Log directory: $LOG_DIR"

# ─── Fault Injection Helpers ───
enable_fault_injection() {
    info "Enabling fault injection..."
    # Driver: bypass address validation for DMA
    echo 1 > "$DRV_FAULT_SYSFS"
    pass "  Driver: fault_inject_skip_addr_check=1"
    # RT env var is set per-process via env prefix on run_model
    info "  RT: DXRT_FAULT_INJECT_OUTPUT=$FAULT_AT"
}

disable_fault_injection() {
    info "Disabling fault injection..."
    echo 0 > "$DRV_FAULT_SYSFS" 2>/dev/null || true
    pass "  Driver: fault_inject_skip_addr_check=0"
}

# Ensure fault injection is always disabled on exit (even on error/interrupt)
cleanup() {
    echo ""
    info "Cleanup: restoring driver fault injection to disabled..."
    echo 0 > "$DRV_FAULT_SYSFS" 2>/dev/null || true
    pass "  Driver: fault_inject_skip_addr_check=0 (restored)"
}
trap cleanup EXIT

# Helper: check if output contains "Benchmark Result" line → success
check_benchmark_result() {
    local logfile=$1
    local label=$2
    if grep -q "Benchmark Result" "$logfile"; then
        local fps
        fps=$(grep "FPS" "$logfile" | grep -oP '[\d.]+' | tail -1)
        pass "$label — FPS: $fps"
        return 0
    else
        fail "$label — No benchmark result (process crashed or error)"
        echo "  Last 10 lines of log:"
        tail -10 "$logfile" | sed 's/^/    /'
        return 1
    fi
}

# Helper: wait for dxrtd to restart (after recovery _Exit)
wait_for_dxrtd() {
    local timeout=$1
    local elapsed=0
    info "Waiting for dxrtd to restart (up to ${timeout}s)..."
    while [[ $elapsed -lt $timeout ]]; do
        if pgrep -x dxrtd > /dev/null 2>&1; then
            local new_pid
            new_pid=$(pgrep -x dxrtd)
            pass "dxrtd restarted (PID: $new_pid) after ${elapsed}s"
            # Give it a moment to fully initialize
            sleep 2
            return 0
        fi
        sleep 1
        elapsed=$((elapsed + 1))
    done
    fail "dxrtd did not restart within ${timeout}s"
    return 1
}

RESULT_SINGLE=0
RESULT_MULTI=0
FAIL_SINGLE=0
FAIL_MULTI=0

for ITER in $(seq 1 $ITERATIONS); do

echo ""
echo "############################################################"
echo " Iteration $ITER / $ITERATIONS"
echo "############################################################"

if [[ $RUN_SINGLE -eq 1 ]]; then
# ═══════════════════════════════════════════════════════════════
# TEST 1: Single Client Recovery
# ═══════════════════════════════════════════════════════════════
echo ""
echo "============================================================"
echo " TEST 1: Single Client Recovery"
echo "============================================================"
echo ""

# Phase 1: Trigger fault
info "[1/2] Running fault injection (loops=$FAULT_LOOPS, fault at output #$FAULT_AT)..."
enable_fault_injection
DXRTD_PID_BEFORE=$(pgrep -x dxrtd 2>/dev/null || echo "none")

env DXRT_FAULT_INJECT_OUTPUT="$FAULT_AT" run_model -m "$MODEL_SMALL" -l "$FAULT_LOOPS" > "$LOG_DIR/dma_test_single_fault.log" 2>&1 || true

disable_fault_injection
info "Fault injection client exited."
info "Sleeping ${SLEEP_SEC}s for recovery + dxrtd restart..."
sleep "$SLEEP_SEC"

# Verify dxrtd restarted
DXRTD_PID_AFTER=$(pgrep -x dxrtd 2>/dev/null || echo "none")
if [[ "$DXRTD_PID_AFTER" == "none" ]]; then
    if ! wait_for_dxrtd 30; then
        fail "TEST 1 ABORTED: dxrtd not available"
        RESULT_SINGLE=1
    fi
elif [[ "$DXRTD_PID_BEFORE" != "$DXRTD_PID_AFTER" ]]; then
    pass "dxrtd restarted (PID: $DXRTD_PID_BEFORE → $DXRTD_PID_AFTER)"
else
    warn "dxrtd PID unchanged ($DXRTD_PID_AFTER) — may not have restarted"
fi

# Phase 2: Verify normal operation
if [[ $RESULT_SINGLE -eq 0 ]]; then
    info "[2/2] Running verification (loops=$VERIFY_LOOPS)..."
    run_model -m "$MODEL_SMALL" -l "$VERIFY_LOOPS" > "$LOG_DIR/dma_test_single_verify.log" 2>&1 || true

    if check_benchmark_result "$LOG_DIR/dma_test_single_verify.log" "Single Client Verify"; then
        pass "TEST 1: Single Client Recovery — PASSED"
    else
        fail "TEST 1: Single Client Recovery — FAILED (iter $ITER)"
        RESULT_SINGLE=1
        FAIL_SINGLE=$((FAIL_SINGLE + 1))
    fi
fi
fi  # RUN_SINGLE

if [[ $RUN_MULTI -eq 1 ]]; then
# ═══════════════════════════════════════════════════════════════
# TEST 2: Multi Client Recovery
# ═══════════════════════════════════════════════════════════════
echo ""
echo "============================================================"
echo " TEST 2: Multi Client Recovery"
echo "============================================================"
echo ""

# Phase 1: Trigger fault with four concurrent clients
info "[1/2] Running fault injection with 4 clients (loops=$FAULT_LOOPS, fault at output #$FAULT_AT)..."
enable_fault_injection
DXRTD_PID_BEFORE=$(pgrep -x dxrtd 2>/dev/null || echo "none")

env DXRT_FAULT_INJECT_OUTPUT="$FAULT_AT" run_model -m "$MODEL_SMALL" -l "$FAULT_LOOPS" > "$LOG_DIR/dma_test_multi_fault_1.log" 2>&1 &
PID_CLIENT1=$!
env DXRT_FAULT_INJECT_OUTPUT="$FAULT_AT" run_model -m "$MODEL_LARGE" -l "$FAULT_LOOPS" > "$LOG_DIR/dma_test_multi_fault_2.log" 2>&1 &
PID_CLIENT2=$!
env DXRT_FAULT_INJECT_OUTPUT="$FAULT_AT" run_model -m "$MODEL_SMALL" -l "$FAULT_LOOPS" > "$LOG_DIR/dma_test_multi_fault_3.log" 2>&1 &
PID_CLIENT3=$!
env DXRT_FAULT_INJECT_OUTPUT="$FAULT_AT" run_model -m "$MODEL_LARGE" -l "$FAULT_LOOPS" > "$LOG_DIR/dma_test_multi_fault_4.log" 2>&1 &
PID_CLIENT4=$!

info "Client 1 PID: $PID_CLIENT1 ($MODEL_SMALL)"
info "Client 2 PID: $PID_CLIENT2 ($MODEL_LARGE)"
info "Client 3 PID: $PID_CLIENT3 ($MODEL_SMALL)"
info "Client 4 PID: $PID_CLIENT4 ($MODEL_LARGE)"

# Wait for all to finish (they should crash/exit after fault)
wait $PID_CLIENT1 2>/dev/null || true
wait $PID_CLIENT2 2>/dev/null || true
wait $PID_CLIENT3 2>/dev/null || true
wait $PID_CLIENT4 2>/dev/null || true

disable_fault_injection
info "All fault injection clients exited."
info "Sleeping ${SLEEP_SEC}s for recovery + dxrtd restart..."
sleep "$SLEEP_SEC"

# Verify dxrtd restarted
DXRTD_PID_AFTER=$(pgrep -x dxrtd 2>/dev/null || echo "none")
if [[ "$DXRTD_PID_AFTER" == "none" ]]; then
    if ! wait_for_dxrtd 30; then
        fail "TEST 2 ABORTED: dxrtd not available"
        RESULT_MULTI=1
    fi
elif [[ "$DXRTD_PID_BEFORE" != "$DXRTD_PID_AFTER" ]]; then
    pass "dxrtd restarted (PID: $DXRTD_PID_BEFORE → $DXRTD_PID_AFTER)"
else
    warn "dxrtd PID unchanged ($DXRTD_PID_AFTER) — may not have restarted"
fi

# Phase 2: Verify normal operation
if [[ $RESULT_MULTI -eq 0 ]]; then
    info "[2/2] Running verification (loops=$VERIFY_LOOPS)..."
    run_model -m "$MODEL_SMALL" -l "$VERIFY_LOOPS" > "$LOG_DIR/dma_test_multi_verify.log" 2>&1 || true

    if check_benchmark_result "$LOG_DIR/dma_test_multi_verify.log" "Multi Client Verify"; then
        pass "TEST 2: Multi Client Recovery — PASSED"
    else
        fail "TEST 2: Multi Client Recovery — FAILED (iter $ITER)"
        RESULT_MULTI=1
        FAIL_MULTI=$((FAIL_MULTI + 1))
    fi
fi
fi  # RUN_MULTI

done  # ITERATIONS loop

# ═══════════════════════════════════════════════════════════════
# Summary
# ═══════════════════════════════════════════════════════════════
echo ""
echo "============================================================"
echo " Test Summary"
echo "============================================================"
if [[ $RUN_SINGLE -eq 1 ]]; then
    if [[ $FAIL_SINGLE -eq 0 ]]; then
        pass "TEST 1: Single Client Recovery ($ITERATIONS/$ITERATIONS passed)"
    else
        fail "TEST 1: Single Client Recovery ($FAIL_SINGLE/$ITERATIONS failed)"
    fi
else
    info "TEST 1: Single Client Recovery — SKIPPED"
fi
if [[ $RUN_MULTI -eq 1 ]]; then
    if [[ $FAIL_MULTI -eq 0 ]]; then
        pass "TEST 2: Multi Client Recovery ($ITERATIONS/$ITERATIONS passed)"
    else
        fail "TEST 2: Multi Client Recovery ($FAIL_MULTI/$ITERATIONS failed)"
    fi
else
    info "TEST 2: Multi Client Recovery — SKIPPED"
fi
echo "============================================================"
echo " Log directory: $LOG_DIR"
echo "   dma_test_single_fault.log"
echo "   dma_test_single_verify.log"
echo "   dma_test_multi_fault_{1..4}.log"
echo "   dma_test_multi_verify.log"
echo "============================================================"

EXIT_CODE=0
[[ $RUN_SINGLE -eq 1 && $RESULT_SINGLE -ne 0 ]] && EXIT_CODE=1
[[ $RUN_MULTI -eq 1 && $RESULT_MULTI -ne 0 ]] && EXIT_CODE=1
exit $EXIT_CODE
