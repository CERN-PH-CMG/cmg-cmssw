import itertools

from CMGTools.RootTools.utils.Regions import RegionsBase, Regions1D


class MTRegions( Regions1D ):
    '''Checks mTLeg2 of event.diLepton'''
    def __init__(self, cuts):
        super( MTRegions, self ).__init__()
        self.addRegion('LowMT', 0, cuts.MT_low)
        self.addRegion('HighMT', cuts.MT_high, float('inf') )
        
    def test(self, event):
        diLepton = event.diLepton
        return super( MTRegions, self ).test( diLepton.mTLeg2() )


class SignRegions( Regions1D ):
    '''Checks the charge of event.diLepton'''
    def __init__(self):
        super( SignRegions, self ).__init__( complete=True )
        self.addRegion('OS', float('-inf'), 0)
        self.addRegion('SS', 0, float('inf'))

    def test(self, event):
        diLepton = event.diLepton
        return super( SignRegions, self ).test( diLepton.leg1().charge() * \
                                                diLepton.leg2().charge() )
        

class JetRegions( object ):

    def __init__(self, cuts):
        self.cuts = cuts

    def regionNames(self):
        return ['VBF','Boosted','0or1Jet','Remains']

    def test(self, event):
        selJets = event.cleanJets
        vbf = None 
        if len(selJets) > 1:
            # print event
            vbf = event.vbf
        if len(selJets) > 1 and \
               vbf.mjj > self.cuts.Mjj and \
               vbf.deta > self.cuts.deltaEta and \
               vbf.leadJets[0].eta() * vbf.leadJets[1].eta() < 0 and \
               len( vbf.centralJets ) == 0:
            return 'VBF'
        elif len(selJets) > 0 and selJets[0].pt()>self.cuts.Boosted_JetPt:
            # missing : veto on b-tagged jets in emu channel
            return 'Boosted'
        elif len(selJets)==0 or \
             ( len(selJets)==1 and selJets[0].pt()<self.cuts.Boosted_JetPt ):
            return '0or1Jet'
        else:
            return 'Remains'


class H2TauTauRegions( object ):
    '''Puts together SM regions'''
    def __init__(self, cuts):
        self.dimensions = []
        self.dimensions.append( MTRegions(cuts) )
        self.dimensions.append( SignRegions() )
        self.dimensions.append( JetRegions(cuts) )

    def test(self, event):
        # print 'H2TauTauRegions'
        all = []
        for dim in self.dimensions:
            all.append( dim.test( event ) )
        return '_'.join( all ) 

    def regionNames( self ):
        names = []
        for dim in self.dimensions:
            names.append(dim.regionNames())
        # yeah! 
        allCombs = list(itertools.product(*names))
        allCombNames = map( '_'.join, allCombs )
        return allCombNames            

        
if __name__ == '__main__':

    import CMGTools.RootTools.fwlite.Config as cfg

    vbfKwargs = dict( Mjj = 400,
                      deltaEta = 4.0    
                      )

    cuts = cfg.Analyzer(
        'Dummy',
        # vertexWeight = mc_vertexWeight,
        MT_low = 40,
        MT_high = 60,
        Boosted_JetPt = 150,
        **vbfKwargs
        )

    reg = H2TauTauRegions( cuts )
    print reg.regionNames()
    
