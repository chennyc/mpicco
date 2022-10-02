COMMAND="../compiler/bin/picco-seed runtime-config-$1 u_303"
echo "COMMAND: $COMMAND"
eval "$COMMAND"
while true
do
    if [ -a "flag.txt" ]; 
	then
		eval "rm flag.txt"
		sleep 1
		eval "$COMMAND"
    else
		echo "sleep"
		sleep 10
    fi
done