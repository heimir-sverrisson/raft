#!/usr/bin/env bash
#
cd data
for d in pi-3-a pi-3-b pi-3-c pi-3-d pi-3-e
do
  cd $d
  rm -f raft_*.log
  scp pi@$d:/usr/local/src/raft/raft_'*'.log .
  cd ..
done
