test -f $1 || exit 1
for X in SS 2P2F 3P1F; do 
    echo -n "${X}: "
    #CUT="zz${X}1_mass>70"
    #[[ "$X" == "2P2F" ]] && CUT="zz${X}1_mass>70 && zz1_mass<0 && zz3P1F1_mass<0"
    #[[ "$X" == "3P1F" ]] && CUT="zz${X}1_mass>70 && zz1_mass<0 && zz3P1F1_mass<0"
    python ../python/scripts/eventDumper.py --type zz${X} $1 -f "{run}:{lumi}:{evt}:{zz${X}1_mass:.2f}:{zz${X}1_z1_mass:.2f}:{zz${X}1_z2_mass:.2f}:{zz${X}1_D_bkg_kin:.3f}:{zz${X}1_D_bkg:.3f}:{zz${X}1_D_gg:.3f}:{zz${X}1_D_HJJ_VBF:.3f}:{zz${X}1_D_0m:.3f}:{nJet30ZZ:d}:{Jet1_pt_zs:.2f}:{Jet2_pt_zs:.2f}:{category}:{fabs(zz${X}1_z1_l1_pdgId):.0f}:{fabs(zz${X}1_z2_l1_pdgId):.0f}" -C "zz${X}1_mass>70 && HLT_Signal " 2> /dev/null | grep -v '^Processed' | sort -t: -k3,3 -n | sort -t: -k2,2 -n --stable | sort -t: -k1,1 -n --stable | tee mydump2-CR_${X}-ext.txt | wc -l 
    cut -d: -f1-15 mydump2-CR_${X}-ext.txt > mydump2-CR_${X}.txt
    #python ../python/scripts/eventDumper.py --jet-cleaning=event --type zz${X} $1 -f "{run}:{lumi}:{evt}:{zz${X}1_mass:.2f}:{zz${X}1_z1_mass:.2f}:{zz${X}1_z2_mass:.2f}:{zz${X}1_D_bkg_kin:.3f}:{zz${X}1_D_bkg:.3f}:{zz${X}1_D_gg:.3f}:{zz${X}1_D_0m:.3f}:{nJet30ZZ:d}:{Jet1_pt_zs:.2f}:{Jet2_pt_zs:.2f}:{category}" -C "zz${X}1_mass>70 && HLT_Signal " 2> /dev/null | grep -v '^Processed' | sort -t: -k3,3 -n | sort -t: -k2,2 -n --stable | sort -t: -k1,1 -n --stable > mydump2-CR_${X}-evt.txt 
done
