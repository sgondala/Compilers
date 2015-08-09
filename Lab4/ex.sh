#!/bin/bash

make
./out < test_temp
make run
./a.out
