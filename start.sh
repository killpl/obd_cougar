#!/bin/bash
( cd cougar_bluetooth_lib && node-gyp build )
( cd cougar_lib && node-gyp build )
bozon start
