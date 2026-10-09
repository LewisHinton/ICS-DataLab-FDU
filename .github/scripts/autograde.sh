#!/usr/bin/env bash
#
# Grading helper that replaces classroom-resources/autograding-io-grader@v1.
#
# The upstream action is a tiny Node program (node20 -> dist/main.js) whose whole
# job is: run a command, compare its stdout with an expected value, and publish a
# base64 JSON blob on the `result` step output. This script reproduces that
# observable behaviour so the workflow owns the logic instead of a third party:
#
#   1. run the test command through a shell, feeding `GRADER_INPUT` on stdin;
#   2. grade the command's stdout (trimmed) against `EXPECTED_OUTPUT` using
#      `COMPARISON_METHOD` (exact | contains | regex);
#   3. award `MAX_SCORE` points when it passes, 0 when it fails or errors;
#   4. never fail the step itself -- like the action, it exits 0 and lets the
#      autograding-grading-reporter decide the overall job status;
#   5. publish the outcome on the `result` step output as base64-encoded JSON in
#      the exact shape the reporter parses (`version`, `status`, `max_score`,
#      `tests[].name/status/message/test_code/line_no/score`).
#
# Configuration comes from the environment so workflow steps stay tiny:
#
#   TEST_NAME          required  human readable test name
#   GRADER_COMMAND     required  shell command whose stdout is graded
#   EXPECTED_OUTPUT    required  value the trimmed stdout is compared against
#   COMPARISON_METHOD  required  exact | contains | regex
#   GRADER_INPUT       optional  stdin handed to the command (default: empty)
#   TIMEOUT_MINUTES    optional  per-command timeout in minutes (default: 10)
#   MAX_SCORE          optional  points granted on success (default: 0)

set -uo pipefail

TEST_NAME=${TEST_NAME:-Unknown Test}
GRADER_COMMAND=${GRADER_COMMAND:-Unknown Command}
EXPECTED_OUTPUT=${EXPECTED_OUTPUT:-}
COMPARISON_METHOD=${COMPARISON_METHOD:-exact}
GRADER_INPUT=${GRADER_INPUT:-}
TIMEOUT_MINUTES=${TIMEOUT_MINUTES:-10}
MAX_SCORE=${MAX_SCORE:-0}

now_ms() {
  python3 -c 'import time; print(int(time.time() * 1000))'
}

stdout_file=$(mktemp)
stderr_file=$(mktemp)
trap 'rm -f "$stdout_file" "$stderr_file"' EXIT

start_ms=$(now_ms)
printf '%s' "$GRADER_INPUT" |
  timeout "${TIMEOUT_MINUTES}m" sh -c "$GRADER_COMMAND" \
    >"$stdout_file" 2>"$stderr_file"
command_status=$?
end_ms=$(now_ms)

# Surface the command output in the job log, mirroring the action's stdio.
cat "$stdout_file"
cat "$stderr_file" >&2

status=pass
score=$MAX_SCORE
message=""

if [ "$command_status" -eq 124 ] || [ "$command_status" -eq 137 ]; then
  status=error
  score=0
  message="Command was killed due to timeout"
elif [ "$command_status" -ne 0 ]; then
  status=error
  score=0
  message="Command failed: ${GRADER_COMMAND}"
  if [ -s "$stderr_file" ]; then
    message="${message}"$'\n'"$(cat "$stderr_file")"
  fi
else
  output=$(python3 -c 'import sys; sys.stdout.write(sys.stdin.read().strip())' <"$stdout_file")
  case "$COMPARISON_METHOD" in
    exact)
      [ "$output" = "$EXPECTED_OUTPUT" ] || { status=fail; score=0; }
      ;;
    contains)
      [[ "$output" == *"$EXPECTED_OUTPUT"* ]] || { status=fail; score=0; }
      ;;
    regex)
      # Use Python's re (closer to the action's JS RegExp than bash ERE).
      printf '%s' "$output" |
        EXPECTED="$EXPECTED_OUTPUT" python3 -c '
import os
import re
import sys

try:
    matched = re.search(os.environ["EXPECTED"], sys.stdin.read())
except re.error as error:
    print(f"Invalid regular expression: {error}", file=sys.stderr)
    sys.exit(2)

sys.exit(0 if matched else 1)
' 2>/dev/null
      case $? in
        0) ;;
        1) status=fail; score=0 ;;
        *) status=error; score=0; message="Invalid regular expression: ${EXPECTED_OUTPUT}" ;;
      esac
      ;;
    *)
      status=error
      score=0
      message="Invalid comparison method: ${COMPARISON_METHOD}"
      ;;
  esac
  if [ "$status" = "fail" ]; then
    message="Output does not match expected: ${EXPECTED_OUTPUT} Got: ${output}"
  fi
fi

result=$(
  STATUS="$status" \
  MESSAGE="$message" \
  SCORE="$score" \
  START_MS="$start_ms" \
  END_MS="$end_ms" \
  TEST_NAME="$TEST_NAME" \
  GRADER_COMMAND="$GRADER_COMMAND" \
  GRADER_INPUT="$GRADER_INPUT" \
  MAX_SCORE="$MAX_SCORE" \
  python3 - <<'PY'
import base64
import json
import os

result = {
    "version": 1,
    "status": os.environ["STATUS"],
    "max_score": int(os.environ["MAX_SCORE"]),
    "tests": [
        {
            "name": os.environ["TEST_NAME"],
            "status": os.environ["STATUS"],
            "message": os.environ["MESSAGE"] or None,
            "test_code": f'{os.environ["GRADER_COMMAND"]} <stdin>{os.environ.get("GRADER_INPUT", "")}',
            "filename": "",
            "line_no": 0,
            "execution_time": f'{(int(os.environ["END_MS"]) - int(os.environ["START_MS"])) / 1000:.3f}s',
            "score": int(os.environ["SCORE"]),
        }
    ],
}

encoded = base64.b64encode(json.dumps(result, separators=(",", ":")).encode()).decode()
print(encoded)
PY
)

# Human readable echo of what the reporter will eventually parse.
printf '%s' "$result" | python3 -c 'import base64, sys; print(base64.b64decode(sys.stdin.read().strip()).decode())'

if [ -n "${GITHUB_OUTPUT:-}" ]; then
  printf 'result=%s\n' "$result" >>"$GITHUB_OUTPUT"
fi

exit 0
