import sys 

from CMGTools.RootTools.PyRoot import *
from CMGTools.RootTools.statistics.TreeNumpy import TreeNumpy


arg = sys.argv[1]

nEntries = -1
shift = 100
prefix = None
comp = 'DYJets'
file_cor = None
h_cor = None

if arg == 'data':
    shift = 100
    prefix = 'data'
    comp = 'data_*'
    postfix = int(shift)
elif arg.endswith('.root'):
    shift = -1.
    file_cor = TFile( arg )
    h_cor = file_cor.Get('h_cor')
    prefix = 'cor'
    postfix = os.path.splitext(arg)[0]
else:
    try:
        shift = float(sys.argv[1])
        prefix = 'shift'
        postfix = int(shift)
    except ValueError:
        print 'arg should be "data" or the shift value (a float)'
        sys.exit(1)


chain = Chain(None, 'Prod_Rochester_29Nov/{comp}/ZJetsTreeProducer/ZJetsTreeProducer_tree.root'.format(comp=comp))
# chain = Chain(None, 'higgsvbf125.root')

name = '{prefix}_{postfix}'.format(
    prefix = prefix,
    postfix = postfix
    )

print shift, name, comp

file = TFile( name+'.root', 'recreate')

tree = TreeNumpy(chain.GetName(),
                 ', '.join([chain.GetTitle(), 'corrected']) )

tree.copyStructure(chain)

toShift = ['jet1_pt', 'jet2_pt']

shift_factor = shift/100.

def correct(tree, entry, jetname, shift ):
    eta = getattr(ie, '_'.join([jetname, 'eta']) )
    pt = getattr(ie, '_'.join([jetname, 'pt']) )
    corpt = pt
    if shift < 0:
        etabin = h_cor.FindBin(eta)
        shift = h_cor.GetBinContent(etabin)/100.
    corpt *= shift
    if pt>0:
        # print 'correcting', jetname, eta, pt, corpt, shift
        tree.fill('_'.join([jetname, 'pt']), corpt)
    else:
        tree.fill('_'.join([jetname, 'pt']), pt)
        
    
for index, ie in enumerate(chain.chain):
    if index%1000==0: print 'entry:', index    
    for varName in tree.vars:
        val = getattr(ie, varName)
        # if varName in toShift:
        #     val *= shift_factor
        tree.fill(varName, val)
    correct(tree, ie, 'jet1', shift_factor)
    correct(tree, ie, 'jet2', shift_factor)
    tree.tree.Fill()
    if nEntries>0 and index+1>=nEntries:
        break

file.Write()
file.Close()
