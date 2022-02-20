#!/bin/bash

function usage() {
    echo "Usage: bazel run //linter:clang-format"
    exit 1
}

CMD="clang-format"

if [[ -z "$BUILD_WORKSPACE_DIRECTORY" ]]; then
    echo "Use bazel if you want to run the script"
    usage
fi

if ! [[ -x "$(command -v $CMD)" ]]; then
    echo "$CMD not found, you can install it with sudo apt install clang-format"
fi

pushd "${BUILD_WORKSPACE_DIRECTORY}" &>/dev/null || exit 1
find . \
    \( -name '*.c' \
    -o -name '*.cc' \
    -o -name '*.cpp' \
    -o -name '*.h' \
    -o -name '*.hh' \
    -o -name '*.hpp' \) \
    -exec "${CMD}" -i '{}' \;
popd &>/dev/null || exit 1
