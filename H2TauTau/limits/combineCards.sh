for i in 110 115 120 125 130 135 140 145; do
echo $i
cd ./muTau
combineCards.py Name1=muTau_SM0_mH$i.txt Name2=muTau_SM1_mH$i.txt Name3=muTau_SM2_mH$i.txt > muTau_mH$i.txt
cd ..
done
