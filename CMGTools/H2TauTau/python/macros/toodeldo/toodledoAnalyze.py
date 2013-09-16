import xml.dom.minidom as xml
import pprint
import datetime

from ROOT import gROOT, gStyle, TDatime, TH1F, TCanvas, gPad

# I want:
#   number of completed tasks per day

class Task(object):
    pass


def dayOfYear(date):
    datelist = [int(s) for s in date.split('/')]
    da = datetime.date( datelist[0], datelist[1], datelist[2])
    day = da.timetuple().tm_yday
    if da.year == 2012:
        day += 365
    return day

def extractTask(task):
    completed = task.getElementsByTagName('completed')[0].childNodes[0].nodeValue
    id = int(task.getElementsByTagName('id')[0].childNodes[0].nodeValue)
    # print id
    title = task.getElementsByTagName('title')
    text = title[0].childNodes[0].nodeValue
    try:
        print id, text
    except:
        print '!!!EXCEPTION',task, id
    task = Task()
    task.day = ''
    task.id = id
    task.title = title
    task.completed = completed
    return task


def makePlot(tasks):
    da = TDatime(2011,01,01,12,00,00);
    gStyle.SetTimeOffset(da.Convert());
  
    ct = TCanvas("ct","Time on axis");

    ndays = 390
    toodledo = TH1F("toodledo","toodledo",ndays,0.,ndays*86400);
    for i in range(1,toodledo.GetNbinsX()+1):
        try:
            num = tasks[i]
        except:
            num = 0
        toodledo.SetBinContent(i,num);
        
    ct.cd(2);
    toodledo.SetStats(0)
    toodledo.GetXaxis().SetLabelSize(0.04);
    toodledo.GetXaxis().SetTimeDisplay(1);
    toodledo.GetXaxis().SetNdivisions(5);
    toodledo.GetXaxis().SetTimeFormat("%d\/%m\/%y");
    toodledo.Draw();
    ct.Modified()
    ct.Update()
    return ct, toodledo
    

document = 'toodledo.xml'
dom = xml.parse(document)

domTasks = dom.getElementsByTagName('item')

# extractTask(tasks[0])

tasksPerDay = {}

for domTask in domTasks:
    task = extractTask(domTask)
    completedDay = task.completed.replace('-','/')
    if completedDay == '0000/00/00':
        continue
    completedDay = dayOfYear( completedDay )
    tasksPerDay.setdefault( completedDay, list() ).append(task)

nTasksPerDay = dict( (day, len(tasks)) for day, tasks in tasksPerDay.iteritems() )

pprint.pprint( sorted(nTasksPerDay.iteritems()) )

ct, hist = makePlot(nTasksPerDay)
