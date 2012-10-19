import math
import os
from CMGTools.RootTools.treeComparator import *
from CMGTools.H2TauTau.proto.plotter.categories_TauMu import * 
from CMGTools.RootTools.ReWeighter import ReWeighter
from CMGTools.RootTools.statistics.Value import Value

canvas = None
pad = None
padr = None

def buildCanvas():
    global can, pad, padr
    can = TCanvas('can','',600,600)
    can.cd()
    can.Draw()
    sep = 0.35
    pad = TPad('pad','',0.01,sep,0.99, 0.99)
    pad.SetBottomMargin(0.04)
    padr = TPad('padr','',0.01, 0.01, 0.99, sep)
    padr.SetTopMargin(0.04)
    padr.SetBottomMargin(0.3)
    pad.Draw()
    padr.Draw()
    return can, pad, padr

def drawWithRatio(var, cut, t1, t2, w1, w2, nbins, xmin=0, xmax=200, var2=None):
    h1, h2, leg = draw(var, cut, t1, t2, w1=w1, w2=w2, normalize=False, graphics=False, nbins=nbins,xmin=xmin, xmax=xmax, var2=var2)
    pad.cd()
    h1.Draw()
    h1.GetYaxis().SetRangeUser(0.1, max(h1.GetMaximum(), h2.GetMaximum())*1.2)
    h2.Draw('same')
    leg.Draw('same')
    padr.cd()
    hr = h1.Clone()
    hr.Divide(h2)
    hr.SetStats(0)
    hr.Draw()
    hr.GetYaxis().SetNdivisions(4)
    # hr.GetYaxis().SetTitle('Exp./Obs.')
    hr.GetYaxis().SetTitleSize(0.1)
    hr.GetYaxis().SetTitleOffset(0.5)
    hr.GetXaxis().SetTitle('{var}'.format(var=var))
    hr.GetXaxis().SetTitleSize(0.13)
    hr.GetXaxis().SetTitleOffset(0.9)
    rls = 0.075
    hr.GetYaxis().SetLabelSize(rls)
    hr.GetXaxis().SetLabelSize(rls)
    hr.GetYaxis().SetRangeUser(0.5, 1.5)
    can.cd()
    can.Update()
    return h1, h2, hr, leg


def addWeight(iwstr, wfnam):
    weightFile = shelve.open( wfnam )
    wo = weightFile.values()[0]
    wei = wo.weightStr()
    iwstr = '({iwstr}) * ({wei})'.format(iwstr=iwstr, wei=wei)
    return iwstr


def lowHighMTRatio(hist, lognam, msg, lowmax=None, highmin=None, highmax=None):    
    errlow = ROOT.Double()
    lowmin = 1
    if lowmax is None: lowmax = 4
    low = hist.IntegralAndError(lowmin,lowmax,errlow)
    errhigh = ROOT.Double()
    if highmin is None: highmin = 8
    if highmax is None: highmax = hist.GetNbinsX()+1
    high = hist.IntegralAndError(highmin,highmax,errhigh)
    
    print 'low mT region', hist.GetXaxis().GetBinLowEdge(lowmin), hist.GetXaxis().GetBinUpEdge(lowmax)
    print 'high mT region', hist.GetXaxis().GetBinLowEdge(highmin), hist.GetXaxis().GetBinUpEdge(highmax)

    # 60 to 120 for VBF

    vlow = Value(low, errlow)
    vhigh = Value(high, errhigh)
    vratio = vlow / vhigh
    
    slow =       'low mT      :  {val}'.format(val=vlow)
    shigh =      'high mT     :  {val}'.format(val=vhigh)
    sratio =     'ratio       :  {val}'.format(val=vratio)
    slowrange =  'low range   :  {min:5.1f} -> {max:5.1f}'.format(
        min = hist.GetXaxis().GetBinLowEdge(lowmin),
        max = hist.GetXaxis().GetBinUpEdge(lowmax),
        )
    shighrange = 'high range  :  {min:5.1f} -> {max:5.1f}'.format(
        min = hist.GetXaxis().GetBinLowEdge(highmin),
        max = hist.GetXaxis().GetBinUpEdge(highmax),
        )
    
    
    out = open(lognam.replace('*','X')+'_ratio.txt', 'w')
    thestr = '\n'.join(['-'*70,lognam, '-'*70, msg, slow, shigh, sratio,'\n',
                        slowrange, shighrange])
    thestr += '\n'
    out.write(thestr)
    out.close()
    print thestr
    return vratio
    

def report( dirname, h1, h2, a1, a2, cut, highmin=None, highmax=None):
    if dirname.find('Test')!=-1:
        os.system('rm -r ' + dirname)
    oldpwd = os.getcwd()
    os.mkdir(dirname)
    os.chdir(dirname)
    r1 = lowHighMTRatio(h1, a1, cut, highmin=highmin, highmax=highmax)
    r2 = lowHighMTRatio(h2, a2, cut, highmin=highmin, highmax=highmax)
    
    slog = open('scaling.txt','w')
    sstr = 'scaling   :  {val}\n'.format(val = r1/r2)
    print sstr
    slog.write( sstr )
    slog.close()
    
    can.SaveAs('plot.png')
    os.chdir(oldpwd)


def systematicReport(basename):
    os.mkdir(basename)
    cuts = {
        'Inclusive':cat_Inc,
        '0jet_low':' && '.join( [cat_Inc, cat_J0, 'l1_pt<40'] ), 
        '0jet_high':' && '.join( [cat_Inc, cat_J0, 'l1_pt>40'] ), 
        '1jet_low':' && '.join( [cat_Inc, cat_J1, 'l1_pt<40'] ), 
        '1jet_high':' && '.join( [cat_Inc, cat_J1, 'l1_pt>40'] ), 
        '2jet':' && '.join( [cat_Inc, cat_J2] ), 
        }
    for cut in cuts:
        cuts[cut] = cuts[cut].replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>-0.5')

    # import pdb; pdb.set_trace()
    for cutname, cut in cuts.iteritems():
        highmin = 8
        highmax = None
        if cutname=='2jet':
            highmin = 7
            highmax = 12
        # import pdb; pdb.set_trace()
        h1, h2, hr, leg = drawWithRatio(var, cut, trees[a1], trees[a2], w1=w1, w2=w2, nbins=20, xmin=0, xmax=200, var2=var2)
        report('/'.join([basename, cutname]), h1, h2, a1, a2, cut,
               highmin=highmin, highmax=highmax)
    
if __name__ == '__main__':
    import sys
    import pprint
    import shelve

    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option("-v","--var2", dest="var2",
                      default=None,
                      help="second variable")
    parser.add_option("-i","--iweight", dest="iweight",
                      default=None,
                      help="weight file for second tree")
    parser.add_option("-o","--oweight", dest="oweight",
                      default=None,
                      help="output weight file for second tree")
    parser.add_option("-r","--report", dest="report",
                      default=None,
                      help="directory name for report")

    (options,args) = parser.parse_args()
    if len(args) !=3 :
        print  'pattern1 pattern2 var'
        
    p1 = args[0]
    p2 = args[1]
    a1 = p1.split('/')[0]
    a2 = p2.split('/')[0]
    patterns = [ (a1, p1), (a2, p2) ]
    trees = getTrees('H2TauTauTreeProducerTauMu', patterns)
    pprint.pprint(trees)
    trees[a1].SetTitle( a1 )
    trees[a2].SetTitle( a2 )

    buildCanvas()

    # cut = cat_Inc + ' && ' + cat_VBF.replace('0.978', '0.5')
    # cut = cat_Inc + '&& mt<30 && (abs(genWnu_eta)<2.4 || genWnu_eta<-90) && l1_pt>40' + '&&' + cat_J1
    cut = cat_Inc + ' && ' + cat_J0
    # cut = cat_Inc + ' && ' + cat_J0
    # cut = cat_Inc + ' && l1_pt<40 && ' + cat_J1
    # cut = cat_Inc + ' && ' + cat_J0
    # cut = cat_Inc + '&& (abs(genWnu_eta)<2.4 || genWnu_eta<-90)'
    # cut += ' && ' + cat_J2
    # cut = cat_Inc + ' && l1_' + cat_J1
    # cut = cat_Inc + '  && ' + cat_VBF_Rel.replace('-0.7','0.5')
    # cut = cat_Inc + ' && l1_pt<40 && ' + cat_J0

    if a1.find('WJets')!=-1:
        cut += '&& (abs(genWnu_eta)<2.4 || genWnu_eta<-90)'

    dr = 'sqrt((l1_eta-l2_eta)^2 + (l1_phi-l2_phi)^2)>2'
    cut = cut.replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>-0.5')
    # cut += ' && ' + dr
    w1 = '1'
    if a1.find('WJets') != -1:
        w1 = 'weight'
    w2 = 'weight'

    var = args[2]
    var2 = options.var2
    if var2 is None:
        var2 = var 
    
    if 0:
        h1, h2, leg = draw(var, cut, trees[a1], trees[a2],
                           w1=w1, w2=w2, normalize=False, graphics=False, nbins=20,
                           var2 = 'mt*80.4/91.2')
        buildCanvas()
        ratio = drawWithRatio(h1, h2, leg, var)
        mtrw = ReWeighter('mt', h1, h2)
        weightFile = shelve.open('weights_in.txt')
        weightFile['weights'] = mtrw
        weightFile.close()

    if options.iweight:
        for wfnam in options.iweight.split(','):
            w2 = addWeight( w2, wfnam )
            
    h1, h2, hr, leg = drawWithRatio(var, cut, trees[a1], trees[a2], w1=w1, w2=w2, nbins=20, xmin=0, xmax=200, var2=var2)
    
    if options.oweight:
        weightFile = shelve.open( options.oweight )
        mtrw_out = ReWeighter(var2, h1, h2)
        weightFile[var] = mtrw_out
        weightFile.close()
        
    
    if options.report:
        report(options.report, h1, h2, a1, a2, cut)
