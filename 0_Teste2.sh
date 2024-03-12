#!/bin/bash
source 0_ENV
./retroarch -c retroarch2.cfg -L /home/pauloslash/.config/retroarch/cores/fbneo_libretro.so ${ROM_DIR}/kof98.zip -C ${IP}
