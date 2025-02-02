#!/bin/sh -e

mkdir -p ../build/external
cd ../build/external

if [ ! -d "./openHEVC" ] ; then
    git clone https://github.com/OpenHEVC/openHEVC.git
fi

cd openHEVC

git config --global user.name "Edward-Wu"
git config --global user.email "aimin_email@126.com"
git checkout ffmpeg_update
git am --whitespace=fix ../../../external/Update-buffer-operation-and-fix-stream-loop-coredump.patch
./configure --libdir=/usr/lib64 --disable-sdl2
make -j `nproc`
sudo make install
