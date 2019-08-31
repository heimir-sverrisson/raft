#!/usr/bin/env bash
#
for d in pi-3-a pi-3-b pi-3-c pi-3-d pi-3-e
do
  for f in data/$d/raft_*.log
  do
    echo "loading $f ..."
    ./load_one_log.sh $d $f
  done
done
