#!/bin/bash
#set -x
#
# runRaft.sh: Run the raft code on this node
#
# Assuming the hostname has a name ending in '-[a...x]'
# and that the hostlist.txt is in alphabetical order
# This code turns the trailing letter in the hostname
# into an ordinal int the hostlist.txt such that
# [abc...z] -> [123...26]
#
char=$(hostname | awk -F'-' '{print "\047" $NF}')
char_ord=$(printf '%d' $char)
id=$(echo $char_ord | awk '{ print ($0 - 96);}')
bin/raft clusterlist.txt $id &
