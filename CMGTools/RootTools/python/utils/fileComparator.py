import time
import os
import re
from CMGTools.RootTools.PyRoot import *
from CMGTools.RootTools.utils.file_dir import file_dir, file_dir_names

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc: # Python >2.5
        if exc.errno == errno.EEXIST:
            pass
        else: raise


class Comparator(object):

    def __init__(self, info1, info2, filter):
        self.info1 = info1
        self.info2 = info2
        self.legend = None
        self.filter = re.compile(filter)

    def browse(self):
        maindir = 'Plots_Comparator'
        if os.path.isdir(maindir):
            os.system( 'rm -r ' + maindir)
        os.mkdir(maindir)
        for h1name, h1 in sorted(self.info1.hists.iteritems()):
            h2 = self.info2.hists.get(h1name, None)
            if h2 is None:
                pass
            # print h1name, 'not in', d2dir.GetName()
            else:
                plotdir = '/'.join([maindir,os.path.dirname(h1name)])
                try:
                    mkdir_p( plotdir )
                except:
                    pass
                h1.SetTitle(h1name)
                h2.SetTitle(h1name)
                if not self.filter.search( h1name ):
                    print 'Skipping', h1name
                    continue
                self.drawHists(h1, h2, h1name)
                res = raw_input('')
                
    def drawHists(self, h1, h2, h1name):
        print 'Draw', h1name
        h1.SetMarkerColor(4)
        h2.SetMarkerColor(1)                    
        h1.SetLineColor(4)
        h1.SetFillColor(5)
        h1.SetFillStyle(1001)
        h2.SetLineColor(1)
        maximum = max(h1.GetBinContent(h1.GetMaximumBin()),
                      h2.GetBinContent(h2.GetMaximumBin()))
        h1.Draw('E2')
        h2.Draw('same')
        if self.legend is None:
            self.legend = TLegend(0.5, 0.65, 0.89, 0.8)
            self.legend.AddEntry(h1, self.info1.name, 'pf')
            self.legend.AddEntry(h2, self.info2.name, 'p')
        h1.GetYaxis().SetRangeUser(0.001, maximum*1.2)
        self.legend.Draw('same')
        gPad.Update()
        pngname = '/'.join(['Plots_Comparator',h1name+'.png'])
        print pngname
        gPad.SaveAs(pngname)
        return True

        
class FlatFile(object):
    def __init__(self, tdir, name):
        self.tdir = tdir
        self.name = name
        self.hists = {}
        self.flatten( self.tdir, '.', self.hists )
        
    def flatten(self, dir, mothername, hists):
        for key in dir.GetListOfKeys():
            name = key.GetName()
            absname = '/'.join([mothername, name])
            obj = dir.Get(name)
            if type(obj) in dirTypes:
                self.flatten(obj, absname, self.hists)
            elif type(obj) in histTypes:
                self.hists[absname] = obj


if __name__ == '__main__':
    import sys

    from optparse import OptionParser
    
    parser = OptionParser()
    parser.usage = '''
    fileComparator.py <file1[:dir1]> <file2[:dir2]> 
    '''
    parser.add_option("-f", "--filter", 
                      dest="filter", 
                      help="Filtering regexp pattern to select histograms.",
                      default='.*')

    (options,args) = parser.parse_args()

    
    f1, d1 = file_dir(args[0])
    f2, d2 = file_dir(args[1])
    name1 = '/'.join( [f1.GetName(), d1.GetName()])
    name2 = '/'.join( [f2.GetName(), d2.GetName()])
    file1 = FlatFile( d1, name1)
    file2 = FlatFile( d2, name2)
    comparator = Comparator(file1, file2, options.filter)
    comparator.browse()

