# breakout
The classic Atari Arcade game in C using SPL

## Installation instructions
1. Clone this repo into your local machine
2. Install SPL libraries
```
# Install build dependencies and set up /usr/local/src for administrators.
sudo apt-get update && sudo apt-get install build-essential openjdk-7-jdk git
cd /usr/local/src && sudo chgrp sudo . && sudo chmod g+w,+t .

# Retrieve, configure, build, test, and install the library.
git clone git://github.com/cs50/spl.git && cd spl
make SHELL=/bin/bash
build/tests/TestStanfordCSLib
sudo make SHELL=/bin/bash install
```

---

## TODO
* [Done}]Fix the SPL dependencies
