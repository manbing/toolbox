#!/bin/sh

. /usr/share/libubox/jshn.sh
. /lib/functions/network.sh

echo "$1"

network_get_ipaddr ip $1
echo "ip is $ip"

network_get_subnet subnet $1
echo "subnetis $subnet"

network_get_gateway gw $1
echo "gateway is $gw"

if network_get_dnsserver srvs $1; then
	for srv in ${srvs}; do
		dns_servers="$dns_servers $srv"
	done
fi
echo "DNS is $dns_servers"
