# raft
Yet another implementation of the RAFT distributed concensus algorithm.
The implementation is done in C++ and is mostly for my pleasure working on low level things that include networking, distributed systems and hard algorithms.

##Status
The following things are starting to work:

* UDP client and server sockets
* Multi-threading
* JSON generation and parsing

##Installation on Rasperry Pi
Install the `boost` library
* `sudo apt-get install libboost1.55-all`

Install Cmake
* `sudo apt-get install cmake`

Get `rapidjson` from Github
* `git clone https://github.com/miloyip/rapidjson.git`

Compile the rapidjson from source as described on `http://rapidjson.org`, but make sure to fix the following before you proceed
* Remove `-march=native` from the `CMAKE_CXX_FLAGS` in the file `CMakeLists.txt`
* Proceed with the steps described above under `Installation`
