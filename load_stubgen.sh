#!/usr/bin/env bash
curl -L -o artifact.zip "https://nightly.link/Fedorrracora/mathInterval/workflows/build-wheels/main/mathInterval.pyi.zip"
unzip -o artifact.zip
rm -f artifact.zip
mv mathInterval.pyi mathInterval/mathInterval.pyi
