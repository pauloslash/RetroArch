#!/bin/bash
source 0_ENV
init(){
    ./retroarch -c retroarch1.cfg -L /home/pauloslash/.config/retroarch/cores/fbneo_libretro.so ${ROM_DIR}/kof98.zip -C ${IP} &
    instancia1=$!
    ./retroarch -c retroarch2.cfg -L /home/pauloslash/.config/retroarch/cores/fbneo_libretro.so ${ROM_DIR}/kof98.zip -C ${IP} &
    instancia2=$!
    wait $instancia1
    kill $instancia2
}

while true; do
    init
done
