tauMuAllHists = ['mu/mu_h_eta',
                 'mu/mu_h_iso',
                 'mu/mu_h_iso_abs',
                 'mu/mu_h_iso_abs_dbeta',
                 'mu/mu_h_iso_ch',
                 'mu/mu_h_iso_dbeta',
                 'mu/mu_h_iso_nh',
                 'mu/mu_h_iso_ph',
                 'mu/mu_h_pt',
                 'tau/tau_h_eta',
                 'tau/tau_h_iso',
                 'tau/tau_h_iso_abs',
                 'tau/tau_h_iso_abs_dbeta',
                 'tau/tau_h_iso_ch',
                 'tau/tau_h_iso_dbeta',
                 'tau/tau_h_iso_nh',
                 'tau/tau_h_iso_ph',
                 'tau/tau_h_pt',
                 'tauMu/tauMu_h_mT',
                 'tauMu/tauMu_h_pzeta',
                 'tauMu/tauMu_h_svfitmass',
                 'tauMu/tauMu_h_vismass',
                 'vertex/vertex_h_nvertices' ]

def histogramSet( options, allHists=None):
    if allHists is None:
        allHists = tauMuAllHists
    hists = set()
    if options.histlist is not None:
        hists = set([ hist for hist in options.histlist.split(',') if hist is not '' ])
    elif options.histgroup is None:
        hists = set( allHists )
    else:
        histgroup = options.histgroup
        groups = [ group for group in histgroup.split(',') if group is not '']
        if 'TAUMU' in groups:
            hists.update([
                # 'mu/mu_h_iso_dbeta',
                # 'tau/tau_h_iso',
                # 'tau/tau_h_pt',
                'tauMu/tauMu_h_mT',
                #'tauMu/tauMu_h_pzeta',
                'tauMu/tauMu_h_svfitmass',
                'tauMu/tauMu_h_vismass',
                # 'vertex/vertex_h_nvertices'
                ])
        if 'TAU' in groups:
            hists.update([hist for hist in allHists if 'tau_' in hist])
        if 'MU' in groups:
            hists.update([hist for hist in allHists if 'mu_' in hist])          
          
    return hists

