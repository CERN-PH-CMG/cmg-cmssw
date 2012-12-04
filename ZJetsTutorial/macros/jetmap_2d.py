import os
from numpy import array

from CMGTools.RootTools.PyRoot import *
from CMGTools.RootTools.treeComparator import draw 
from CMGTools.RootTools.html.DirectoryTree import Directory

data = Chain(None, 'Prod_Rochester_29Nov/data_*/ZJetsTreeProducer/ZJetsTreeProducer_tree.root')


xbins = [
    0.,
    0.1,
    0.2,
    0.3,
    0.4,
    0.5,
    0.6,
    0.7,
    0.8,
    0.9,
    1.0,
    1.2,
    1.4,
    1.6,
    1.8,
    2.0,
    2.5,
    3.0,
    4.0,
    5.0
    ]

ybins = [
    5.,
    10.,
    15.,
    20.,
    25.,
    30.,
    40.,
    60.,
    80.,
    100.,
    150.,
    200.,
    300.,
    ]


xbinsa = array(xbins)
ybinsa = array(ybins)

def plot(tree):
    h = TH2F('h','',
             len(xbinsa)-1, xbinsa,
             len(ybinsa)-1, ybinsa
             )
    data.Draw('jet1_pt:abs(jet1_eta)>>h','diL_charge==0 && leg1_relIso05<0.1 && leg2_relIso05<0.1')
    return h

def plot1D(tree):
    h = TH1F('h','',
             len(xbinsa)-1, xbinsa) 
    data.Draw('abs(jet1_eta)>>h','diL_charge==0 && leg1_relIso05<0.1 && leg2_relIso05<0.1')

h = plot(data)
h.Draw('lego2')
