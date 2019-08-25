# Raft

Yet another implementation of the RAFT distributed concensus algorithm.
Description of the algorithm can be found in the [Stanford Paper](http://web.stanford.edu/~ouster/cgi-bin/papers/raft-atc14).
The implementation is done in C++ and is mostly for my pleasure working on low level things that include networking, distributed systems and interesting algorithms.
In the end I want this to run on a cluster (maybe 5) Raspberry Pi's. An implementation of my Distributed Id Service would be the first thing to once this is up and running for real.

##Status
The following things are starting to work:

- UDP client and server sockets
- Multi-threading
- JSON generation and parsing
- Election
- Finate State Machine (FSM) for protocol

##Installation on Rasperry Pi
Install the Raspian OS if not there already. Using a HDMI monitor and USB keyboard/mouse.

Set the hostname:

- Edit `/etc/hostname` and replace `raspberrypi`with your own choice, i.e. `pi-3-[abcdef]`
- Edit `/etc/hosts` the same way for address
- Setup Wifi if needed
  Reboot here and ssh into the new node as HDMI is not useful from now on.

Update the OS:

- `sudo apt-get update`
- `sudo apt-get dist-upgrade`

Increase the swap space

Edit `/etc/dphys-swapfile`

- set `CONF_SWAPSIZE=2048`
- run `sudo systemctl restart dphys-swapfile`
- check that `free -m` shows the increased swap space

Install the `boost` library

- `sudo apt-get install libboost1.62-all`

Install Cmake

- `sudo apt-get install cmake`

Make `/usr/local/src` owned by the user `pi`

- `cd /usr/local`
- `sudo chown -R pi:pi src`
- `cd src`
  
Get `rapidjson` from Github

- `git clone https://github.com/miloyip/rapidjson.git`

Compile the rapidjson from source as described on `http://rapidjson.org`, but make sure to fix the following before you proceed

- Remove `-march=native` from the `CMAKE_CXX_FLAGS` in the file `CMakeLists.txt`
- Proceed with the steps described above under `Installation`

Clone this repo

- `cd /usr/local/src`
- `git clone https://github.com/heimir-sverrisson/raft.git`

Make the software

- `cd raft`
- `make`
s
