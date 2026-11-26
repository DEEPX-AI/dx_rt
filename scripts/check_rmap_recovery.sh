#!/bin/bash
#
# check_rmap_recovery.sh - did dxrtd (service mode) handle the FW RMAP recovery
# (DXRT_RECOV_RMAP) correctly?
#
# The FW reports a damaged RMAP with a recovery event and waits up to 3 s for
# DXRT_CMD_START(1). dxrtd rewrites every loaded RMAP block of the device from its
# SHM copy and answers (DeviceDispatcher::HandleFwRecoveryEvent). This script reads
# the dxrt.service journal (and optionally a FW UART capture) and reports, per dxrtd
# process, every RMAP recovery it handled and whether any of them failed.
#
# usage: check_rmap_recovery.sh [-s SINCE] [-e UNTIL] [-u UART_LOG] [-n EXPECTED]
#                               [-w SECONDS] [-U UNIT] [-v]
#   -s SINCE     journal start, journalctl --since syntax ("2026-11-10 21:22:00", "-1h")
#   -e UNTIL     journal end, journalctl --until syntax
#                (no -s/-e: the whole current boot)
#   -u UART_LOG  FW UART capture of the same run, to cross-check the FW side
#   -n EXPECTED  expected number of RMAP recoveries in the window
#   -w SECONDS   a dxrtd exit, abort or NPU BUS Error this soon after an RMAP
#                recovery of the same dxrtd counts as an RMAP failure (default 30);
#                later ones are only listed as unrelated
#   -U UNIT      systemd unit of dxrtd (default: dxrt)
#   -v           print every RMAP log line
#
# exit: 0 PASS, 1 FAIL, 2 usage or environment error, 3 no RMAP recovery in the window

UNIT="dxrt"
SINCE=""
UNTIL=""
UART_LOG=""
EXPECTED=""
WINDOW=30
VERBOSE=0

usage() { sed -n '3,26p' "$0" | sed 's/^# \{0,1\}//'; }

while getopts "s:e:u:n:w:U:vh" opt; do
    case "$opt" in
        s) SINCE="$OPTARG" ;;
        e) UNTIL="$OPTARG" ;;
        u) UART_LOG="$OPTARG" ;;
        n) EXPECTED="$OPTARG" ;;
        w) WINDOW="$OPTARG" ;;
        U) UNIT="$OPTARG" ;;
        v) VERBOSE=1 ;;
        h) usage; exit 0 ;;
        *) usage; exit 2 ;;
    esac
done

if [[ -n "$EXPECTED" && ! "$EXPECTED" =~ ^[0-9]+$ ]]; then echo "error: -n needs a number" >&2; exit 2; fi
if [[ ! "$WINDOW" =~ ^[0-9]+$ ]]; then echo "error: -w needs a number of seconds" >&2; exit 2; fi
if [[ -n "$UART_LOG" && ! -r "$UART_LOG" ]]; then echo "error: cannot read UART log: $UART_LOG" >&2; exit 2; fi

journal_args=(-u "$UNIT" --no-pager -o short-unix)
if [[ -z "$SINCE" && -z "$UNTIL" ]]; then
    journal_args+=(-b)
    window_desc="current boot"
else
    [[ -n "$SINCE" ]] && journal_args+=(--since "$SINCE")
    [[ -n "$UNTIL" ]] && journal_args+=(--until "$UNTIL")
    window_desc="${SINCE:+since \"$SINCE\"}${SINCE:+${UNTIL:+ }}${UNTIL:+until \"$UNTIL\"}"
fi

# Keep only the lines this check reads (a boot can hold millions of dxrtd lines).
# "Started" marks a new dxrtd process, so an exit is never pinned on the previous one.
KEEP='damaged RMAP|RMAP recovery|RestoreRmapBlocks|Error Code +: NPU BUS Error|Terminal recovery failure|Recovery failed for device|Aborting service|Main process exited|Started '
LOG_FILE=$(mktemp) || exit 2
trap 'rm -f "$LOG_FILE" "$LOG_FILE.err"' EXIT
journalctl "${journal_args[@]}" 2>"$LOG_FILE.err" | grep -E -- "$KEEP" > "$LOG_FILE"
if (( PIPESTATUS[0] != 0 )); then
    echo "error: journalctl failed:" >&2; cat "$LOG_FILE.err" >&2; exit 2
fi
if grep -q "not seeing messages from other users" "$LOG_FILE.err"; then
    echo "error: no access to the system journal; run with sudo or join the systemd-journal group" >&2
    exit 2
fi

fmt_ts() { date -d "@${1%.*}" '+%m-%d %H:%M:%S'; }

# One pass over the journal ("<epoch> <host> <ident>[pid]: <message>").
# Messages: lib/device_pool/device_dispatcher.cpp, lib/dxrt_service/dxrt_service_v2.cpp.
# Emits:
#   RUN <pid> <first> <last> <restored> <zero_blocks> <restore_failed> <write_failed> <start_failed>
#   BAD <ts> <pid> <seconds after RMAP> <what>       (counts as an RMAP failure)
#   OTHER <ts> <pid> <what>                          (not near an RMAP recovery; info only)
ANALYSIS=$(awk -v W="$WINDOW" '
    function near(p, t) { return (p in last) && (t - last[p] <= W) }
    function note(p, t, what) {
        if (p == "") p = "?"
        if (near(p, t)) printf "BAD %s %s %d %s\n", t, p, t - last[p], what
        else            printf "OTHER %s %s %s\n", t, p, what
    }
    {
        t = $1; pid = ""
        if (match($3, /^dxrtd\[[0-9]+\]/)) { pid = substr($3, 7, RLENGTH - 7); cur = pid }
    }
    /systemd\[1\]: Started / { cur = "" }
    pid != "" && /FW found a damaged RMAP; restored/ {
        if (!(pid in first)) { first[pid] = t; order[++n] = pid }
        last[pid] = t; ok[pid]++
        if (/restored 0 RMAP block/) zero[pid]++
    }
    pid != "" && /FW requested RMAP recovery but restoring RMAP failed/ {
        if (!(pid in first)) { first[pid] = t; order[++n] = pid }
        last[pid] = t; rfail[pid]++
    }
    pid != "" && /RestoreRmapBlocks: write failed/      { wfail[pid]++ }
    pid != "" && /DXRT_CMD_START for RMAP recovery failed/ { sfail[pid]++ }
    pid != "" && /Error Code +: NPU BUS Error/           { note(pid, t, "FW reported NPU BUS Error") }
    pid != "" && /Terminal recovery failure|Recovery failed for device|Aborting service/ {
        note(pid, t, "fatal recovery path")
    }
    /systemd\[1\]: .*Main process exited/ && !/status=0\/SUCCESS/ && !/status=15\/TERM/ {
        match($0, /code=[a-z]+, status=[^ ]+/)
        note(cur, t, "exited (" substr($0, RSTART, RLENGTH) ")")
    }
    END {
        for (i = 1; i <= n; i++) {
            p = order[i]
            printf "RUN %s %s %s %d %d %d %d %d\n", p, first[p], last[p], ok[p], zero[p], rfail[p], wfail[p], sfail[p]
        }
    }' "$LOG_FILE")

restored=0; zero_blocks=0; restore_fail=0; write_fail=0; start_fail=0
fails=()
echo "== dxrtd ($UNIT.service) : $window_desc"
while read -r tag pid first last ok zero rfail wfail sfail; do
    [[ "$tag" == "RUN" ]] || continue
    (( restored += ok, zero_blocks += zero, restore_fail += rfail, write_fail += wfail, start_fail += sfail ))
    printf '  dxrtd[%s]  %s ~ %s  RMAP recoveries %d (restored %d, restore failed %d)\n' \
        "$pid" "$(fmt_ts "$first")" "$(fmt_ts "$last")" $((ok + rfail)) "$ok" "$rfail"
    (( zero > 0 ))  && fails+=("dxrtd[$pid] found no loaded RMAP block to restore $zero time(s)")
    (( rfail > 0 )) && fails+=("dxrtd[$pid] could not restore the RMAP $rfail time(s)")
    (( wfail > 0 )) && fails+=("dxrtd[$pid] failed $wfail RMAP block write(s)")
    (( sfail > 0 )) && fails+=("dxrtd[$pid] failed DXRT_CMD_START $sfail time(s); the FW dropped those requests")
done <<< "$ANALYSIS"
attempts=$((restored + restore_fail))
(( attempts == 0 )) && echo "  (no RMAP recovery logged)"

while read -r tag ts pid after what; do
    [[ "$tag" == "BAD" ]] || continue
    fails+=("dxrtd[$pid] $what at $(fmt_ts "$ts"), ${after}s after an RMAP recovery")
done <<< "$ANALYSIS"

others=$(printf '%s\n' "$ANALYSIS" | grep -c "^OTHER ")
if (( others > 0 )); then
    echo "  -- not RMAP-related (no RMAP recovery of that dxrtd within ${WINDOW}s before):"
    printf '     dxrtd exits %d, NPU BUS Error %d, fatal recovery %d; latest:\n' \
        "$(printf '%s\n' "$ANALYSIS" | grep -c '^OTHER .* exited (code=')" \
        "$(printf '%s\n' "$ANALYSIS" | grep -c '^OTHER .* NPU BUS Error')" \
        "$(printf '%s\n' "$ANALYSIS" | grep -c '^OTHER .* fatal recovery path')"
    printf '%s\n' "$ANALYSIS" | grep "^OTHER " | tail -5 | while read -r _ ts pid what; do
        [[ "$pid" == "?" ]] && who="dxrtd" || who="dxrtd[$pid]"
        printf '     %s %s %s\n' "$(fmt_ts "$ts")" "$who" "$what"
    done
fi

if (( VERBOSE )) && (( attempts + write_fail + start_fail > 0 )); then
    echo "  -- RMAP lines"
    grep -E "damaged RMAP|RMAP recovery|RestoreRmapBlocks" "$LOG_FILE" | while read -r ts rest; do
        printf '     %s %s\n' "$(fmt_ts "$ts")" "$rest"
    done
fi

if [[ -n "$EXPECTED" ]] && (( attempts != EXPECTED )); then
    fails+=("expected $EXPECTED RMAP recoveries, dxrtd handled $attempts")
fi

# FW side (plat/drivers/dx_npu_diag.c), optional
if [[ -n "$UART_LOG" ]]; then
    UART=$(tr -d '\r' < "$UART_LOG")
    ucount() { printf '%s\n' "$UART" | grep -cE -- "$1"; }
    fw_req=$(ucount "RMAP checksum mismatch\. Requesting RMAP resend from host")
    fw_answered=$(ucount "Wait finished\. Elapsed: [0-9]+ ms, waitHostAct: 0")
    fw_timeout=$(ucount "Timeout for waiting!")
    fw_escalate=$(ucount "Error threshold reached for IRQ_CMD_CKSUM|IRQ_CMD_CKSUM: Threshold exceeded")
    fw_retry_ok=$(ucount "RMAP checksum OK\. Proceeding with NPU reset")
    fw_wait_max=$(printf '%s\n' "$UART" | grep -oE "Wait finished\. Elapsed: [0-9]+ ms" | grep -oE "[0-9]+" | sort -n | tail -1)

    echo "== FW UART : $UART_LOG"
    printf '  %-38s %s\n' "RMAP resend requested" "$fw_req"
    printf '  %-38s %s\n' "  host answered (waitHostAct: 0)" "$fw_answered"
    printf '  %-38s %s\n' "  timed out (3 s)" "$fw_timeout"
    printf '  %-38s %s\n' "  longest host wait" "${fw_wait_max:--} ms"
    printf '  %-38s %s\n' "checksum OK, re-issued without host" "$fw_retry_ok"
    printf '  %-38s %s\n' "IRQ_CMD_CKSUM escalated" "$fw_escalate"

    (( fw_timeout > 0 ))  && fails+=("FW timed out waiting for the host $fw_timeout time(s)")
    (( fw_escalate > 0 )) && fails+=("FW escalated IRQ_CMD_CKSUM $fw_escalate time(s)")
    (( fw_answered != fw_req )) && fails+=("FW requested $fw_req resend(s) but only $fw_answered were answered")
    (( fw_req != attempts )) && fails+=("FW requested $fw_req resend(s) but dxrtd handled $attempts (use -s/-e to match the UART capture)")
fi

echo
if (( ${#fails[@]} > 0 )); then
    for f in "${fails[@]}"; do echo "  - $f"; done
    echo "RESULT: FAIL"
    exit 1
fi
if (( attempts == 0 )); then
    echo "RESULT: NO RMAP RECOVERY IN WINDOW"
    exit 3
fi
echo "RESULT: PASS ($attempts RMAP recovery(s) handled by dxrtd)"
exit 0
