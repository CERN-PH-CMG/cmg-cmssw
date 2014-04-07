for i in 110 115 120 125 130 135 140 145; do
echo $i
cd ./muTau
combine -M Asymptotic --rMax 40 --minosAlgo stepping ./muTau_SM0_mH$i.txt >> combine_0_$i.log
combine -M Asymptotic --rMax 30 --minosAlgo stepping ./muTau_SM1_mH$i.txt >> combine_1_$i.log
combine -M Asymptotic --rMax 30 --minosAlgo stepping ./muTau_SM2_mH$i.txt >> combine_2_$i.log
combine -M Asymptotic --rMax 20 --minosAlgo stepping ./muTau_mH$i.txt >> combine_$i.log
cd ..
done

