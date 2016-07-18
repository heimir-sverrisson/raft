#!/bin/bash
bin/raft hostlist.txt 1 &
cd bin
./raft ../hostlist.txt 2 &
