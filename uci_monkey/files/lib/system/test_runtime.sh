#!/bin/sh
uci delete uci_test.test
uci commit uci_test
sleep 1
uci set uci_test.test=test
uci set uci_test.test.init01=init01
uci set uci_test.test.init02=init02
uci commit uci_test

# cd /var/
# tftp -g -r libuci.so ...
LD_LIBRARY_PATH=/var/ ./ucimonkey -m &
cp ./ucimonkey ./ucimonkey2
LD_LIBRARY_PATH=/var/ ./ucimonkey2 -s mikasa &
LD_LIBRARY_PATH=/var/ ./ucimonkey2 -s zeke &
LD_LIBRARY_PATH=/var/ ./ucimonkey2 -s grisha &

while true
do
	sleep 1
	if ! pidof ucimonkey
	then
		echo "uci_test config test done"
		cat /etc/config/uci_test
        #uci show uci_test.test
	fi
done
