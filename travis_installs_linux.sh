#!/usr/bin/env bash

lsb_release -a
sudo apt-add-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-add-repository -y ppa:beineri/opt-qt542
sudo apt-get -qq update
sudo apt-get -qq install g++-4.8 libc6-i386 qt54tools
export CXX="g++-4.8"
export CC="gcc-4.8"
