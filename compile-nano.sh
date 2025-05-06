#!/bin/bash

arduino-cli compile -b arduino:avr:nano
arduino-cli upload -b arduino:avr:nano -p /dev/ttyUSB0
