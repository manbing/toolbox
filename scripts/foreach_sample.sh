#!/bin/sh

. /lib/functions.sh

handler() {
	config_get $1 $2
}

config_load network
config_foreach handler interface device
