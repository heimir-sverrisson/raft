#!/bin/bash
TODAY=$(date +%Y-%b-%d)
sed -i -e "s/	info	//; s/${TODAY} //" $1