# Setting parameters for the GenStudies
# Pay enough care when you change this !

# Event classification

LTauCategory = {0 : 'vbf_tight',
                1 : 'vbf_loose',
                2 : '1jet_high_mediumhiggs',
                3 : '1jet_high_lowhiggs',
                4 : '1jet_medium',
                5 : '0jet_high',
                6 : '0jet_medium',
                7 : '0jet_low'}

EmuCategory = {0 : 'vbf_tight',
               1 : 'vbf_loose',
               2 : '1jet_high',
               3 : '1jet_low',
               4 : '0jet_high',
               5 : '0jet_low'}

TauTauCategory = {0 : 'vbf',
                  1 : '1jet_high_highhiggs',
                  2 : '1jet_high_mediumhiggs'}

LLCategory = {0 : 'vbf',
              1 : '1jet_high',
              2 : '1jet_low',
              3 : '0jet_high',
              4 : '0jet_low'}



DecayMode = {0 : ['Tautau', TauTauCategory],
             1 : ['ee', LLCategory],
             2 : ['mumu', LLCategory],
             3 : ['emu', EmuCategory],
             4 : ['mue', EmuCategory],
             5 : ['eleTau', LTauCategory],
             6 : ['muTau', LTauCategory],
             7 : ['Tauele', LTauCategory],
             8 : ['Taumu', LTauCategory]}


def vbfCategorySelection(decaymode, vbf, dilepton, jets):

#    print 'vbf : mjj, deta, higgs pT, cjv', vbf.mjj, vbf.deta, dilepton.p4().pt(), len(vbf.centralJets)
            
    if decaymode in [3,4,5,6,7,8] : # Lepton + Tau, emu

        if vbf.mjj > 700. and vbf.deta > 4. and dilepton.p4().pt() > 100 and len(vbf.centralJets)==0:
            return 'vbf_tight', 0
        elif vbf.mjj > 500 and vbf.deta > 3.5 and len(vbf.centralJets)==0:
            return 'vbf_loose', 1
        else:
            return 'None',-1

    elif decaymode in [0]: # tautau

        if vbf.mjj > 500. and vbf.deta > 3.5 and dilepton.p4().pt() > 100 and len(vbf.centralJets)==0:
            return 'vbf',0
        else:
            return 'None',-1

    elif decaymode in [1,2]: # ee, mm
        if len(vbf.centralJets)==0 and jets[0].eta()*jets[1].eta() < 0:
            return 'vbf',0
        else:
            return 'None',-1
    else:
        print 'Unexpected Decay Mode !'


def BoostCategorySelection(decaymode, higgs, dilepton):

#    print 'Boost : vis1, vis2, higgs pT, met', higgs.leg1_vis(), higgs.leg2_vis(), dilepton.p4().pt(), higgs.met().p4().pt()
            
    if decaymode in [5,7] : # e + Tau

        taupt = higgs.leg2_vis()
        if(decaymode==7): taupt = higgs.leg1_vis()
            
        if(taupt > 45 and dilepton.p4().pt() > 100 and higgs.met().p4().pt() > 30):
            return '1jet_high_mediumhiggs',2,taupt
        elif(taupt > 45 and dilepton.p4().pt() < 100 and higgs.met().p4().pt() > 30):
            return '1jet_high_lowhiggs',3,taupt
        elif(taupt > 30 and taupt < 45 and higgs.met().p4().pt() > 30):
            return '1jet_medium',4,taupt
        else:
            return 'None',-1, taupt

    elif decaymode in [6,8]: # mu + Tau

        taupt = higgs.leg2_vis()
        if(decaymode==8): taupt = higgs.leg1_vis()
            
        if(taupt > 45 and dilepton.p4().pt() > 100):
            return '1jet_high_mediumhiggs',2,taupt
        elif(taupt > 45 and dilepton.p4().pt() < 100):
            return '1jet_high_lowhiggs',3,taupt
        elif(taupt > 30 and taupt < 45):
            return '1jet_medium',4,taupt
        else:
            return 'None',-1,taupt

    elif decaymode in [3,4]: # emu

        mupt = higgs.leg2_vis()
        if(decaymode==4): mupt = higgs.leg1_vis()

        if(mupt > 35):
            return '1jet_high',2,mupt
        elif(mupt < 35):
            return '1jet_low',3,mupt
        else:
            return 'None',-1,mupt

    elif decaymode in [0]: # Tau + Tau

        taupt = higgs.leg1_vis() # sorted by order, so just use first one
            
        if(taupt > 45 and dilepton.p4().pt() > 170):
            return '1jet_high_highhiggs',1,taupt
        elif(taupt > 45 and dilepton.p4().pt() > 100):
            return '1jet_high_mediumhiggs',2,taupt
        else:
            return 'None',-1,taupt

    elif decaymode in [1,2]: # ee, mm

        leppt = higgs.leg1_vis() # sorted by order, so just use first one
            
        if(leppt > 35):
            return '1jet_high',1,leppt
        elif(leppt < 35):
            return '1jet_low',2,leppt
        else:
            return 'None',-1,leppt

    else:
        print 'Unexpected Decay Mode !'        



def NoJetCategorySelection(decaymode, higgs, dilepton):

#    print '0-jet : vis1, vis2', higgs.leg1_vis(), higgs.leg2_vis()
    
    if decaymode in [5,6,7,8] : # lepton + Tau

        taupt = higgs.leg2_vis()
        if(decaymode in [7,8]): taupt = higgs.leg1_vis()
            
        if(taupt > 45):
            return '0jet_high',5
        elif(taupt > 30 and taupt < 45):
            return '0jet_medium',6
        elif(taupt < 30):
            return '0jet_low',7
        else:
            return 'None',-1

    elif decaymode in [3,4]: # emu

        mupt = higgs.leg2_vis()
        if(decaymode==4): mupt = higgs.leg1_vis()

        if(mupt > 35):
            return '0jet_high',4
        elif(mupt < 35):
            return '0jet_low',5
        else:
            return 'None',-1

    elif decaymode in [0]: # Tau + Tau
        return 'None',-1

    elif decaymode in [1,2]: # ee, mm

        leppt = higgs.leg1_vis() # sorted by order, so just use first one
            
        if(leppt > 35):
            return '0jet_high',3
        elif(leppt < 35):
            return '0jet_low',4
        else:
            return 'None',-1

    else:
        print 'Unexpected Decay Mode !'        
