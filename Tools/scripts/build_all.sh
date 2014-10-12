#!/bin/bash
# useful script to test all the different build types that we support.
# This helps when doing large merges
# Andrew Tridgell, November 2011

. config.mk

set -e
set -x

INCREMENTAL=0
while getopts "i" opt; do
    case "$opt" in
        i)  INCREMENTAL=1
            ;;
        *)
            echo "BAD ARGUMENTS"
            exit -1
            ;;
    esac
done

echo "Testing ArduPlane build"
pushd ArduPlane
for b in all apm2 apm2beta apm1-hil apm1-hilsensors apm2-hil apm2-hilsensors sitl sitl-mount linux apm2-obc; do
    pwd
    if [ $INCREMENTAL -eq 0 ]
    then
        make clean
    fi
    make $b -j4
done
popd

echo "Testing ArduCopter build"
pushd ArduCopter

# for b in all apm2 apm1-hil apm2-hil sitl apm2-heli linux; do
# px4-v2 
for b in all apm2 apm1-hil apm2-hil sitl apm2-heli linux; do
    pwd
    if [ $INCREMENTAL -eq 0 ]
    then
        make clean
    fi
    make $b -j4
done
popd


echo "Testing APMRover build"
pushd APMrover2
for b in all apm2 sitl apm2-hil linux; do
    pwd
    if [ $INCREMENTAL -eq 0 ]
    then
        make clean
    fi
    make $b -j4
done
popd

echo "Testing AntennaTracker build"
pushd AntennaTracker
for b in apm2 sitl; do
    pwd
    if [ $INCREMENTAL -eq 0 ]
    then
       make clean
    fi
    make $b -j4
done
popd

echo "Testing build of examples"
examples="Tools/VARTest Tools/CPUInfo"
for d in $examples; do
    pushd $d
    if [ $INCREMENTAL -eq 0 ]
    then
        make clean
    fi
    make apm2 -j4
    if [ $INCREMENTAL -eq 0 ]
    then
        make clean
    fi
    make sitl -j4
    popd
done


test -d ../libmaple && {
echo "Testing flymaple build"
for d in ArduPlane ArduCopter APMrover2; do
    pushd $d
    if [ $INCREMENTAL -eq 0 ]
    then
        make clean
    fi
    make flymaple -j4
    popd
 done
}

pushd Tools/Replay
if [ $INCREMENTAL -eq 0 ]
then
    make clean
fi
make linux -j4
popd

test -n "$PX4_ROOT" && test -d "$PX4_ROOT" && {
    ./Tools/scripts/build_all_px4.sh
}

test -n "$VRBRAIN_ROOT" && test -d "$VRBRAIN_ROOT" && {
     ./Tools/scripts/build_all_vrbrain.sh
}

exit 0
