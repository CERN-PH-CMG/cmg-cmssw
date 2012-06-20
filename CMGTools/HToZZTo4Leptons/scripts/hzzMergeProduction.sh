
#!/bin/sh



cd $1
haddChunks.py .
#rm -rf *Chunk*

direc=`ls|grep -v Chunk`

for dir in $direc 
do
hzz4LPack.py  -s 1.0 $dir -f $2
done
cd ..

mkdir $1_out

mv $1/*.* $1_out/

echo 'Packed'