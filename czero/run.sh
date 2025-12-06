#!/bin/sh

make
kldunload -v ./alexzero.ko
kldload -v ./alexzero.ko
