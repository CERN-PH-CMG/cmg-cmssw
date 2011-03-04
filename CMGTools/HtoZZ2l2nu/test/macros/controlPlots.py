import sys
import ROOT
from DataFormats.FWLite import Events, Handle
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
from ROOT import getRunInfo
from genLevelUtils import getDecayTreeFrom, getDecayCounters


#open file
url='cmgTuple.root'
if(len(sys.argv)>1):url=sys.argv[1]
file = ROOT.TFile(url)

events = Events (file)
hcandHandle  = Handle ('std::vector<cmg::DiObject<cmg::DiObject<cmg::Muon,cmg::Muon>,cmg::BaseMET> >')
hcandLabel = ('cmgDiMuonPlusMET')

hgenCandHandle  = Handle ('std::vector<reco::GenParticle>')
hgenCandLabel = ('genHiggsFinalState')

#book histograms
muonPt_h = ROOT.TH1F("muon_pt", "#mu#mu;p_{T} [GeV/c]; Events", 100, 0.,300.)
met_h    = ROOT.TH1F("met",     "Missing transverse energy;#slash{E}_{T} [GeV/c]; Events", 100,  0.,300.)

# loop over events
for event in events:

    #reco level
    event.getByLabel(hcandLabel,hcandHandle)

    #fill the histos
    for hcand in hcandHandle.product() :
        muonPt_h.Fill( hcand.leg1().leg1().pt() )
        muonPt_h.Fill( hcand.leg1().leg2().pt() )
        met_h.Fill( hcand.leg2().pt() )

    #gen level
    event.getByLabel(hgenCandLabel,hgenCandHandle)
    mothers,daughters,finalstates=getDecayTreeFrom(hgenCandHandle,25,True)
    nmuons,nelecs,ntaus,nneutrinos,njets=getDecayCounters(mothers,daughters,finalstates)
    print ' muons=' + str(nmuons) + ' elecs=' + str(nelecs) + ' taus=' + str(ntaus) + ' nu=' + str(nneutrinos) + ' jets=' + str(njets)        

canvas = ROOT.TCanvas("c","c")
canvas.SetWindowSize(1000,500)
canvas.Divide(2,1)
canvas.cd(1)
muonPt_h.Draw("hist")
canvas.cd(2)
met_h.Draw("hist")


raw_input('Any key to quit...')

file.Close()
