import FWCore.ParameterSet.Config as cms

## e.g. invertSelector(cmgCandSel.clone())
def invertSelector(sel):
    """Takes a selector and returns a clone with the inverse selection"""
    result = sel.clone()
    result.cut.setValue('!(%s)' % sel.cut.value())
    return result

# do not really on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.

cmgCandSel = cms.EDFilter(
    "CandViewSelector",
    src = cms.InputTag(""),
    cut = cms.string( "pt()>0" )
    )


