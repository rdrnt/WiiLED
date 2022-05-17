# this script is in ROUGH shape
# delete the already existing boot.dol or Wiitest.elf file
rm -rf boot.dol Wiiled.elf
# run makefule
make
# rename .dol
mv Wiiled.dol boot.dol
# build to our wii using wiiload
./wiiload boot.dol
