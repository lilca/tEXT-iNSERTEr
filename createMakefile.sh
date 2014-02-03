sp=' '

if [ $# -lt 1 ]; then
	tar=.
else
	tar=$1
fi

rm $tar/Makefile.txt

all='all: '
for file in `find $tar -name "*.tir" -print`
do
	all=$all$sp$file
done
echo $all >> $tar/Makefile.txt
echo >> $tar/Makefile.txt
for file in `find $tar -name "*.tir" -print`
do
	tir $file -makefile -y >> $tar/Makefile.txt
done