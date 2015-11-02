DIR=${1-TrashSync}
python ../python/scripts/eventDumper.py $DIR/HZZ4L/fourLeptonTreeProducer/tree.root -f '{run}:{lumi}:{evt}:{zz1_mass:.2f}:{zz1_z1_mass:.2f}:{zz1_z2_mass:.2f}:{zz1_D_bkg_kin:.3f}:{zz1_D_bkg:.3f}:{zz1_D_gg:.3f}:{zz1_D_HJJ_VBF:.3f}:{zz1_D_0m:.3f}:{nJet30ZZ:d}:{Jet1_pt_zs:.2f}:{Jet2_pt_zs:.2f}:{category}:{fabs(zz1_z1_l1_pdgId):.0f}:{fabs(zz1_z2_l1_pdgId):.0f}' -C 'zz1_mass>70 && HLT_Signal ' 2> /dev/null | grep -v '^Processed' | sort -t: -k3,3 -n | sort -t: -k2,2 -n --stable | sort -t: -k1,1 -n --stable > mydump2-ext.txt
cut -d: -f1-15  mydump2-ext.txt > mydump2.txt
cut -d: -f16-17 mydump2-ext.txt | wc -l
cut -d: -f16-17 mydump2-ext.txt | grep '11:11' | wc -l
cut -d: -f16-17 mydump2-ext.txt | grep '13:13' | wc -l
cut -d: -f16-17 mydump2-ext.txt | grep '13:11\|11:13' | wc -l
