import copy
from ROOT import TH1F,TFile,TDirectoryFile

datacards_aliases = {
    'Ztt':'ZTT',
    'Ztt_ZL':'ZL',
    'Ztt_ZJ':'ZJ',
    'WJets':'W',
    'TTJets':'TT',
    'QCD':'QCD',
    'VV':'VV',
    #
    'Data':'data_obs',
    # VBF
    'HiggsVBF90':'qqH90',
    'HiggsVBF95':'qqH95',
    'HiggsVBF100':'qqH100',
    'HiggsVBF105':'qqH105',
    'HiggsVBF110':'qqH110',
    'HiggsVBF115':'qqH115',
    'HiggsVBF120':'qqH120',
    'HiggsVBF125':'qqH125',
    'HiggsVBF130':'qqH130',
    'HiggsVBF135':'qqH135',
    'HiggsVBF140':'qqH140',
    'HiggsVBF145':'qqH145',
    'HiggsVBF150':'qqH150',
    'HiggsVBF155':'qqH155',
    'HiggsVBF160':'qqH160',
    # gluon fusion
    'HiggsGGH90':'ggH90',
    'HiggsGGH95':'ggH95',
    'HiggsGGH100':'ggH100',
    'HiggsGGH105':'ggH105',
    'HiggsGGH110':'ggH110',
    'HiggsGGH115':'ggH115',
    'HiggsGGH120':'ggH120',
    'HiggsGGH125':'ggH125',
    'HiggsGGH130':'ggH130',
    'HiggsGGH135':'ggH135',
    'HiggsGGH140':'ggH140',
    'HiggsGGH145':'ggH145',
    'HiggsGGH150':'ggH150',
    'HiggsGGH155':'ggH155',
    'HiggsGGH160':'ggH160',
    # VH
    'HiggsVH90':'VH90',
    'HiggsVH95':'VH95',
    'HiggsVH190':'VH100',
    'HiggsVH195':'VH105',
    'HiggsVH110':'VH110',
    'HiggsVH115':'VH115',
    'HiggsVH120':'VH120',
    'HiggsVH125':'VH125',
    'HiggsVH130':'VH130',
    'HiggsVH135':'VH135',
    'HiggsVH140':'VH140',
    'HiggsVH145':'VH145',
    'HiggsVH150':'VH150',
    'HiggsVH155':'VH155',
    'HiggsVH160':'VH160',
    }




def datacards(plot, cutstring, shift, channel='muTau', prefix=None, energy='8TeV'):

    category = 'Unknown'
    if cutstring.find('Xcat_IncX')!=-1:
        category = 'inclusive'

    # Moriond categories
    if cutstring.find('Xcat_J1X')!=-1:
        category = 'boosted'
        if cutstring.find('l1_pt<40')!=-1:
            category = 'boost_low'
        elif cutstring.find('l1_pt>40')!=-1:
            category = 'boost_high'
    if cutstring.find('Xcat_J0X')!=-1:
        category = '0jet'
        if cutstring.find('l1_pt<40')!=-1:
            category = '0jet_low'
        elif cutstring.find('l1_pt>40')!=-1:
            category = '0jet_high'
    if cutstring.find('Xcat_VBF')!=-1:
        category = 'vbf'

    # Summer 13 categories
    if cutstring.find('Xcat_J0_lowX')!=-1:
        category = '0jet_low'
    elif cutstring.find('Xcat_J0_mediumX')!=-1:
        category = '0jet_medium'
    elif cutstring.find('Xcat_J0_highX')!=-1:
        category = '0jet_high'

    if cutstring.find('Xcat_J1_high_mediumhiggsX')!=-1:
        category = '1jet_high_mediumhiggs'
    elif cutstring.find('Xcat_J1_high_lowhiggsX')!=-1:
        category = '1jet_high_lowhiggs'
    elif cutstring.find('Xcat_J1_mediumX')!=-1:
        category = '1jet_medium'
    
    if cutstring.find('Xcat_VBF_tightX')!=-1:
        category = 'vbf_tight'
    elif cutstring.find('Xcat_VBF_looseX')!=-1:
        category = 'vbf_loose'

    if cutstring.find('Xcat_J1BX')!=-1:
        category = 'btag'
    elif cutstring.find('Xcat_0BX')!=-1:
        category = 'nobtag'

    ext = None
    if shift == 'Up' or shift == 'Down':
        ch = 'mutau'
        if channel.lower()=='eletau':
            ch = 'etau'
        ext = 'CMS_scale_t_{chan}_{energy}{shift}'.format(
            chan=ch,
            energy=energy,
            shift=shift
            )
    elif shift:
        ext = shift

    fileName = '{channel}_{category}.root'.format(channel=channel,
                                                  category=category)
    if ext:
        fileName = '{channel}_{category}_{ext}.root'.format(channel=channel,
                                                          category=category,
                                                          ext=ext)
    if prefix:
        fileName = '_'.join([prefix, fileName])
        
    file = TFile(fileName, 'recreate')
    print 'output file', fileName
    zttzl = None
    zttzj = None
    for myName, hist in sorted(plot.histosDict.iteritems()):
        rogerName = datacards_aliases.get(myName, None)
        if rogerName is not None:
            theName = rogerName
            if ext:
                if rogerName=='data_obs':
                    # data not written for shifted samples
                    continue
                theName = '_'.join([rogerName,ext])
            print 'writing', myName, 'as', theName
            hist.weighted.Write( theName )
            if myName == 'Ztt_ZL':
                zttzl = copy.deepcopy(hist)
            if myName == 'Ztt_ZJ':
                zttzj = copy.deepcopy(hist)
    if zttzl and zttzj:
        name = 'ZLL'
        if ext:
            name = '_'.join( [name, ext])
        print 'writing', name
        zttzl.Add(zttzj)
        zttzl.weighted.Write(name)
    file.Close()


def getobjs( dir ):
    objs = []
    for key in dir.GetListOfKeys():
        obj = dir.Get(key.GetName())
        objs.append(obj)
    return objs

def merge( fileNames ):
    lastchan = None
    files = []
    categories = {}
    for fnam in fileNames:
        chan = fnam.split('_',1)[0]
        categ = fnam.split('_',1)[1].split('.')[0]
        categories[fnam] = categ
        if lastchan is not None and chan!=lastchan:
            print lastchan, chan
            raise ValueError('cannot add different channels:', str(fileNames))
        files.append( TFile(fnam) )
        lastchan = chan
    output = TFile('.'.join([lastchan,'root']), 'recreate')
    for file in files:
        print file.GetName()
        categdir = None
        for key in file.GetListOfKeys():
            output.cd()
            obj = file.Get(key.GetName())
            if type(obj) is TDirectoryFile:
                # subdir = output.mkdir(key.GetName())
                subdir = output.mkdir(lastchan + '_' + key.GetName())
                subdir.cd()
                subobjs = getobjs( obj )
                for subobj in subobjs:
                    subobj.Write()
            else:
                if categdir is None:
                    # categdir = output.mkdir( categories[file.GetName()] )
                    categdir = output.mkdir(lastchan + '_' + categories[file.GetName()] )
                categdir.cd()
                obj.Write(key.GetName())
    output.Close()
