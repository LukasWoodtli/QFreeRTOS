#!/usr/bin/env bash

sudo add-apt-repository --yes ppa:ubuntu-sdk-team/ppa
sudo apt-get update -qq
sudo apt-get install graphviz doxygen mscgen
sudo apt-get install qtbase5-dev
sudo apt-get install qt5-default qttools5-dev-tools
sudo apt-get -q install build-essential
