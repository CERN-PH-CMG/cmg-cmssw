#!/usr/bin/env python
import ROOT, os, re
from array import array
from math import sqrt

class tableToHisto2DConverter:
    def __init__(self,lepType,idType,foption):
        self.fout = ROOT.TFile.Open("eff_%s.root" % lepType, foption)
        self.idType = idType
        self.tables = [ "eff_%s_%s_%s.txt" %(lepType,idType,submodule) for submodule in ["barrel","endcap"] ]        
        if(lepType=="muon"): self.etabin_edges = [0,1.2,2.5]
        else: self.etabin_edges = [0,1.479,2.5]

    def convertTable(self):
        eb=1
        for table in self.tables:

            print "Processing table ",table,"..."
            file=open(table,'r') 
            row = file.readlines()

            ptbin_edges = []
            mc_effs = []
            data1_effs = []
            data2_effs = []

            for line in row:
                if line.find("GeV") > -1:
                    info = line.split('&')
                    bin = (info[0].split('\\'))[0]
                    ptbin_edges.append(float(bin.split('-')[0]))
                    mc_eff = [ float((re.findall(r'[-+]?\d*\.\d+|\d+', (info[1].split('\\'))[0]))[0]) , float((re.findall(r'[-+]?\d*\.\d+|\d+', (info[1].split('\\'))[1]))[0]) ]
                    data1_eff = [ float((re.findall(r'[-+]?\d*\.\d+|\d+', (info[2].split('\\'))[0]))[0]) , float((re.findall(r'[-+]?\d*\.\d+|\d+', (info[2].split('\\'))[1]))[0]) ]
                    data2_eff = [ float((re.findall(r'[-+]?\d*\.\d+|\d+', (info[3].split('\\'))[0]))[0]) , float((re.findall(r'[-+]?\d*\.\d+|\d+', (info[3].split('\\'))[1]))[0]) ]
                    mc_effs.append(mc_eff)
                    data1_effs.append(data1_eff)
                    data2_effs.append(data2_eff)

            ptbin_edges.append(float(bin.split('-')[1]))

            if "barrel" in table:
                hist2dmc = ROOT.TH2D("Lep%sEffmc" % self.idType,"effmc",len(ptbin_edges)-1,array('d',ptbin_edges),len(self.etabin_edges)-1,array('d',self.etabin_edges))
                hist2ddata1 = ROOT.TH2D("Lep%sEffdata1" % self.idType,"effdata1",len(ptbin_edges)-1,array('d',ptbin_edges),len(self.etabin_edges)-1,array('d',self.etabin_edges))
                hist2ddata2 = ROOT.TH2D("Lep%sEffdata2" % self.idType,"effdata2",len(ptbin_edges)-1,array('d',ptbin_edges),len(self.etabin_edges)-1,array('d',self.etabin_edges))
                hist2dsf1 = ROOT.TH2D("Lep%sSF1" % self.idType,"sf1",len(ptbin_edges)-1,array('d',ptbin_edges),len(self.etabin_edges)-1,array('d',self.etabin_edges))
                hist2dsf2 = ROOT.TH2D("Lep%sSF2" % self.idType,"sf2",len(ptbin_edges)-1,array('d',ptbin_edges),len(self.etabin_edges)-1,array('d',self.etabin_edges))

            for pb in xrange(1,len(ptbin_edges)):
                hist2dmc.SetBinContent(pb,eb,mc_effs[pb-1][0])
                hist2dmc.SetBinError(pb,eb,mc_effs[pb-1][1])
    
                hist2ddata1.SetBinContent(pb,eb,data1_effs[pb-1][0])
                hist2ddata1.SetBinError(pb,eb,data1_effs[pb-1][1])

                hist2ddata2.SetBinContent(pb,eb,data2_effs[pb-1][0])
                hist2ddata2.SetBinError(pb,eb,data2_effs[pb-1][1])

                hist2dsf1.SetBinContent(pb,eb,data1_effs[pb-1][0]/mc_effs[pb-1][0])
                hist2dsf1.SetBinError(pb,eb,data1_effs[pb-1][0]/mc_effs[pb-1][0] * sqrt(data1_effs[pb-1][1]/data1_effs[pb-1][0] + mc_effs[pb-1][1]/mc_effs[pb-1][0]))

                hist2dsf2.SetBinContent(pb,eb,data2_effs[pb-1][0]/mc_effs[pb-1][0])
                hist2dsf2.SetBinError(pb,eb,data2_effs[pb-1][0]/mc_effs[pb-1][0] * sqrt(data2_effs[pb-1][1]/data2_effs[pb-1][0] + mc_effs[pb-1][1]/mc_effs[pb-1][0]))

            eb += 1
    
        self.fout.WriteTObject(hist2dmc.Clone())
        self.fout.WriteTObject(hist2ddata1.Clone())
        self.fout.WriteTObject(hist2ddata2.Clone())
        self.fout.WriteTObject(hist2dsf1.Clone())
        self.fout.WriteTObject(hist2dsf2.Clone())

        self.fout.Close()


if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options]")
    parser.add_option("-l", "--lepType", dest="lepType", default='muon', help="lepton type (muon or electron)");
    (options, args) = parser.parse_args()

    ROOT.gROOT.SetBatch(True)
    if options.lepType == "muon": idTypes = ["Loose", "Tight"]
    elif options.lepType == "electron": idTypes = ["Loose", "Tight"]
    else:
        print "lepType should be either muon or electron. Exiting"
        sys.exit(0)
    foption = "recreate"
    for idt in idTypes:
        converter = tableToHisto2DConverter(options.lepType,idt,foption)
        converter.convertTable()
        foption = "update"
    print "Done."
