if [ $# -eq 0 ]
then
    echo "Usage: compile_test.sh <filename>"
    exit 1;
fi 

ca65 -I common -o $1.o $1.s
ld65 -C nes.cfg $1.o -o $1.nes
mv $1.nes ../rom_singles/.


