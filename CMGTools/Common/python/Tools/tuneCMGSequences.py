import FWCore.ParameterSet.Config as cms

def removeObject( process, object, postfix ):
    # need to target the correct sequence not to break them 

    try:
        obj  = getattr( process, object + 'Sequence' + postfix)
        getattr( process, 'cmgObjectSequence'+postfix ).remove( obj )
    except:
        pass
        

    if object!='met':
        # no cut summary for met yet
        try:
            cutSummary = getattr( process, object + 'CutSummarySequence' + postfix )
            getattr( process, 'cutSummarySequence'+postfix ).remove( cutSummary )
        except:
            pass

    try: 
        hist = getattr( process, object + 'HistogramSequence' + postfix ) 
        getattr( process, 'histogramSequence'+postfix ).remove( hist )
    except:
        pass

    try:
        size = getattr( process, object + 'SelSize' + postfix )
        getattr( process, 'collectionSizeSequence'+postfix ).remove( size )
    except:
        pass

    try:
        skim = getattr( process, object + 'SkimSequence' + postfix )
        getattr( process, 'skimSequence'+postfix ).remove( skim )
    except:
        pass


def tuneCMGSequences(process, postpostfix=''):
    # removeObject( process, 'tau', 'AK5NoPUSub'+postpostfix)
    removeObject( process, 'muon', 'AK5NoPUSub'+postpostfix)
    removeObject( process, 'electron', 'AK5NoPUSub'+postpostfix)
    removeObject( process, 'met', 'AK5NoPUSub'+postpostfix)
    removeObject( process, 'trigger', 'AK5NoPUSub'+postpostfix)
