#!/bin/sh
for x in $(find src/ -name '*.cpp' -o -name '*.h'); do clang-format -style=file -i $x; done
