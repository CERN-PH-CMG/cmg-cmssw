# from ROOT import TTree, TH1F, TH2F, TProfile, gDirectory

class Histograms(object):

    def __init__(self, name):
        self.name = name
        self.hists = []
        self.named = []
        # attributes inheriting from TH1 and TNamed
        # are kept track of automagically, even if they are in
        # child classes
        # setting StatOverflows True for all histograms
        for var in vars( self ).values():
            try:
                if var.InheritsFrom('TNamed'):
                    self.named.append(var)
                    if var.InheritsFrom('TH1'):
                        var.StatOverflows(True)
                        self.hists.append(var)
            except:
                pass
        print 'TH1     list:', self.hists
        print 'TNamed  list:', self.named

    def FormatHistos(self, style ):
        for hist in self.hists:
            style.FormatHisto( hist )

    def Write(self, dir ):
        self.dir = dir.mkdir( self.name )
        self.dir.cd()
        for hist in self.hists:
            hist.Write()
        dir.cd()

        

class SubHistograms( Histograms ):
    def __init__(self, name):
        self.h_pt = TH1F(name+'_h_pt', ';p_{T} (GeV)', 40, 0, 100)
        super(SubHistograms, self).__init__( name )
