def isLowDensity(component):
    nfiles = len(component.files)
    return nfiles>200 and \
           component.dataset_entries/float(nfiles)<500


def groupingProposal(component, nperfile):
    nfiles = len(component.files)
    cnpf = component.dataset_entries / float(nfiles)
    grouping = int(nperfile / cnpf)
    res = dict( name = component.name,
                entries = component.dataset_entries,
                grouping = grouping,
                entriespergroup = cnpf * grouping,
                njobs = nfiles / grouping
                )
    return res


def density(components, npf = 5e3):
    big = filter( isLowDensity, components )
    for c in big:
        print groupingProposal(c, npf)
