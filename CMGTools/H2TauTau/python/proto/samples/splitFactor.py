def splitFactor(comp, nEventsPerJob=2e4):
    split = int(comp.dataset_entries / nEventsPerJob)
    if split>len(comp.files): split = len(comp.files)
    if split==0: split = 1
    return split
    
