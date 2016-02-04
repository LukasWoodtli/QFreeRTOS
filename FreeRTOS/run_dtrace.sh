#!/usr/bin/env bash

sudo dtrace -s dtrace_test.d -o dtrace_out.txt -c Demo/build-QFreeRTOS-Desktop_Qt_5_5_0_clang_64bit-Debug/QFreeRTOS
