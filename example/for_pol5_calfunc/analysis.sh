#!/bin/sh

app="dsdana"

calfile="database_cc.txt"

function ana ()
{
    infile=$1
    outfile=${infile/".root"/"_detailed_hittree.root"}
    #outfile=${outfile/"eventtree"/"detailed_hittree"}
    
    $app << EOF
define_analysis
1
2
8
-1
modify_param
full ok
$infile
eventtree
$calfile
$outfile
hittree
analyze_data
read_data
-1
1000
exit
EOF
}

for data in $@
do
    ana $data
done
