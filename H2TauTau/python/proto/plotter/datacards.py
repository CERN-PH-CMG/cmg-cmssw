from ROOT import TH1F,TFile

aliases = {
    'Ztt':'ZTT',
    'WJets':'W',
    'TTJets':'TT',
    'QCD':'QCD',
    'VV':'VV',
    #
    'Data':'data_obs',
    # VBF
    'HiggsVBF110':'qqH110',
    'HiggsVBF115':'qqH115',
    'HiggsVBF120':'qqH120',
    'HiggsVBF125':'qqH125',
    'HiggsVBF130':'qqH130',
    'HiggsVBF135':'qqH135',
    'HiggsVBF140':'qqH140',
    'HiggsVBF145':'qqH145',
    # gluon fusion
    'HiggsGGH110':'ggH110',
    'HiggsGGH115':'ggH115',
    'HiggsGGH120':'ggH120',
    'HiggsGGH125':'ggH125',
    'HiggsGGH130':'ggH130',
    'HiggsGGH135':'ggH135',
    'HiggsGGH140':'ggH140',
    'HiggsGGH145':'ggH145',
    # VH
    'HiggsVH110':'VH110',
    'HiggsVH115':'VH115',
    'HiggsVH120':'VH120',
    'HiggsVH125':'VH125',
    'HiggsVH130':'VH130',
    'HiggsVH135':'VH135',
    'HiggsVH140':'VH140',
    'HiggsVH145':'VH145',
    }



def datacards(plot, category, channel='tauMu'):
    fileName = '{channel}_{category}.root'.format(channel=channel,
                                                  category=category)
    file = TFile(fileName, 'recreate')
    print 'output file', fileName
    for myName, hist in plot.histosDict.iteritems():
        print myName, hist.name
        rogerName = aliases.get(myName, None)
        if rogerName is not None:
            print 'writing', myName, 'as', rogerName
            hist.weighted.Write(rogerName)
    file.Close()
