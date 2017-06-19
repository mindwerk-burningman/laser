# Laser Interactivity

## Install

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

```sh
./test.sh
```

## Running (just simulator)

**Simulator**

```sh
$ jack_server_control

# new tab
$ openlase-simulator 
$ openlase-simple

# new tab
$ jack_connect libol:out_x simulator:in_x
$ jack_connect libol:out_y simulator:in_y
$ jack_connect libol:out_g simulator:in_g
$ jack_connect libol:out_r simulator:in_r
$ jack_connect libol:out_b simulator:in_b
```

## Running (just laser)

```sh
# run lasershark
./lasershark_hostapp/lasershark_jack

# connect ports
$ jack_connect libol:out_x lasershark:in_x
$ jack_connect libol:out_y lasershark:in_y
$ jack_connect libol:out_g lasershark:in_g
$ jack_connect libol:out_r lasershark:in_r
$ jack_connect libol:out_b lasershark:in_b
```

## Troubleshooting ##

If the simulator is running but not the laser, make sure `LaserOS` is not also running.  


 