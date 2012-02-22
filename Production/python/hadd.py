import os
import pprint

def hadd(file, odir, idirs):
    if not file.endswith('.root'):
        return
    haddCmd = ['hadd']
    haddCmd.append( file.replace( idirs[0], odir ) )
    for dir in idirs:
        haddCmd.append( file.replace( idirs[0], dir ) )
    # import pdb; pdb.set_trace()
    cmd = ' '.join(haddCmd)
    print cmd
    os.system(cmd)


def haddRec(odir, idirs):
    print 'adding', idirs
    print 'to', odir 

    cmd = ' '.join( ['mkdir', odir])
    # os.system( cmd )
    try:
        os.mkdir( odir )
    except OSError:
        print 
        print 'ERROR: directory in the way. Maybe you ran hadd already in this directory? Remove it and try again'
        print 
        raise
    for root,dirs,files in os.walk( idirs[0] ):
        # print root, dirs, files
        for dir in dirs:
            dir = '/'.join([root, dir])
            dir = dir.replace(idirs[0], odir)
            cmd = 'mkdir ' + dir 
            # print cmd
            # os.system(cmd)
            os.mkdir(dir)
        for file in files:
            hadd('/'.join([root, file]), odir, idirs)

def haddChunks(idir):
    chunks = {}
    for file in sorted(os.listdir(idir)):
        filepath = '/'.join( [idir, file] )
        # print filepath
        if os.path.isdir(filepath):
            compdir = file
            try:
                prefix,num = compdir.split('_Chunk')
            except ValueError:
                # ok, not a chunk
                continue
            # print prefix, num
            chunks.setdefault( prefix, list() ).append(filepath)
    pprint.pprint(chunks)
    for comp, chunks in chunks.iteritems():
        odir = '/'.join( [idir, comp] )
        print odir, chunks
        haddRec(odir, chunks)

    
if __name__ == '__main__':
    import sys
    args = sys.argv
    # odir = args[1]
    # idirs = args[2:]
    # haddRec(odir, idirs)
    haddChunks(sys.argv[1])
