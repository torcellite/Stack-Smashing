#!/bin/bash

# Demo for bin/aleph_one/example3

read -p "Enter the offset for bin/aleph_one/example3 " offset
while [ $offset != "stop" ]; do
    bin/aleph_one/example3 $offset
    read -p "Enter the offset for bin/aleph_one/example3 " offset
done