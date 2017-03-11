#!/bin/bash
( cd cougar_bluetooth_lib && node-gyp build ) && bozon start
