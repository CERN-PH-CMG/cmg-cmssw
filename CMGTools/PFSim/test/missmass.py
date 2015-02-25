import sys
from CMGTools.RootTools.PyRoot import *

dir = sys.argv[1]

samples = {
    'ZH':dict(nGenEvents=20000., sigma=0.131),
    'WWH':dict(nGenEvents=5000., sigma=0.0267)
    }

flumi = 500. # 500 fb-1
plumi = flumi * 1e3

def loadSample(samples, sampleName):
    chain = Chain('PFSimTreeProducer', '/'.join([dir,sampleName,'PFSimTreeProducer','*.root']))
    print 'loading', chain.GetEntries()
    samples[sampleName]['tree'] = chain
    buildHist(samples, sampleName)


def buildHist(samples, sampleName):
    sam = samples[sampleName]
    tree = samples[sampleName]['tree']
    hnam ='hist_'+sampleName 
    hist = TH1F( hnam, hnam, 50, 0, 250)
    tree.Project(hnam, 'simMissMass', 'simVisMass>105 && simVisMass<135')
    xsec = sam['sigma']
    nev = sam['nGenEvents']
    hist.Scale(plumi*xsec/nev)
    hist.SetStats(0)
    hist.SetTitle(';m_{miss} (GeV);Events')
    samples[sampleName]['hist'] = hist

loadSample(samples, 'ZH')
loadSample(samples, 'WWH')

hzh = samples['ZH']['hist']
hwwh = samples['WWH']['hist']

sBlue.fillStyle = 0
sRed.fillStyle = 0

sBlue.formatHisto(hzh)
sRed.formatHisto(hwwh)

c = TCanvas("c", "", 600, 600)
formatPad(c)
hzh.Draw()
hwwh.Draw('same')

leg = TLegend(0.18, 0.71, 0.39, 0.89, '500 fb^{-1}')
leg.AddEntry(hzh, 'HZ', 'l')
leg.AddEntry(hwwh, 'WWH', 'l')
leg.SetFillStyle(0)
leg.Draw()

