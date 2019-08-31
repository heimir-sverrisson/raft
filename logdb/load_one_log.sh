#!/usr/bin/env bash
#
#
if [ $# -ne 2 ]; then
  echo "Need two arguments, node-name and file-name" 2>&1
  exit 1
fi
awk -F'\t' -v node=$1 '{print node "\t" substr($1,2,26) "\t" $2 "\t" $4}' < $2 |
pgloader raft_log.load
