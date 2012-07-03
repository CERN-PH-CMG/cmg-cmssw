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


def file_dir(name):
    spl = name.split(':')
    file = spl[0]
    dir = None
    if len(spl)==2:
        file = spl[0]
        dir = spl[1]
    tfile = TFile(file)
    tdir = tfile 
    if dir:
        tdir = tfile.Get(dir)
    return tfile, tdir


def datacards(plot, category, shift=None, channel='muTau'):
    ext = None
    if shift:
        if shift == 'Up':
            ext = 'CMS_scale_tUp'
        elif shift == 'Down':
            ext = 'CMS_scale_tDown'
        else:
            raise ValueError('shift should be "Up", "Down", or None. You gave shift='+str(shift))
        
    fileName = '{channel}_{category}.root'.format(channel=channel,
                                                  category=category)
    if ext:
        fileName = '{channel}_{category}_{ext}.root'.format(channel=channel,
                                                          category=category,
                                                          ext=ext)
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
                subdir = output.mkdir(key.GetName())
                subdir.cd()
                subobjs = getobjs( obj )
                for subobj in subobjs:
                    subobj.Write()
            else:
                if categdir is None:
                    categdir = output.mkdir( categories[file.GetName()] )
                categdir.cd()
                obj.Write(key.GetName())
    output.Close()
