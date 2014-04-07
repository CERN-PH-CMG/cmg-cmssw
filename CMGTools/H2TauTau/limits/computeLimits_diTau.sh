for i in 125; do
echo $i
cd ./tauTau
combine -M Asymptotic --rMax 30 --minosAlgo stepping ./tauTau_SM1_mH$i.txt >> combine_1_$i.log
cd ..
done

