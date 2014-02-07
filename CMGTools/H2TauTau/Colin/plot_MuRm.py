import math
import os

canvas = None
pad = None
padr = None


#TODO warning normalization
def drawWithRatio(var, cut, t1, t2, w1, w2, nbins, xmin=0, xmax=200, var2=None):
    comp = draw(var, cut, t1, t2, w1=w1, w2=w2, normalize=-1,
                nbins=nbins,xmin=xmin, xmax=xmax, var2=var2)
    return comp
    
def addWeight(iwstr, wfnam):
    weightFile = shelve.open( wfnam )
    wo = weightFile.values()[0]
    wei = wo.weightStr()
    iwstr = '({iwstr}) * ({wei})'.format(iwstr=iwstr, wei=wei)
    return iwstr


def lowHighMTRatio(hist, lognam, msg, lowmax=None, highmin=None, highmax=None):    
    errlow = ROOT.Double()
    lowmin = 1
    if lowmax is None: lowmax = 3
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
    

def report( dirname, comp, a1, a2, cut, highmin=None, highmax=None, force=False):
    if force:
        os.system('rm -r ' + dirname)
    oldpwd = os.getcwd()
    os.mkdir(dirname)
    os.chdir(dirname)
    r1 = lowHighMTRatio(comp.h1, a1, cut, highmin=highmin, highmax=highmax)
    r2 = lowHighMTRatio(comp.h2, a2, cut, highmin=highmin, highmax=highmax)
    
    slog = open('scaling.txt','w')
    rFactor = r1/r2
    sstr = 'scaling   :  {val}\n'.format(val = rFactor)
    print sstr
    slog.write( sstr )
    slog.close()
    comp.pad_main.cd()
    latex = TLatex()
    latex.SetNDC()
    latex.DrawLatex(0.6, 0.6, 'R={val:3.2f} #pm {err:3.2f}'.format(val=rFactor.val, err=rFactor.err))
    comp.can.SaveAs('plot.png')
    os.chdir(oldpwd)


def systematicReport(basename):
    os.mkdir(basename)
    # Moriond categories
##     cuts = {
##         'Inclusive':cat_Inc,
##         '0jet_low':' && '.join( [cat_Inc, cat_J0, 'l1_pt<40'] ), 
##         '0jet_high':' && '.join( [cat_Inc, cat_J0, 'l1_pt>40'] ), 
##         '1jet_low':' && '.join( [cat_Inc, cat_J1, 'l1_pt<40'] ), 
##         '1jet_high':' && '.join( [cat_Inc, cat_J1, 'l1_pt>40'] ), 
##         '2jet':' && '.join( [cat_Inc, cat_J2] ), 
##         }
    # Summer 13 categories
    cuts = {
        'Inclusive':cat_Inc,
        '0jet_medium':' && '.join( [cat_Inc, cat_J0_medium, 'l1_pt>30'] ), 
        '0jet_high':' && '.join( [cat_Inc, cat_J0_high, 'l1_pt>30'] ), 
        '1jet_medium':' && '.join( [cat_Inc, cat_J1_medium, 'l1_pt>30'] ), 
        '1jet_high_lowhiggs':' && '.join( [cat_Inc, cat_J1_high_lowhiggs, 'l1_pt>30'] ), 
        '1jet_high_mediumhiggs':' && '.join( [cat_Inc, cat_J1_high_mediumhiggs, 'l1_pt>30'] ),
        '1B':'&&'.join( [cat_Inc, cat_1BInclusive] ),
        '0B':'&&'.join( [cat_Inc, cat_0B] )
        # think about tau pT cut!!
        #         'vbf_tight':' && '.join( [cat_Inc, cat_VBF_tight] ),
        #         'vbf_loose':' && '.join( [cat_Inc, cat_VBF_loose] ),
        }
    
    for cut in cuts:
        # cuts[cut] = cuts[cut].replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>-0.5')
        cuts[cut] = cuts[cut].replace('l1_threeHitIso<1.5', 'l1_threeHitIso<10')
        if a1.find('WJets')!=-1:
            cuts[cut] += '&& (abs(genWnu_eta)<2. || genWnu_eta<-90)'

    for cutname, cut in cuts.iteritems():
        highmin = 8
        highmax = None
        if cutname.startswith('vbf'):
            highmin = 7
            highmax = 12
        # import pdb; pdb.set_trace()
        # h1, h2, hr, leg = drawWithRatio(var, cut, trees[a1], trees[a2], w1=w1, w2=w2, nbins=20, xmin=0, xmax=200, var2=var2)
        comp = drawWithRatio(var, cut, trees[a1], trees[a2], w1=w1, w2=w2, nbins=20, xmin=0, xmax=200, var2=var2)
        report('/'.join([basename, cutname]), comp, a1, a2, cut,
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
        sys.exit(1)

    if options.report and os.path.isdir(options.report):
        print options.report, ' already exists. quit'
        sys.exit(2)

    # importing after test of argument validity
    # not to waste time loading fwlite.
    from CMGTools.RootTools.treeComparator import *
    from CMGTools.H2TauTau.proto.plotter.categories_TauMu import * 
    from CMGTools.RootTools.ReWeighter import ReWeighter
    from CMGTools.RootTools.statistics.Value import Value
    from ROOT import TLatex
        
    p1 = args[0]
    p2 = args[1]
    a1 = p1.split('/')[0]
    a2 = p2.split('/')[0]
    patterns = [ (a1, p1), (a2, p2) ]
    trees = getTreesOld('H2TauTauTreeProducerTauMu', patterns)
    pprint.pprint(trees)
    trees[a1].SetTitle( a1 )
    trees[a2].SetTitle( a2 )

    # beware weight! 
    # cut = ' && '.join( [cat_Inc] )
    cut = ' && '.join( [cat_Inc, cat_VBF_Rel_20, 'l1_pt>30'] )
    # + '&&' + cat_0B
    # cut = cat_Inc
    if a1.find('WJets')!=-1:
        cut += '&& (abs(genWnu_eta)<2. || genWnu_eta<-90)'

    # dr = 'sqrt((l1_eta-l2_eta)^2 + (l1_phi-l2_phi)^2)>2
    cut = cut.replace('l1_threeHitIso<1.5', 'l1_threeHitIso<10')
    # cut += ' && ' + dr
    w1 = '1'
    w2 = 'weight'
    if a1.find('WJets') != -1:
        w1 = '1'
        w2 = '1'

    var = args[2]
    var2 = options.var2
    if var2 is None:
        var2 = var 
    
    if 0:
        comp = draw(var, cut, trees[a1], trees[a2],
                    w1=w1, w2=w2, normalize=False, nbins=20,
                    var2 = 'mt*80.4/91.2')
##         h1, h2, leg = draw(var, cut, trees[a1], trees[a2],
##                            w1=w1, w2=w2, normalize=False, graphics=False, nbins=20,
##                            var2 = 'mt*80.4/91.2')
        buildCanvas()
        ratio = drawWithRatio(h1, h2, leg, var)
        mtrw = ReWeighter('mt', h1, h2)
        weightFile = shelve.open('weights_in.txt')
        weightFile['weights'] = mtrw
        weightFile.close()

    if options.iweight:
        for wfnam in options.iweight.split(','):
            w2 = addWeight( w2, wfnam )
            
    comp = drawWithRatio(var, cut, trees[a1], trees[a2], w1=w1, w2=w2, nbins=20, xmin=0, xmax=200, var2=var2)
    # report('Test', comp, a1, a2, cut, highmin=8)
     
    
    if options.oweight:
        weightFile = shelve.open( options.oweight )
        mtrw_out = ReWeighter(var2, h1, h2)
        weightFile[var] = mtrw_out
        weightFile.close()
        
    
    if options.report:
        report(options.report, comp, a1, a2, cut, highmin=8)
    else:
        dir = 'Tmp_plot_MuRm'
        report(dir, comp, a1, a2, cut, highmin=8, force=True)
        
