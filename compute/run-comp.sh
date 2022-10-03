SLEEPTIME=(6 4 2 0)

CMULT="./add $1 runtime-config-$2 private-$1.pem 1 1 fakeinput out "

MULTSIZE=(1 10 100 1000 10000 100000)
MULTREP=(1000 1000 1000 100 100 100)


for i in {0..5}
do
    for k in {1..2}
    do
        echo "COMMAND: $CMULT ${MULTSIZE[$i]} ${MULTREP[$i]}"
        eval "$CMULT ${MULTSIZE[$i]} ${MULTREP[$i]}"
        sleep ${SLEEPTIME[$1]}
		if [ "$1" = 1 ]
		then
		    touch flag.txt
		fi
   done
done

CEQZ="./eqz $1 runtime-config-$2 private-$1.pem 1 1 fakeinput out "

for i in {0..5}
do
    for k in {1..2}
    do
        echo "COMMAND: $CEQZ ${MULTSIZE[$i]} ${MULTREP[$i]}"
        eval "$CEQZ ${MULTSIZE[$i]} ${MULTREP[$i]}"
        sleep ${SLEEPTIME[$1]}
		if [ "$1" = 1 ]
		then
		    touch flag.txt
		fi
   done
done

CLTZ="./ltz $1 runtime-config-$2 private-$1.pem 1 1 fakeinput out "

for i in {0..6}
do
    for k in {1..2}
    do
        echo "COMMAND: $CLTZ ${MULTSIZE[$i]} ${MULTREP[$i]}"
        eval "$CLTZ ${MULTSIZE[$i]} ${MULTREP[$i]}"
        sleep ${SLEEPTIME[$1]}
		if [ "$1" = 1 ]
		then
		    touch flag.txt
		fi
   done
done

CBITOP="./bitop $1 runtime-config-$2 private-$1.pem 1 1 fakeinput out "

for i in {0..6}
do
    for k in {1..2}
    do
        echo "COMMAND: $CBITOP ${MULTSIZE[$i]} ${MULTREP[$i]}"
        eval "$CBITOP ${MULTSIZE[$i]} ${MULTREP[$i]}"
        sleep ${SLEEPTIME[$1]}
		if [ "$1" = 1 ]
		then
		    touch flag.txt
		fi
   done
done
