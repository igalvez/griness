if [ "$#" -lt 2 ]
then
   echo "usage: extract_log.sh <opcode name> <log name without extension>"
   exit 1;
fi 
l1=`grep $1 $2.log -n | grep 3A0 | sed '1q;d' | cut -d":" -f1`
echo "$l1"
l2=`grep $1 $2.log -n | grep 3A0 | tail -1 | cut -d":" -f1`
echo "$l2"
sed -n "$l1,$l2 p;$(($l2+1)) q" $2.log > $2-$1-all.log
