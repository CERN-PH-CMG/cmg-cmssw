import ROOT

channels=['4e','4mu','2e2mu']
translation={'4e':'EleEle','4mu':'MuMu','2e2mu':'MuEle'}
f=ROOT.TFile('template_KD.root',"RECREATE")

for channel in channels:
    ff=ROOT.TFile('Dsignal_'+channel+'.root')
    h=ff.Get('h_mzzD')
    f.cd()

    h.SetName(translation[channel]+'_ggH_7TeV')
    h.Write()
    h.SetName(translation[channel]+'_ggH_8TeV')
    h.Write()
    
    h.SetName(translation[channel]+'_qqH_7TeV')
    h.Write()
    h.SetName(translation[channel]+'_qqH_8TeV')
    h.Write()

    h.SetName(translation[channel]+'_ZH_7TeV')
    h.Write()
    h.SetName(translation[channel]+'_ZH_8TeV')
    h.Write()

    h.SetName(translation[channel]+'_WH_7TeV')
    h.Write()
    h.SetName(translation[channel]+'_WH_8TeV')
    h.Write()

    h.SetName(translation[channel]+'_ttH_7TeV')
    h.Write()
    h.SetName(translation[channel]+'_ttH_8TeV')
    h.Write()
    ff.Close()

    ff=ROOT.TFile('Dbackground_qqZZ_'+channel+'.root')
    h=ff.Get('h_mzzD')
    f.cd()
    h.SetName(translation[channel]+'_QQZZ_7TeV')
    h.Write()
    h.SetName(translation[channel]+'_QQZZ_8TeV')
    h.Write()

    h.SetName(translation[channel]+'_FAKES_7TeV')
    h.Write()
    h.SetName(translation[channel]+'_FAKES_8TeV')
    h.Write()

    h=ff.Get('h_mzzD_up')

    h.SetName(translation[channel]+'_FAKES_7TeV_CR_Up')
    h.Write()
    h.SetName(translation[channel]+'_FAKES_8TeV_CR_Up')
    h.Write()

    h=ff.Get('h_mzzD_dn')
    h.SetName(translation[channel]+'_FAKES_7TeV_CR_Down')
    h.Write()
    h.SetName(translation[channel]+'_FAKES_8TeV_CR_Down')
    h.Write()


    ff=ROOT.TFile('Dbackground_ggZZ_'+channel+'.root')
    h=ff.Get('h_mzzD')
    f.cd()
    h.SetName(translation[channel]+'_GGZZ_7TeV')
    h.Write()
    h.SetName(translation[channel]+'_GGZZ_8TeV')
    h.Write()
    ff.Close()

f.Close()
    

    

            

