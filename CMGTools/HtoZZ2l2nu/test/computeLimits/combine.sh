hadd -f higgsComb.root cardsComb/???/higgsCombineTest.Asymptotic*.root cardsComb/????/higgsCombineTest.Asymptotic*.root
root -l -b -q plotLimit.C++'("COUNTComb","higgsComb.root",  9 , 10.000 )'
