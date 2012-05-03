import os

def cmsswRelease():
    return os.environ['CMSSW_BASE'].split('/')[-1]

def cmsswIs44X():
    return cmsswRelease().find('CMSSW_4_4_') != -1
def cmsswIs52X():
    return cmsswRelease().find('CMSSW_5_2_') != -1

if __name__ == '__main__':
    print cmsswRelease()
    print 'is 44?', cmsswIs44X()
    print 'is 52?', cmsswIs52X()
