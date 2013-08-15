
#!/bin/sh



cd $1
mkdir data
mv DoubleMu* data/
mv DoubleElectron* data/
mv MuEG* data/
cd data
find DoubleMu*/FourLeptonTreeProducer/*.root |xargs hadd DoubleMu.root
find DoubleElectron*/FourLeptonTreeProducer/*.root |xargs hadd DoubleElectron.root
find MuEG*/FourLeptonTreeProducer/*.root |xargs hadd MuEG.root
hadd DATA.root DoubleMu.root DoubleElectron.root MuEG.root
rm DoubleMu.root DoubleElectron.root MuEG.root
cd ..

echo 'DATA IS DONE'

haddChunks.py .

#rm -rf *Chunk*

direc=`ls|grep -v Chunk|grep -v DoubleMu|grep -v DoubleEle| grep -v MuEG|grep -v data`

for dir in $direc 
do
hzz4LPack.py  -s 1.0 $dir -f $2
done
cd ..

mkdir $1_out

mv $1/*.* $1_out/
mv $1/data/DATA.root  $1_out/

echo 'Packed'