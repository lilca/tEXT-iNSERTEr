sp=' '
fn=Makefile

if [ $# -lt 1 ]; then
	tar=.
else
	tar=$1
fi

rm $tar/$fn

all='all: '
for file in `find $tar -name "*.tir" -print`
do
	all=$all$sp${file%.*}
done
echo $all >> $tar/$fn
echo >> $tar/$fn
for file in `find $tar -name "*.tir" -print`
do
	tir $file -makefile -y >> $tar/$fn
done