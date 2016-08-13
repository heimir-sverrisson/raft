#!/bin/bash
#
# kill the raft processes
#
ps -ef | grep raft | grep -v grep | awk '{print $2}' | xargs kill
