import cPickle, re, optparse, os, os.path


def convertFile(filename,pattern,verbose=0):
    if "/store" not in filename: 
        raise RuntimeError, "%s doesn't look like a PFN" % filename
    lfn = re.sub(".*(/store/.*)(?:\\?.*)?","\\1",filename)
    if verbose: print "\t%s -> %s" % (filename, (pattern % lfn))
    return pattern % lfn

def convertComponent(comp,pattern,verbose=0):
    comp.files = [ convertFile(f,pattern,verbose) for f in comp.files ]
    return comp

def convertPickled(filename,pattern,verbose=0):
    if os.path.isdir(filename):
        if os.path.exists(filename+"/config.pck"):
            filename += "/config.pck"
    if not os.path.exists(filename):
        raise RuntimeError, "Filename '%s' does not exist" % filename
    if not os.path.exists(filename+".sav"):
        if verbose: print "Making backup in %s.sav" % filename
        os.system("cp %s %s.sav" % (filename, filename))
    fin = open(filename+".sav")
    comp = cPickle.load(fin)
    fin.close()
    if verbose: print "Loaded %s (%s) - %d files." % (filename, comp.name, len(comp.files))
    comp = convertComponent(comp,pattern,verbose)
    fout = open(filename, 'w')
    cPickle.dump(comp,fout)
    fout.close()
    print "Converted %s (%s) using %s - %d files." % (filename, comp.name, pattern, len(comp.files))

if __name__ == '__main__':
    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option("-v", dest="verbose", action="store_const", const = 1, default=0, help="be verbose")
    parser.add_option("-p", "--pattern", dest="pattern", default="root://cms-xrd-global.cern.ch/%s", help="Use this pattern to convert lfn in pfn; must contain '%s' ")
    parser.add_option("-e", "--eos", dest="pattern", action="store_const", const = 'root://eoscms.cern.ch//eos/cms%s',
                      help="Use eos.cern.ch to access the files")
    parser.add_option("-A", "--AAA", dest="pattern", action="store_const", const = "root://cms-xrd-global.cern.ch/%s",
                      help="Use AAA to access the files, with the global redirector")
    (options, args) = parser.parse_args()
    for a in args:
        convertPickled(a,options.pattern,options.verbose)
