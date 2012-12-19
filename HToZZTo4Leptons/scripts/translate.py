import ROOT

channels=['4e','4mu','2e2mu']
translation={'4e':'EleEle','4mu':'MuMu','2e2mu':'MuEle'}
f=ROOT.TFile('template_MELA.root',"RECREATE")

for channel in channels:
    ff=ROOT.TFile('Dsignal_'+channel+'.root')
    h=ff.Get('h_mzzD')
    f.cd()

    h.SetName(translation[channel]+'_ggH')
    h.Write()
    
    h.SetName(translation[channel]+'_qqH')
    h.Write()

    h.SetName(translation[channel]+'_ZH')
    h.Write()

    h.SetName(translation[channel]+'_WH')
    h.Write()

    h.SetName(translation[channel]+'_ttH')
    h.Write()


    h=ff.Get('h_mzzD_up')
    f.cd()
    h.SetName(translation[channel]+'_ggH_up')
    h.Write()

    h.SetName(translation[channel]+'_qqH_up')
    h.Write()

    h.SetName(translation[channel]+'_ZH_up')
    h.Write()

    h.SetName(translation[channel]+'_WH_up')
    h.Write()

    h.SetName(translation[channel]+'_ttH_up')
    h.Write()

    h=ff.Get('h_mzzD_dn')
    f.cd()
    h.SetName(translation[channel]+'_ggH_dn')
    h.Write()

    h.SetName(translation[channel]+'_qqH_dn')
    h.Write()

    h.SetName(translation[channel]+'_ZH_dn')
    h.Write()

    h.SetName(translation[channel]+'_WH_dn')
    h.Write()

    h.SetName(translation[channel]+'_ttH_dn')
    h.Write()

    ff.Close()

    ff=ROOT.TFile('Dbackground_qqZZ_'+channel+'.root')
    h=ff.Get('h_mzzD')
    f.cd()
    h.SetName(translation[channel]+'_QQZZ')
    h.Write()

    h.SetName(translation[channel]+'_FAKES')
    h.Write()

    h=ff.Get('h_mzzD_up')
    f.cd()
    h.SetName(translation[channel]+'_QQZZ_up')
    h.Write()

    h.SetName(translation[channel]+'_FAKES_up')
    h.Write()

    h=ff.Get('h_mzzD_dn')
    f.cd()
    h.SetName(translation[channel]+'_QQZZ_dn')
    h.Write()

    h.SetName(translation[channel]+'_FAKES_dn')
    h.Write()

    ff.Close()


    ff=ROOT.TFile('Dbackground_ggZZ_'+channel+'.root')
    h=ff.Get('h_mzzD')
    f.cd()
    h.SetName(translation[channel]+'_GGZZ')
    h.Write()

    h=ff.Get('h_mzzD_up')
    f.cd()
    h.SetName(translation[channel]+'_GGZZ_up')
    h.Write()

    h=ff.Get('h_mzzD_dn')
    f.cd()
    h.SetName(translation[channel]+'_GGZZ_dn')
    h.Write()

    ff.Close()

f.Close()
    

    

            

