test -f $1 || exit 1
python ../python/scripts/eventDumper.py $1 -f '{run}:{lumi}:{evt}:{zz1_mass:.2f}:{zz1_z1_mass:.2f}:{zz1_z2_mass:.2f}:{zz1_D_bkg_kin:.3f}:{zz1_D_bkg:.3f}:{zz1_D_gg:.3f}:{zz1_D_HJJ_VBF:.3f}:{zz1_D_0m:.3f}:{nJet30ZZ:d}:{Jet1_pt_zs:.2f}:{Jet2_pt_zs:.2f}:{category}:{fabs(zz1_z1_l1_pdgId):.0f}:{fabs(zz1_z2_l1_pdgId):.0f}' -C 'zz1_mass>70 && (zz1_mass < 110 || zz1_mass > 150) && HLT_Signal' 2> /dev/null | grep -v '^Processed' | sort -t: -k3,3 -n | sort -t: -k2,2 -n --stable | sort -t: -k1,1 -n --stable > mydata2.txt
for M in high low; do
echo $M
cut="< 110"; [[ "$M" == "high" ]] && cut="> 150";
awk -F: "{if (\$4 $cut) print}" mydata2.txt | cut -d: -f1-15 > mydata-${M}.txt
awk -F: "{if (\$4 $cut) print}" mydata2.txt | cut -d: -f16-17 | sort | uniq -c
echo
done
