#!/bin/sh
cd `dirname ${BASH_ARGV[0]}`
topdir=`pwd | xargs echo -n`
export PATH=$topdir/bin:$PATH
cd `cd -`
