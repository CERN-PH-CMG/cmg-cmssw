import copy
from CMGTools.RootTools.PyRoot import *
from CMGTools.RootTools.Style import *
from CMGTools.RootTools.treeComparator import draw
from CMGTools.RootTools.HistComparator import HistComparator
from ROOT import TGraph


nbins = 50
xmin = -5
xmax = 5

# shifts = [90,95,100,105,110]
shifts = [80, 90, 100, 110, 120]

var = 'jet1_eta'
cut = '(jet1_pt>30 && jet1_looseJetId>0.5 && jet1_puJetId>0.5 && diL_charge==0 && leg1_relIso05<0.1 && leg2_relIso05<0.1 && diL_charge==0 && abs(diL_mass-91.2)<15)*weight'
cut_higheta = '(jet1_pt>30 && jet1_looseJetId>0.5 && jet1_puJetId>0.5 && diL_charge==0 && leg1_relIso05<0.1 && leg2_relIso05<0.1 && diL_charge==0 && abs(diL_mass-91.2)<15 && abs(jet1_eta)>2.5 && abs(jet1_eta)<3.4 )*weight'

# data = Chain(None, 'data_100.root')
data = Chain(None, 'Prod_Rochester_29Nov/data_*/ZJetsTreeProducer/ZJetsTreeProducer_tree.root')

data_hist = TH1F('data_hist', 'data_hist', nbins, xmin, xmax)
data_hist.SetMarkerStyle(8)
data_hist.Sumw2()
data.Draw(var + '>>data_hist', cut, 'goff')

cor = Chain(None, 'dyjets_cor.root')
cor.SetWeight(1.37749,'global')

cor_hist = TH1F('cor_hist', 'cor_hist', nbins, xmin, xmax)
cor_hist.SetMarkerStyle(8)
sBlue.formatHisto(cor_hist)
cor.Draw(var + '>>cor_hist', cut, 'goff')


class Graph(TGraph):
    def __init__(self, n, bin):
        self.bin = bin
        super(Graph, self).__init__(n)
        

def load(shift, locals):
    fnam = 'Shifts/shift_{shift}.root'.format(shift=shift)
    print fnam
    chain = Chain(None, fnam)
    locals['c_{shift}'.format(shift=shift)] = chain
    hnam = 'h_{shift}'.format(shift=shift)
    hist = TH1F(hnam, hnam, nbins, xmin, xmax)
    hist.SetXTitle('#eta')
    hist.SetYTitle('number of jets')
    chain.SetWeight(1.37749,'global')
    chain.Draw(var + '>>' + hnam, cut, 'goff')
    hist.SetTitle(hnam)
    nextStyle().formatHisto(hist)
    locals[hnam] = hist

def graph(shiftlist, bin, locals):
    gr = Graph( len(shiftlist), bin )
    max = 0
    for idx, shift in enumerate(shiftlist):
        hnam = 'h_{shift}'.format(shift=shift)
        hist = locals[hnam]
        bc = hist.GetBinContent(bin)
        if bc > max:
            max = bc  
        gr.SetPoint(idx, bc, shift)
    # gr.Draw('alp')
    # gPad.Update()
    return gr, max

def solve(gr, data):
    gr.Draw('alp')
    bin = gr.bin
    dbc = data.GetBinContent(bin)
    scale = 100
    if dbc: 
        scale = gr.Eval(dbc, None, 'S')
    print 'solving', bin, dbc, scale
    return scale

def all_solve(graphs, data):
    h_cor = TH1F('h_cor', 'corrections;#eta;shift (%)',
                 data.GetNbinsX(),
                 data.GetXaxis().GetXmin(),
                 data.GetXaxis().GetXmax())
    sBlack.formatHisto(h_cor)
    h_cor.SetFillStyle(0)
    h_cor.SetStats(0)
    for gr in graphs:
        scale = solve(gr, data)
        print gr.bin, scale
        h_cor.SetBinContent(gr.bin, scale)
    can_cor = TCanvas('can_cor','can_cor', 600, 600)
    formatPad(can_cor)
    h_cor.Draw()
    gPad.Update()
    gPad.SaveAs('corrections.png')
    return h_cor, can_cor
        

def all_graphs(locals):
    graphs = []
    mx = 0
    for bini in range(nbins):
        bini += 1
        gr, grmax = graph(shifts, bini, locals)
        if grmax > mx:
            mx = grmax 
        graphs.append(gr)
    can_all_graphs = TCanvas('can_all_graphs','can_all_graphs', 600, 600)
    hist = TH2F('hist',';number of jets; shift (%)',
                10, 0, mx*1.1,
                10, min(shifts)-5, max(shifts)+5
                )
    hist.GetXaxis().SetNdivisions(5)
    sBlack.formatHisto( hist )
    formatPad(can_all_graphs)
    hist.SetStats(0)
    hist.Draw()
    for gr in graphs:
        gr.Draw('same')
    gPad.SaveAs('shift_funcs.png')
    gPad.Update()
    return graphs, hist, can_all_graphs


def all_eta( hists ):
    theHists = hists
    theHists[0] = copy.deepcopy( hists[0] )
    theHists[0].SetTitle('')
    theHists[0].SetStats(0)
    same = ''
    can = TCanvas('can_all_eta', 'can_all_eta', 600, 600)
    formatPad(can)
    leg = TLegend(0.7, 0.6, 0.89, 0.89)
    for hist in theHists:
        print hist.GetName()
        hist.Draw(same)
        leg.AddEntry(hist, hist.GetName(), 'lf')
        if same == '':
            same = 'same'
    data_hist.Draw('same')
    leg.AddEntry(data_hist, 'Data', 'p') 
    leg.Draw()
    gPad.Update()
    gPad.SaveAs('all_eta.png')
    gPad.SetLogy()
    gPad.SaveAs('all_eta_log.png')
    gPad.SetLogy(0)
    gPad.Update()
    return can, leg

def sort_hists_by_shift( locals ):
    hists = {}
    for key, hist in locals.iteritems():
        if not key.startswith('h_'):
            continue
        h, shift = key.split('_')
        try: 
            shift = int(shift)
            hists[shift] = hist
        except ValueError:
            continue
    shists = []
    for shift, hist in sorted(hists.iteritems(), reverse=True):
        shists.append(hist)
    return shists


for shift in shifts:
    load(shift, locals())

c_hists = sort_hists_by_shift( locals() )
can_all_eta, all_eta_leg = all_eta(c_hists)
    
# data_hist.Draw()
# h_100.Draw('same')
comp = HistComparator('#eta',data_hist, h_100, 'data', 'DYJets uncor.')
comp.draw()
comp.can.SaveAs('comp_uncor.png')

comp2 = HistComparator('#eta',data_hist, cor_hist, 'data', 'DYJets cor.')
comp2.draw()
comp2.can.SaveAs('comp_cor.png')

graphs, sup_hist, can_all_graphs = all_graphs(locals())

can_cor = TCanvas('can_cor', 'can_cor')
h_cor, can_cor = all_solve( graphs, data_hist )
h_cor.Draw()

f_cor = TFile('corrections.root', 'recreate')
f_cor.cd()
h_cor.Write()
f_cor.Close()

# c = draw(var, cut, c_110, c_90, name1='up', name2='down', nbins=nbins, xmin=xmin, xmax=xmax); c.draw()


