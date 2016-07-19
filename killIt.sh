#!/bin/bash
#
# kill the raft processes
#
ps -f | grep raft | grep -v grep | awk '{print $2}' | xargs kill
