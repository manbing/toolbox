#!/bin/sh
uci delete runtime.test
uci commit runtime
sleep 1
uci set runtime.test=test
uci set runtime.test.init01=init01
uci set runtime.test.init02=init02
uci commit runtime

./ucimonkey -m &
cp ./ucimonkey ./ucimonkey2
./ucimonkey2 -s mikasa &
./ucimonkey2 -s zeke &
./ucimonkey2 -s grisha &

while true
do
	sleep 1
	if ! pidof ucimonkey
	then
		echo "Runtime config test done"
		uci show runtime.test
	fi
done
