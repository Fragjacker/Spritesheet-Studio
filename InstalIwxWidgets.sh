#!/bin/bash -e
myRepo=$(pwd)
CMAKE_GENERATOR_OPTIONS=-G"Visual Studio 17 2022"
#CMAKE_GENERATOR_OPTIONS=-G"Visual Studio 15 2017 Win64"
#CMAKE_GENERATOR_OPTIONS=(-G"Visual Studio 16 2019" -A x64)  # CMake 3.14+ is required
if [  ! -d "$myRepo/wxWidgets"  ]; then
    echo "cloning wxWidgets repository"
    git clone https://github.com/wxWidgets/wxWidgets.git --recurse-submodules
else
    cd wsWidgets
    git pull --rebase
    cd ..
fi
RepoSource=wxWidgets
mkdir -p wxWidgets
pushd wxWidgets
CMAKE_OPTIONS=(-DBUILD_SHARED_LIBS=OFF -DBUILD_WITH_STATIC_CRT=OFF)
set -x
cmake "${CMAKE_GENERATOR_OPTIONS[@]}" "${CMAKE_OPTIONS[@]}"  -DCMAKE_INSTALL_PREFIX="$myRepo"/wx_install -DwxBUILD_SHARED=OFF
echo "************************* $Source_DIR -->release"
cmake --build . --target install
popd