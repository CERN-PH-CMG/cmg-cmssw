#!/usr/bin/env python
import ROOT, os, re
from array import array
from math import sqrt


lepType="muon"

class tableToHisto2DConverter:
    def __init__(self,lepType,idType,tables):
        self.fout = ROOT.TFile.Open("eff_%s_%sid.root" % (lepType,idType), "RECREATE")
        self.tables = tables
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

            if ROOT.gROOT.FindObject("effmc") == None:
                hist2dmc = ROOT.TH2D("effmc","effmc",len(ptbin_edges)-1,array('d',ptbin_edges),len(self.etabin_edges)-1,array('d',self.etabin_edges))
                hist2ddata1 = ROOT.TH2D("effdata1","effdata1",len(ptbin_edges)-1,array('d',ptbin_edges),len(self.etabin_edges)-1,array('d',self.etabin_edges))
                hist2ddata2 = ROOT.TH2D("effdata2","effdata2",len(ptbin_edges)-1,array('d',ptbin_edges),len(self.etabin_edges)-1,array('d',self.etabin_edges))
                hist2dsf1 = ROOT.TH2D("sf1","sf1",len(ptbin_edges)-1,array('d',ptbin_edges),len(self.etabin_edges)-1,array('d',self.etabin_edges))
                hist2dsf2 = ROOT.TH2D("sf2","sf2",len(ptbin_edges)-1,array('d',ptbin_edges),len(self.etabin_edges)-1,array('d',self.etabin_edges))

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
    parser = OptionParser(usage="%prog [options] barreltable endcaptable")
    parser.add_option("-l", "--lepType", dest="lepType", default='muon', help="lepton type (muon or electron)");
    parser.add_option("-i", "--idType", dest="idType", default='loose', help="lepton id type (loose or tight for muons, veto or tight for electrons)");
    (options, args) = parser.parse_args()
    if len(args) != 2:
        print "You must specify the barrel and endcap efficiency tables"
        exit()

    ROOT.gROOT.SetBatch(True)
    converter = tableToHisto2DConverter(options.lepType,options.idType,args)
    converter.convertTable()
    print "Done."
