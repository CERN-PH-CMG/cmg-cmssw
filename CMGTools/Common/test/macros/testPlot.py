import ROOT as rt
import os
from TreeHolder import TreeHolder

if __name__ == '__main__':

    rt.gROOT.Macro('init.C')

    from optparse import OptionParser
    
    parser = OptionParser()
    parser.add_option("-b", "--batch", dest="batch",action="store_true", default=True,help='Used to suppress plot printing in root')
    parser.add_option("-o", "--output", dest="output", default='output.root',help='Output root file')
    (options, files) = parser.parse_args()

    for f in files:
        if not os.path.exists(f):
            continue

        input = None 
        try:
            input = rt.TFile.Open(f)
            th = TreeHolder(input.Get('Events'))

            th.plot("cmgDiMuons_cmgdimuon__PAT.obj.pt()","TMath::Abs(cmgDiMuons_cmgdimuon__PAT.obj.leg1().eta()) < 2.1")

            #TODO: Currently causes a segfault in ROOT - same issue as Colin as seen with strbitsets?
            #th.plot("cmgDiMuons_cmgdimuon__PAT.obj.pt()",'cmgDiMuons_cmgdimuon__PAT.obj.getSelection("VBTF_muon")')

            output = rt.TFile.Open(options.output,'RECREATE')
            th.write('Events')
            output.Close()

        finally:
            if input is not None:
                input.Close()
