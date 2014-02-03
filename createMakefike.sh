rm Makefile.txt
for file in `find . -name "*.tir" -print`
do
	tir $file -makefile -y >> Makefile.txt
done