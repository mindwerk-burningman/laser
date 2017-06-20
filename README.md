# Laser Interactivity

## Install

Install latest Leapmotion v2 SDK
https://developer.leapmotion.com/sdk/v2/

Install [homebrew-openlase](https://github.com/edy555/homebrew-openlase) via `homebrew` which has a few patches to allow `openlase` to work on a Mac.

```sh
$ curl -fsSk https://raw.github.com/mxcl/homebrew/go | ruby
$ brew install cmake
$ brew install ffmpeg
$ brew install qt
$ brew install jack --build-from-source
$ brew tap edy555/openlase
$ brew install openlase
```

Install `openusb` via homebrew

```sh
brew install openusb
```

Install the [lasershark_hostapp](https://github.com/marcan/lasershark_hostapp) submodule, which will allow us to run `homebrew-openlase` to the laser and not just the simulator.

```sh
git submodule init
git submodule update
```

## Build ##

```sh
make
```

## Run

Execute:

```sh
./run.sh <program> <output>
```

Where <program> is the laser program, and <output> is either laser, sim, or both.

## Troubleshooting ##

If the simulator is running but not the laser, make sure `LaserOS` is not also running.  


 