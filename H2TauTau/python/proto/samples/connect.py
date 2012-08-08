import sys
import pprint
import os
import re
import operator

from CMGTools.Production.cmgdbApi import CmgdbApi
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

db = CmgdbApi()
db.connect()

def findFirstAncestor(dataset_id, info):
    cols, rows = db.sql("select parent_dataset_id, path_name, primary_dataset_entries, number_total_jobs, number_files_missing, number_files_bad, dataset_fraction, task_id from dataset_details where dataset_id={dataset_id}".format(dataset_id=dataset_id))
    # pprint.pprint(rows)
    if len(rows)==0:
        print 'cannot find dataset with id', dataset_id
    elif len(rows)>1:
        assert(False)
    else:
        # print 'dataset', rows[0][1]
        parent_id = rows[0][0]
        info.append(rows[0])
        if parent_id is None:
            # print 'last in the DB'
            return 
        findFirstAncestor( parent_id, info )



class DatasetInfo(list):
    def get(self, stepName):
        matches = [ stepi for stepi in self if stepi['step']==stepName]
        return matches

    def __str__(self):
        theStrs = [
            'primary_dataset_entries = {nentries}'.format(nentries=self.primary_dataset_entries)
            ]
        theStrs.extend( map(str, self) )
        return '\n'.join(theStrs)

reTAU = re.compile('TAU\S+')
rePatPFAOD = re.compile('V\d+')
rePatPATCMG = re.compile('PAT_CMG\S+')


def processInfo(info):
    dsInfo = DatasetInfo()
    dsInfo.primary_dataset_entries = None
    for ds in info:
        job_eff = None
        fraction = None
        skim = False
        # print ds
        pid, path_name, pde, njobs, nmiss, nbad, dataset_fraction, task_id = ds
        # try to find the total number of entries in the CMS dataset
        if pde:
            if dsInfo.primary_dataset_entries is None:
                dsInfo.primary_dataset_entries=pde
            elif dsInfo.primary_dataset_entries != pde:
                print 'WARNING! there can only be one value for primary_dataset_entries in the history of a dataset, see task',task_id 
        # which step is that?
        base = os.path.basename(path_name)
        fraction = dataset_fraction
        # print path_name, base
        if reTAU.match(base):
            step = 'TAUTAU'
        elif rePatPFAOD.match(base):
            step = 'PFAOD'
        elif rePatPATCMG.match(base):
            step = 'PATCMG'
            if fraction:
                fraction /=2.
        else:
            step = 'Unknown'
        # print step
        # print 'fraction', fraction
        # processing efficiency using job information
        if njobs:
            # print njobs, nmiss, nbad
            job_eff = 1 - (nmiss + nbad)/float(njobs)
            # print 'job efficiency', job_eff
            if job_eff - fraction > 0.1:
                # high job efficiency, but low dataset_fraction.
                # print 'skimmin'
                skim = True
        else:
            pass
            # print 'WARNING, number_total_jobs not set for', path_name, 'see savannah task', task_id
        # storing info
        dsInfo.append( dict( path_name = path_name,
                             step = step,
                             jobeff=job_eff,
                             fraction=fraction,
                             skim=skim,
                             task_id=task_id
                             )
                       )
    return dsInfo
        


rePatMass = re.compile('M-(\d+)_')

def findAlias(path_name, aliases):
    name = None
    for dsname, alias in aliases.iteritems():
        if path_name.startswith(dsname):
            name = alias
    if name is None:
        return None
        # raise ValueError('dataset name not recognized: ' + path_name)
    match = rePatMass.search(path_name)
    if match and name != 'DYJets':
        mass = match.group(1)
        return name + mass
    else:
        return name 


def connectSample(components, row, filePattern, aliases, cache, verbose):
    id = row[0]
    path_name = row[1]
    file_owner = row[2]
    info = []
    findFirstAncestor(id, info)
    dsInfo = processInfo(info)
    if verbose:
        pprint.pprint( dsInfo )
    globalEff = 1.
    nEvents = dsInfo.primary_dataset_entries 
    taskurl = 'https://savannah.cern.ch/task/?{task_id}'.format(task_id=dsInfo[0]['task_id'])
    for step in dsInfo:
        if step['step']=='TAUTAU':
            eff = step['jobeff']
        elif step['step']=='PATCMG':
            eff = step['fraction']
        else:
            continue
        globalEff *= eff
    path_name = dsInfo[0]['path_name']
    compName = findAlias(path_name, aliases)
    if compName is None:
        print 'WARNING: cannot find alias for', path_name
        return False
    comps = [comp for comp in components if comp.name == compName]
    if len(comps)>1:
        print 'WARNING find several components for compName', compName
        print map(str, comps)
        return False
    elif len(comps)==0:
        print 'WARNING no component found for compName', compName
        # import pdb; pdb.set_trace()
        return False
    comp = comps[0]
    if not ( comp.name.startswith('data_') or \
             comp.name.startswith('embed_') ):
        comp.nGenEvents = nEvents
        comp.nGenEvents *= globalEff
    print 'LOADING:', comp.name, nEvents, globalEff, taskurl
    # print dsInfo
    comp.files = getFiles(path_name, file_owner, filePattern, cache)
    if comp.name.startswith('data_'):
        if globalEff<0.99:
            print 'ARGH! data sample is not complete.', taskurl
            print dsInfo
    else:
        if globalEff<0.9:
            print 'WEIRD! Efficiency is way too low! you might have to edit your cfg manually.'
            print dsInfo

    
def connect(components, samplePattern, filePattern, aliases, cache, verbose=False):
    pattern = samplePattern
    cols, rows = db.sql("select dataset_id, path_name, file_owner from dataset_details where path_name like '{pattern}' order by path_name".format(
        pattern = samplePattern
        ))
    for row in rows:
        connectSample(components, row, filePattern, aliases, cache, verbose)
        
    
    
if __name__ == '__main__':
    pass
