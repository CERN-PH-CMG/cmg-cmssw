import ROOT
import math

def makeList(filename,out):
    f = ROOT.TFile(filename)
    t=f.Get('FourLeptonTreeProducer/tree')
    ff=open(out,'w')
    for event in t:

        if event.H_Mass>70:
            ff.write('%d:'% (event.RUN))
            ff.write('%d:'% (event.LUMI))
            ff.write('%d:'% (event.EVENT))
            ff.write('%.2f:'% (event.H_Mass))
            ff.write('%.2f:'% (event.H_Z1_Mass))
            ff.write('%.2f:'% (event.H_Z2_Mass))
            ff.write('%.2f:'% (event.H_MassErrRaw))
            ff.write('%.2f:'% (event.H_MassErr))
            ff.write('%.3f:'% (max(event.H_KD,-1)))
            ff.write('%.2f:'% (event.H_Pt))
            ff.write('%.1f:'% float(event.H_NJets))
            ff.write('%.2f:'% (max(event.H_LeadingJetPt,-1)))
            ff.write('%.2f:'% (max(event.H_SubleadingJetPt,-1)))
            ff.write('%.2f:'% (max(event.H_MJJ,-1)))
            if event.H_DEta>-99.:
                ff.write('%.3f:'% math.fabs(event.H_DEta))
            else:
                ff.write('-1.000:')
    
            ff.write('%.3f:'% (max(event.H_Fisher,-1)))
            ff.write('%.3f:'% (max(event.H_GG0KD,-1)))
            ff.write('%.3f:'% (max(event.H_GG0HKD,-1)))
            ff.write('%.3f:'% (max(event.H_QQ1PlusKD,-1)))
            ff.write('%.3f:'% (max(event.H_QQ1MinusKD,-1)))
            ff.write('%.3f:'% (max(event.H_GG2PlusKD,-1)))
            ff.write('%.3f'% (max(event.H_QQ2PlusKD,-1)))
            ff.write('\n')
    f.Close()                         
    ff.close




