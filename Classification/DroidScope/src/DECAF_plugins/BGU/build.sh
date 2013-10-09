#!/bin/bash

make $1
if [ -f bgu.so ]
then
  cp bgu.so ~/Android/AndroidSDK/sdk/DroidScope/plugins/bgu.so
fi
