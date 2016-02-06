#!/usr/bin/env bash
 
export DEBIAN_FRONTEND=noninteractive
sudo add-apt-repository --yes ppa:ubuntu-sdk-team/ppa
sudo apt-get update -qq
sudo apt-get -y install graphviz doxygen mscgen
sudo apt-get -y install qtbase5-dev
sudo apt-get -y install qt5-default qttools5-dev-tools
sudo apt-get -q install build-essential
