#!/bin/bash

ifconfig | grep "inet"

terminator -x netstat

bin/simple_server -r $HOME/Stack-Smashing
