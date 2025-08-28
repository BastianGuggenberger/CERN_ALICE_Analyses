#!/usr/bin/env bash
set -euo pipefail

MACRO="${1:-dcafilter.C}"

for i in $(seq 0 11); do
  root -l -b -q "${MACRO}+(${i})"
  printf "\n%02d/12 done\n\n" "$((i+1))"
done