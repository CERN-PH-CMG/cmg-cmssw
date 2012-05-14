#!/bin/env python

import sys
import os
import re
import gzip

from CMGTools.Production.dataset import createDataset

def jobNumber( fileName ):
    m = pattern.match(fileName)
    return int(m.group(1))

def jobDir( allJobsDir, job ):
    return '{all}/Job_{job}'.format(all=allJobsDir,
                                    job=job)

def lsfReport( stdoutgz, unzip=False):
    sep_line = '-'*70
    print
    print sep_line
    print stdoutgz
    print
    stdout = None
    if unzip:
        stdout = gzip.open(stdoutgz)
    else:
        stdout = open(stdoutgz)
    for line in stdout.readlines()[-50:]:
        line = line.rstrip('\n')
        print line

def jobReport( allJobsDir, job ):
    jdir = jobDir( allJobsDir, job )
    for root, dirs, files in os.walk(jdir):
        stdout = 'STDOUT.gz'
        if stdout in files:
            lsfReport('/'.join( [root, stdout] ), True)
        stdout = 'STDOUT'       
        if stdout in files:
            lsfReport('/'.join( [root, stdout] ))

def jobSubmit( allJobsDir, job, cmd):    
    jdir = jobDir( allJobsDir, job )
    oldPwd = os.getcwd()
    os.chdir( jdir )
    print cmd
    os.system( cmd )
    os.chdir( oldPwd )
    
if __name__ == '__main__':

    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog [options] <dataset> <jobs_dir>
    
    Prints the list of bad jobs.
    Using the options, you can get a log of what happened during each bad job,
    and you can resubmit these jobs.
    """
    parser.add_option("-r", "--report", dest="report",
                      action = 'store_true',
                      default=False,
                      help='Print report for bad jobs.')
    parser.add_option("-s", "--submit", dest="submit",
                      action = 'store_true',
                      default=False,
                      help='Print resubmission command')
    parser.add_option("-u", "--user", dest="user", default=os.environ.get('USER', None),help='user owning the dataset.\nInstead of the username, give "LOCAL" to read datasets in a standard unix filesystem, and "CMS" to read official CMS datasets present at CERN.')
    parser.add_option("-p", "--pattern", dest="pattern", default='.*root',help='regexp pattern for root file printout')
    parser.add_option("-b", "--batch", dest="batch",
                      help="batch command. default is: 'bsub -q 8nh < batchScript.sh'. You can also use 'nohup < ./batchScript.sh &' to run locally.",
                      default="bsub -q 8nh < ./batchScript.sh")
    parser.add_option("-c", "--readcache", dest="readcache",
                      action = 'store_true',
                      default=False,
                      help='Read from the cache.')
    parser.add_option("-j", "--badjobs", dest="badjoblists",
                      default=None,
                      help='Lists of bad jobs, as [1,5];[2,5,7]')
    
    (options,args) = parser.parse_args()

    if len(args)!=2:
        print 'please provide the dataset name and the job directory in argument'
        sys.exit(1)
    
    dataset = args[0]
    allJobsDir = args[1]

    user = options.user
    pattern = options.pattern

    data = createDataset(user, dataset, pattern, options.readcache)

    files = data.listOfGoodFiles()
    # print files

    pattern = re.compile('.*_(\d+)\.root$')


    jobNumbers =  sorted( map( jobNumber, files ) ) 
    # print jobNumbers

    badJobs = []
    if options.badjoblists is None:
        prev = 0
        for job in jobNumbers:
            if job - prev != 1 and job!=0:
                for mj in range(prev+1, job):
                    badJobs.append( mj ) 
                    # print mj
            prev = job
    else:
        # import pdb; pdb.set_trace()
        bjlsstr = options.badjoblists.split(';')
        bjlsstr = filter(lambda x: len(x)>0, bjlsstr)
        bjls = map(eval, bjlsstr)
        setOfBadJobs = set()
        for bjl in bjls:
            setOfBadJobs.update( set(bjl) )
        # print setOfBadJobs
        # sys.exit(1)
        # print len(badJobs), 'bad jobs'
        # print badJobs
        badJobs = sorted( setOfBadJobs )

    if options.report:
        for job in badJobs:
            jobReport(allJobsDir, job)
    elif options.submit:
        for job in badJobs:
            jobSubmit(allJobsDir, job, options.batch)
    else:
        print badJobs
