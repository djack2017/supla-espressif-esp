#!/bin/sh

###
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
# 
# @author Przemyslaw Zygmunt przemek@supla.org
#
###

DEP_LIBS="-lssl"
NOSSL=0
SPI_MODE="DIO"

export PATH=/hdd2/Espressif/xtensa-lx106-elf/bin:$PATH
export COMPILE=gcc

case $1 in
   "wifisocket")
   ;;
   "wifisocket_x4")
   ;;
   "wifisocket_54")
   ;;
   "gate_module")
   ;;
   "gate_module_dht11")
   ;;
   "gate_module_dht22")
   ;;
   "gate_module_wroom")
      FLASH_SIZE="2048"
   ;;
   "gate_module2_wroom")
      FOTA=1
      FLASH_SIZE="2048"
   ;;
   "rs_module")
      FOTA=1
      FLASH_SIZE="2048"
   ;;
   "lightswitch_x2")
     FLASH_SIZE="4096"
   ;;
   "lightswitch_x2_54")
     FLASH_SIZE="4096"
   ;;
   "lightswitch_x2_DHT11")
     FLASH_SIZE="4096"
   ;;
   "lightswitch_x2_54_DHT11")
     FLASH_SIZE="4096"
   ;;
   "lightswitch_x2_DHT22")
     FLASH_SIZE="4096"
   ;;
   "lightswitch_x2_54_DHT22")
     FLASH_SIZE="4096"
   ;;
   "sonoff")
      FOTA=1
   ;;
   "sonoff_socket")
      FOTA=1
   ;;
   "sonoff_touch")
      SPI_MODE="DOUT"
      FOTA=1
   ;;
   "sonoff_touch_dual")
      SPI_MODE="DOUT"
      FOTA=1
   ;;
   "sonoff_dual")
      FOTA=1
   ;;
   "sonoff_th16")
      FOTA=1
   ;;
   "sonoff_th10")
      FOTA=1
   ;;
   "sonoff_ds18b20")
      FOTA=1
   ;;
   "sonoff_pow_R2")
      FOTA=1
   ;;
   "EgyIOT")
     DEP_LIBS="-lpwm"
     NOSSL=1
   ;;
   "dimmer")
     DEP_LIBS="-lpwm"
     NOSSL=1
   ;;
   "zam_row_01")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "zam_row_02")
      FLASH_SIZE="2048"
      FOTA=1
      EXTRA_CCFLAGS="-DSRPC_QUEUE_SIZE=4 -DSRPC_QUEUE_MIN_ALLOC_COUNT=4"
   ;;
   "zam_row_04")
      FLASH_SIZE="2048"
      FOTA=1
      EXTRA_CCFLAGS="-DSRPC_QUEUE_SIZE=4 -DSRPC_QUEUE_MIN_ALLOC_COUNT=4"
   ;;
   "zam_row_01_tester")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "zam_srw_01_tester")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "zam_row_07")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "zam_row_07_demo")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "zam_srw_01")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "zam_srw_02")
      FLASH_SIZE="2048"
      FOTA=1
      EXTRA_CCFLAGS="-DSRPC_QUEUE_SIZE=4 -DSRPC_QUEUE_MIN_ALLOC_COUNT=4"
   ;;
   "n_srw_01")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "k_srw_01")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "zam_pnw_01")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "zam_srw_01_tester")
      #FLASH_SIZE="2048"
      #FOTA=1
   ;;
   "zam_srw_03")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "zam_sbw_01")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "zam_sbw_02")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "n_sbp_01")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "k_sbw_01")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "zam_slw_01")
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "zam_slw_02")
      FLASH_SIZE="2048"
      FOTA=1
      EXTRA_CCFLAGS="-DSRPC_QUEUE_SIZE=4 -DSRPC_QUEUE_MIN_ALLOC_COUNT=4"
   ;;
   "zam_mew_01") 
      FLASH_SIZE="2048"
      FOTA=1
   ;;
   "rgbw_wroom")
      FLASH_SIZE="2048"
      DEP_LIBS="-lpwm -lssl"
   ;;
   "h801")
     DEP_LIBS="-lpwm -lssl"
     FOTA=1
   ;;
   "impulse_counter")
     FLASH_SIZE="2048"
     FOTA=1
   ;;
   "vl_dimmer")
     FLASH_SIZE="2048"
     FOTA=1
     EXTRA_CCFLAGS="-DSRPC_QUEUE_SIZE=4 -DSRPC_QUEUE_MIN_ALLOC_COUNT=4 -DESP8266_LOG_DISABLED=1"
   ;;
   "hp_homeplus") 
     FLASH_SIZE="2048"
     FOTA=1
     EXTRA_CCFLAGS="-DSRPC_QUEUE_SIZE=4 -DSRPC_QUEUE_MIN_ALLOC_COUNT=4 -DESP8266_LOG_DISABLED=1"
   ;;
	
   *)
   echo "Usage:"
   echo "       build.sh BOARD_TYPE";
   echo "--------------------------";
   echo " Board types:             ";
   echo "              wifisocket  ";
   echo "              wifisocket_x4";
   echo "              wifisocket_54";
   echo "              gate_module";
   echo "              gate_module_dht11";
   echo "              gate_module_dht22";
   echo "              gate_module_wroom";
   echo "              gate_module2_wroom";
   echo "              rs_module";
   echo "              sonoff";
   echo "              sonoff_ds18b20";
   echo "              sonoff_touch";
   echo "              sonoff_dual";
   echo "              sonoff_socket";
   echo "              sonoff_th10";
   echo "              sonoff_th16";
   echo "              sonoff_dual";
   echo "              sonoff_pow_R2";
   echo "              EgyIOT";
   echo "              dimmer";
   echo "              rgbw_wroom";
   echo "              h801";
   echo "              lightswitch_x2";
   echo "              lightswitch_x2_54";
   echo "              lightswitch_x2_DHT11";
   echo "              lightswitch_x2_54_DHT11";
   echo "              lightswitch_x2_DHT22";
   echo "              lightswitch_x2_54_DHT22";
   echo "              impulse_counter";
   echo 
   echo
   exit;
   ;;
   
esac 

CFG_SECTOR=0x3C

case $FLASH_SIZE in
   "512")
    "512 flash size is not supported"
    exit 0
   ;;
   "2048")
     SPI_SIZE_MAP=3
   ;;
   "4096")
     SPI_SIZE_MAP=4
   ;;
   *)
     FLASH_SIZE="1024"
     SPI_SIZE_MAP=2
   ;;
esac

OUTDIR=../firmware
export SDK_PATH=/hdd2/Espressif/ESP8266_NONOS_SDK154
export BIN_PATH=/hdd2/Espressif/ESP8266_BIN154
LD_DIR=sdk154

#export SDK_PATH=/hdd2/Espressif/ESP8266_NONOS_SDK210
#export BIN_PATH=/hdd2/Espressif/ESP8266_NONOS_SDK210
#LD_DIR=sdk210


make clean

BOARD_NAME=$1

if [ "$NOSSL" -eq 1 ]; then
  EXTRA_CCFLAGS="${EXTRA_CCFLAGS} -DNOSSL=1"
  BOARD_NAME="$1"_nossl
else
  EXTRA_CCFLAGS="${EXTRA_CCFLAGS} -DNOSSL=0"
fi

[ -e $OUTDIR ] || mkdir $OUTDIR

if [ "$FOTA" -eq 1 ]; then

  APP=1

  if [ "user2" = "$2" ]; then
   APP=2
  fi

  case $FLASH_SIZE in
      "1024")
       CFG_SECTOR=0x7C
       ;;
      "2048")
        SPI_SIZE_MAP=5
        CFG_SECTOR=0xFC
      ;;
      "4096")
        SPI_SIZE_MAP=6
        CFG_SECTOR=0xFC
      ;;
  esac

   make SUPLA_DEP_LIBS="$DEP_LIBS" FOTA="$FOTA" BOARD=$1 CFG_SECTOR="$CFG_SECTOR" BOOT=new APP="$APP" SPI_SPEED=40 SPI_MODE="$SPI_MODE" SPI_SIZE_MAP="$SPI_SIZE_MAP" __EXTRA_CCFLAGS="$EXTRA_CCFLAGS" && \
   cp $BIN_PATH/upgrade/user"$APP"."$FLASH_SIZE".new."$SPI_SIZE_MAP".bin "$OUTDIR"/"$BOARD_NAME"_user"$APP"."$FLASH_SIZE"_"$SPI_MODE".new."$SPI_SIZE_MAP".bin && \
   cp $SDK_PATH/bin/boot_v1.5.bin $OUTDIR/boot_v1.5.bin

else

   cp ./ld/"$LD_DIR"/"$FLASH_SIZE"_eagle.app.v6.ld $SDK_PATH/ld/eagle.app.v6.ld || exit 1

   make SUPLA_DEP_LIBS="$DEP_LIBS" BOARD=$1 CFG_SECTOR=$CFG_SECTOR BOOT=new APP=0 SPI_SPEED=40 SPI_MODE="$SPI_MODE" SPI_SIZE_MAP="$SPI_SIZE_MAP" __EXTRA_CCFLAGS="$EXTRA_CCFLAGS" && \
   cp $BIN_PATH/eagle.flash.bin $OUTDIR/"$BOARD_NAME"_"$FLASH_SIZE"_"$SPI_MODE"_eagle.flash.bin && \
   cp $BIN_PATH/eagle.irom0text.bin $OUTDIR/"$BOARD_NAME"_"$FLASH_SIZE"_"$SPI_MODE"_eagle.irom0text.bin &&
   
   exit 0
fi

exit 1
