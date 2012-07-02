from ROOT import TFile, TDirectory, TBrowser

def loadDesc(desc):
    spl = desc.split(':')
    file = spl[0]
    dir = None
    if len(spl)==2:
        dir = spl[1]
    return file, dir


def loadDescMap( fileName ):
    mapfile = open(fileName)
    filedirs = dict()
    for line in mapfile:
        spl = line.split()
        if len(spl) != 2:
            continue
        ifile, idir = loadDesc(spl[0])
        ofile, odir = loadDesc(spl[1])
        filedirs[(ifile,idir)] = ofile, odir
    return filedirs


def copyDirItems( idir, odir ):
    odir.cd()
    for key in idir.GetListOfKeys():
        obj = idir.Get(key.GetName())
        obj.Write(key.GetName())
    
def processRootFiles( descmap ):
    ifiles = dict()
    ofiles = dict()
    # odirs = []
    for input, output in descmap.iteritems():
        ifnam = input[0]
        ofnam = output[0]
        idnam = input[1]
        odnam = output[1]
        ifile = ifiles.get(ifnam, None)
        if ifile is None:
            ifile = TFile(ifnam)
            ifiles[ifnam] = ifile
        ofile = ofiles.get(ofnam, None)
        if ofile is None:
            ofile = TFile(ofnam,'recreate')
            ofiles[ofnam] = ofile
        idir = ifile
        if idnam:
            idir = ifile.Get(idnam)
        odir = ofile
        # import pdb; pdb.set_trace()
        if odnam:
            odir = ofile.Get(odnam)
            if odir == None:
                print 'mkdir', odnam
                odir = ofile.mkdir( odnam )
                # odirs.append(odir)
        copyDirItems( idir, odir )
    # ofile = ofiles['muTauSM_JoseJune17_tScale_mVis_Reb.root']
    ofile.cd()
    ofile.ls()
    for file in ofiles.values():
        file.Write()
    print ifiles
    print ofiles


if __name__ == '__main__':
    import sys

    descMap = loadDescMap( sys.argv[1] ) 
    processRootFiles( descMap )
