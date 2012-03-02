echo "May10"
lumiCalc2.py -i finalTauPlusXMay.txt recorded -hltpath HLT_IsoMu12_LooseIsoPFTau10_v4 | grep HLT_IsoMu12_LooseIsoPFTau10_v4

echo "PRv4"
lumiCalc2.py -i finalTauPlusXv4.txt recorded -hltpath  HLT_IsoMu15_LooseIsoPFTau15_v2 | grep HLT_IsoMu15_LooseIsoPFTau15_v2
lumiCalc2.py -i finalTauPlusXv4.txt recorded -hltpath  HLT_IsoMu15_LooseIsoPFTau15_v4 | grep HLT_IsoMu15_LooseIsoPFTau15_v4
lumiCalc2.py -i finalTauPlusXv4.txt recorded -hltpath  HLT_IsoMu15_LooseIsoPFTau15_v5 | grep HLT_IsoMu15_LooseIsoPFTau15_v5
lumiCalc2.py -i finalTauPlusXv4.txt recorded -hltpath  HLT_IsoMu15_LooseIsoPFTau15_v6 | grep HLT_IsoMu15_LooseIsoPFTau15_v6

echo "Aug5"
lumiCalc2.py -i finalTauPlusXAug.txt recorded -hltpath  HLT_IsoMu15_LooseIsoPFTau15_v8 | grep HLT_IsoMu15_LooseIsoPFTau15_v8

echo "Oct3"
lumiCalc2.py -i finalTauPlusXv6.txt recorded -hltpath  HLT_IsoMu15_LooseIsoPFTau15_v8 | grep HLT_IsoMu15_LooseIsoPFTau15_v8
lumiCalc2.py -i finalTauPlusXv6.txt recorded -hltpath  HLT_IsoMu15_LooseIsoPFTau15_v9 | grep HLT_IsoMu15_LooseIsoPFTau15_v9

echo "2011B"
#lumiCalc2.py -i finalTauPlusX11B.txt recorded -hltpath  HLT_IsoMu15_LooseIsoPFTau15_v9 | grep HLT_IsoMu15_LooseIsoPFTau15_v9
lumiCalc2.py -i finalTauPlusX11B.txt recorded -hltpath  HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1 | grep HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1
lumiCalc2.py -i finalTauPlusX11B.txt recorded -hltpath  HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5 | grep HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5
lumiCalc2.py -i finalTauPlusX11B.txt recorded -hltpath  HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6 | grep HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6

