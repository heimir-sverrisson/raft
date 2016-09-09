#!/bin/bash
TODAY=$(date +%F)
sed -i -e "s/	info	//; s/${TODAY} //" $1
