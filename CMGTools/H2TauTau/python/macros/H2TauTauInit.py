from CMGTools.RootTools.RootInit import *
from CMGTools.H2TauTau.macros.h2TauTauAliases import *
from CMGTools.H2TauTau.macros.H2TauTauHistogramList import *

# aliases  ----------------------------------------------------------


def H2TauTauInit(pattern):
    (events,lumi) = Init(pattern)
    # adding aliases specific to this analysis
    aliases = AliasSetter(events, h2TauTauAliases, 'H2TAUTAU')
    return events,lumi


if __name__ == '__main__':

    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog <pattern to root files>\n
    Initialize the environment and create a chain with these files.
    """

    (options,args) = parser.parse_args()
    if len(args)!=1:
        print 'please provide a pattern for your root files'
        sys.exit(1)
    pattern = args[0]

    print 'initializing h->tau tau root environment'
    (events,lumi) = H2TauTauInit( pattern )
