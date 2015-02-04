from DataFormats.FWLite import Events as FWLiteEvents
import os, subprocess, json

class EOSEventsWithDownload(object):
    def __init__(self, files, tree_name):
        query = ["edmFileUtil", "--ls", "-j"]+[("file:"+f if f[0]=="/" else f) for f in files]
        retjson = subprocess.check_output(query)
        retobj = json.loads(retjson)
        self._files = []
        self._nevents = 0
        for entry in retobj:
            self._files.append( (str(entry['file']), self._nevents, self._nevents+entry['events'] ) ) # str() is needed since the output is a unicode string
            self._nevents += entry['events']
        self._fileindex = -1
        self._localCopy = None
        self.events = None
    def __len__(self):
        return self._nevents
    def __getattr__(self, key):
        return getattr(self.events, key)
    def isLocal(self,filename):
        fpath = filename.replace("root://eoscms.cern.ch//","/").replace("root://eoscms//","/")
        if "?" in fpath: fpath = fpath.split["?"][0]
        try:
            finfo = subprocess.check_output(["/afs/cern.ch/project/eos/installation/pro/bin/eos.select", "fileinfo", fpath])
            replicas = False
            for line in finfo.split("\n"):
                if line.endswith("geotag"):
                    replicas = True
                elif replicas and ".cern.ch" in line:
                    geotag = int(line.split()[-1])
                    print "Found a replica with geotag %d" % geotag
                    return (geotag < 9000)
        except:
            pass
        return True
    def __getitem__(self, iEv):
        if self._fileindex == -1 or not(self._files[self._fileindex][1] <= iEv and iEv < self._files[self._fileindex][2]):
            self.events = None # so it's closed
            if self._localCopy:
                print "Removing local cache file %s" % self._localCopy
                os.remove(self._localCopy)
            for i,(fname,first,last) in enumerate(self._files):
                if first <= iEv and iEv < last:
                    print "For event range [ %d, %d ) will use file %r " % (first,last,fname)
                    self._fileindex = i
                    if fname.startswith("root://eoscms"):
                        if not self.isLocal(fname):
                            tmpdir = os.environ['TMPDIR'] if 'TMPDIR' in os.environ else "/tmp"
                            localfile = tmpdir+"/"+os.path.basename(fname)
                            try:
                                print "Filename %s is remote (geotag >= 9000), will do a copy to local path %s " % (fname,localfile)
                                subprocess.check_output(["xrdcp","-f",fname,localfile])
                                self._localCopy = localfile
                                fname = localfile
                            except:
                                print "Could not save file locally, will run from remote"
                    print "Will run from "+fname
                    self.events = FWLiteEvents([fname])
                    break
        self.events.to(iEv)
        return self
    def __del__(self):
        todelete = getattr(self,'_localCopy',None)
        if todelete:
            print "Removing local cache file ",todelete
            os.remove(todelete)

