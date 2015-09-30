import os
import csv
import sys
import ROOT

def parseJSON(infile):
    with open(infile,'rb') as f:
        reader = csv.reader(f,delimiter=',',quoting=csv.QUOTE_NONE)
        keys=next(reader)
        rows=[]
        for row in reader:
            rows.append(row)
        return keys,rows

def compareJSONS(j1,j2):
    if j1[0]!=j2[0]:
        print 'Different keys'
        return False
    for i in xrange(min(len(j1[1]),len(j2[1]))):
        if j1[1][i]!=j2[1][i]:
            print 'Difference found on line %d'%i
            print ', '.join(j1[i][1])+' != '+', '.join(j2[i][1])
            return False
    if len(j1[1])!=len(j2[1]):
        print 'Different number of lines: %d vs. %d'%(len(j1[1]),len(j2[1]))
        return False
    print 'All ok'
    return True


muonkeys=[('pt','LepGood_pt'),('eta','LepGood_eta'),('phi','LepGood_phi'),
        ('pdgID','LepGood_pdgId'),('charge','LepGood_charge'),('miniIso','LepGood_miniRelIso'),
        ('miniIsoCharged','LepGood_miniRelIsoCharged'),('miniIsoNeutral','LepGood_miniRelIsoNeutral'),
        ('jetPtRel','LepGood_jetPtRelv2'),('jetCSV','LepGood_jetBTagCSV'),('jetPtRatio','LepGood_jetPtRatio_LepAwareJECv2'),
        ('sip3D','LepGood_sip3d'),('dxy','LepGood_dxy'),('dz','LepGood_dz'),('segmentCompatibility','LepGood_segmentCompatibility')]

if __name__=="__main__":

    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] tree.root")
    parser.add_option("-F", dest="friends", type="string", default=[], action="append",  help="Friend tree") 
    parser.add_option("-t", dest="test", type="string", default="muons", help="test name")
    parser.add_option("-n", dest="maxN", type="float", default="1e9", help="max entries")
    parser.add_option("-v", dest="verbose", default=False, action="store_true", help="print output")
    parser.add_option("-o", dest="outfile", default="outfile.txt", help="output file")
    (options, args) = parser.parse_args()

    test = options.test
    fname = args[0]
    tfile = ROOT.TFile(fname)
    t = tfile.tree
    for fr in options.friends:
        f.AddFriend("sf/t",fr)
    maxN=int(options.maxN)
    N=0
    if test=="muons":
        res=[['event#'],[]]
        res[0].extend([key[0] for key in muonkeys])
        for ev in t:
            N+=1
            if N>maxN: break
            muons=[]
            for i in xrange(t.nLepGood):
                if abs(t.LepGood_pdgId[i])==13:
                    muons.append(i)
            if len(muons)<1: continue
            for m in muons:
                out = []
                out.append(t.evt)
                for key in muonkeys:
                    out.append(getattr(t,key[1])[m])
                res[1].append(out)
        outfilename = options.outfile
        with open(outfilename,'w') as outfile:
            outfile.write(','.join(res[0])+'\n')
            for row in res[1]:
                outfile.write('%d'%row[0]+','+','.join(['%.5f'%x for x in row[1:]])+'\n')
            outfile.close()
        with open(outfilename,'r') as outfile:
            for line in outfile: print line.rstrip('\n')
