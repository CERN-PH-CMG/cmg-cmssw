import FWCore.ParameterSet.Config as cms

def removeObject( process, object, postfix ):
    # need to target the correct sequence not to break them 

    obj  = getattr( process, object + 'Sequence' + postfix)
    getattr( process, 'cmgObjectSequence'+postfix ).remove( obj )

    if object!='met':
        # no cut summary for met yet
        cutSummary = getattr( process, object + 'CutSummarySequence' + postfix )
        getattr( process, 'cutSummarySequence'+postfix ).remove( cutSummary )

    hist = getattr( process, object + 'HistogramSequence' + postfix ) 
    getattr( process, 'histogramSequence'+postfix ).remove( hist )

    skim = getattr( process, object + 'SkimSequence' + postfix )
    getattr( process, 'skimSequence'+postfix ).remove( skim )
    


def tuneCMGSequences(process):
    removeObject( process, 'tau', 'AK7')
    removeObject( process, 'muon', 'AK7')
    removeObject( process, 'electron', 'AK7')
    removeObject( process, 'met', 'AK7')
    removeObject( process, 'met', 'AK5LC')
