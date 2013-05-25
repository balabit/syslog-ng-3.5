#!/bin/sh

lcov --capture --directory `pwd` --compat-libtool --output-file coverage.info
genhtml coverage.info --output-directory coverage.html
firefox  coverage.html/index.html
