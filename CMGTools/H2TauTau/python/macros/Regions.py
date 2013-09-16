import itertools

class RegionsBase( object ):
    '''Not necessary?'''
    def __init__(self):
        self.regions = {}
        
    def test(self, event):
        return None


class Regions1D( object ):
    '''Handles a set of regions along 1 dimension (region_name, min, max).
    ROOTOOLS
    The regions should be non-overlapping.'''

    def __init__(self, complete=False):
        self.regions = {}
        self.overlapChecked = False
        self.complete = complete

    def regionNames(self):
        names = self.regions.values()
        if not self.complete:
            names.append('None')
        return names

    def addRegion(self, name, min, max):
        self.regions[(min, max)] = name

    def _checkOverlap(self):
        self.overlapChecked = True
        overlap = False
        for region in self.regions.keys():
            min, max = region
            for other in self.regions.keys():
                omin, omax = other 
                if min < omin and omin < max:
                    return True
                if min < omax and omax < max:
                    return True
                
    def test(self, var):
        '''Returns the name of the region containing var, and None if such
        a region cannot be found.

        The algorithm might not be the fastest one around, but profiling
        said we do not care. 
        '''
        if not self.overlapChecked:
            overlap = self._checkOverlap()
            if overlap:
                raise ValueError( 'Please define non-overlapping regions' )
        last = None
        for region, name in sorted(self.regions.iteritems() ):
            min, max = region
            if min > var:
                break
            else:
                last = (min, max, name)
        # last is now the last range for which min < var
        if last is None:
            if self.complete:
                raise ValueError('you declared this region as complete, it is not.')
            return 'None'
        min, max, name = last
        if var < max:
            return name
        else:
            if self.complete:
                raise ValueError('you declared this region as complete, it is not.')
            return 'None'

    def __str__(self):
        tmp = '\n'.join( map(str, sorted(self.regions.iteritems() )) )
        return tmp


class JetRegions( object ):

    def __init__(self, cuts):
        self.cuts = cuts

    def regionNames(self):
        return ['VBF','Boosted','0or1Jet','Remains']

    def test(self, event):
        selJets = event.jets
        vbf = None 
        if len(selJets) > 1:
            vbf = event.vbf
        if len(selJets) > 1 and \
               vbf.mjj > self.cuts.VBF_Mjj and \
               vbf.deta > self.cuts.VBF_Deta and \
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


class MTRegions( Regions1D ):
    '''SM cuts'''
    def __init__(self, cuts):
        super( MTRegions, self ).__init__()
        self.addRegion('LowMT', 0, cuts.MT_low)
        self.addRegion('HighMT', cuts.MT_high, float('inf') )
        
    def test(self, event):
        ditau = event.diTau
        return super( MTRegions, self ).test( ditau.mTLeg2() )


class SignRegions( Regions1D ):
    '''Checks the charge of event.ditau'''
    def __init__(self):
        super( SignRegions, self ).__init__( complete=True )
        self.addRegion('OS', float('-inf'), 0)
        self.addRegion('SS', 0, float('inf'))

    def test(self, event):
        ditau = event.diTau
        return super( SignRegions, self ).test( ditau.leg1().charge() * \
                                                ditau.leg2().charge() )
        

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


    htt = H2TauTauRegions()
    print htt.regionNames()
    
