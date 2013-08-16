import FWCore.ParameterSet.Config as cms


diJetHistograms = cms.untracked.PSet(
    pt = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('pt()'),
            nbins = cms.untracked.int32(200),
            low = cms.untracked.double(0),
            high = cms.untracked.double(2000),
            title = cms.untracked.string("p_{T}")
            )
        ),
    eta = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('eta()'),
            nbins = cms.untracked.int32(50),
            low = cms.untracked.double(-5.),
            high = cms.untracked.double(5.),
            title = cms.untracked.string("#eta")
            )
        ),
    phi = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('phi()'),
            nbins = cms.untracked.int32(50),
            low = cms.untracked.double(-3.15),
            high = cms.untracked.double(3.15),
            title = cms.untracked.string("#phi")
            )
        ),
    mass = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('mass()'),
            nbins = cms.untracked.int32(350),
            low = cms.untracked.double(0),
            high = cms.untracked.double(3500),
            title = cms.untracked.string("M")
            )
        ),
    deltaPhi = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('deltaPhi(leg1().phi(), leg2().phi())'),
            nbins = cms.untracked.int32(10),
            low = cms.untracked.double(0),
            high = cms.untracked.double(3.15),
            title = cms.untracked.string("#Delta #phi_{1,2}")
            )
        ),

    deltaEta = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg1().eta() - leg2().eta()'),
            nbins = cms.untracked.int32(40),
            low = cms.untracked.double(-2),
            high = cms.untracked.double(2),
            title = cms.untracked.string("#Delta #eta_{1,2}")
            )
        ),

    Eta1_vs_Eta2 = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg1().eta()'),
            nbins = cms.untracked.int32(40),
            low = cms.untracked.double(-2),
            high = cms.untracked.double(2),
            title = cms.untracked.string("#eta_{1}")
            ),
        cms.untracked.PSet(
            var = cms.untracked.string('leg2().eta()'),
            nbins = cms.untracked.int32(40),
            low = cms.untracked.double(-2),
            high = cms.untracked.double(2),
            title = cms.untracked.string("#eta_{2}")
            )

        ),

    Phi1_vs_Phi2 = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg1().phi()'),
            nbins = cms.untracked.int32(30),
            low = cms.untracked.double(0),
            high = cms.untracked.double(6.16),
            title = cms.untracked.string("#phi_{1}")
            ),
        cms.untracked.PSet(
            var = cms.untracked.string('leg2().phi()'),
            nbins = cms.untracked.int32(30),
            low = cms.untracked.double(-3.15),
            high = cms.untracked.double(6.16),
            title = cms.untracked.string("#phi_{2}")
            )

        ),

    Pt1_vs_Pt2 = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg1().pt()'),
            nbins = cms.untracked.int32(200),
            low = cms.untracked.double(0),
            high = cms.untracked.double(2000),
            title = cms.untracked.string("p_{T, 1}")
            ),
        cms.untracked.PSet(
            var = cms.untracked.string('leg2().pt()'),
            nbins = cms.untracked.int32(200),
            low = cms.untracked.double(0),
            high = cms.untracked.double(2000),
            title = cms.untracked.string("p_{T, 2}")
            )

        ),


    pt1 = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg1().pt()'),
            nbins = cms.untracked.int32(200),
            low = cms.untracked.double(0),
            high = cms.untracked.double(2000),
            title = cms.untracked.string("p_{T,1}")
            )
        ),
    eta1 = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg1().eta()'),
            nbins = cms.untracked.int32(50),
            low = cms.untracked.double(-5.),
            high = cms.untracked.double(5.),
            title = cms.untracked.string("#eta_{1}")
            )
        ),
    phi1 = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg1().phi()'),
            nbins = cms.untracked.int32(50),
            low = cms.untracked.double(-3.15),
            high = cms.untracked.double(3.15),
            title = cms.untracked.string("#phi_{1}")
            )
        ),
    mass1 = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg1().mass()'),
            nbins = cms.untracked.int32(350),
            low = cms.untracked.double(0),
            high = cms.untracked.double(100),
            title = cms.untracked.string("M_{1}")
            )
        ),
   Eta1_vs_Phi1 = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg1().eta()'),
            nbins = cms.untracked.int32(40),
            low = cms.untracked.double(-2),
            high = cms.untracked.double(2),
            title = cms.untracked.string("#eta_{1}")
            ),
        cms.untracked.PSet(
            var = cms.untracked.string('leg1().phi()'),
            nbins = cms.untracked.int32(30),
            low = cms.untracked.double(-3.15),
            high = cms.untracked.double(3.15),
            title = cms.untracked.string("#phi_{1}")
            )

        ),


    pt2 = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg2().pt()'),
            nbins = cms.untracked.int32(200),
            low = cms.untracked.double(0),
            high = cms.untracked.double(2000),
            title = cms.untracked.string("p_{T,1}")
            )
        ),

    eta2 = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg2().eta()'),
            nbins = cms.untracked.int32(50),
            low = cms.untracked.double(-5.),
            high = cms.untracked.double(5.),
            title = cms.untracked.string("#eta_{1}")
            )
        ),
    phi2 = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg2().phi()'),
            nbins = cms.untracked.int32(50),
            low = cms.untracked.double(-3.15),
            high = cms.untracked.double(3.15),
            title = cms.untracked.string("#phi_{1}")
            )
        ),
    mass2 = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg2().mass()'),
            nbins = cms.untracked.int32(350),
            low = cms.untracked.double(0),
            high = cms.untracked.double(100),
            title = cms.untracked.string("M_{1}")
            )
        ),
    Eta2_vs_Phi2 = cms.untracked.VPSet(
        cms.untracked.PSet(
            var = cms.untracked.string('leg2().eta()'),
            nbins = cms.untracked.int32(40),
            low = cms.untracked.double(-2),
            high = cms.untracked.double(2),
            title = cms.untracked.string("#eta_{2}")
            ),
        cms.untracked.PSet(
            var = cms.untracked.string('leg2().phi()'),
            nbins = cms.untracked.int32(30),
            low = cms.untracked.double(-3.15),
            high = cms.untracked.double(3.15),
            title = cms.untracked.string("#phi_{2}")
            )

        )
    )



fatJets = cms.untracked.PSet(
    Size1 = cms.untracked.VPSet(
       cms.untracked.PSet(
           var = cms.untracked.string('leg1().numConstituents()-1'),
           nbins = cms.untracked.int32(8),
           low = cms.untracked.double(-0.5),
           high = cms.untracked.double(7.5),
           title = cms.untracked.string("N_{Rad, 1}")
           )
       ),
    Size2 = cms.untracked.VPSet(
       cms.untracked.PSet(
           var = cms.untracked.string('leg2().numConstituents()-1'),
           nbins = cms.untracked.int32(8),
           low = cms.untracked.double(-0.5),
           high = cms.untracked.double(7.5),
           title = cms.untracked.string("N_{Rad, 2}")
           )
       ),


    )










cmgDiJetHistograms = cms.EDAnalyzer(
    "CmgDiJetHistograms",
    inputCollection = cms.InputTag("cmgDiJet"),
    histograms = diJetHistograms
    )

cmgDiPFJetHistograms = cms.EDAnalyzer(
    "CmgDiPFJetHistograms",
    inputCollection = cms.InputTag("cmgDiPFJet"),
    histograms = diJetHistograms
    )

cmgDiFatJetHistograms = cms.EDAnalyzer(
    "CmgDiFatJetHistograms",
    inputCollection = cms.InputTag("cmgDiFatJet"),
    histograms = diJetHistograms
    )

cmgDiFatJetHistograms_FatJets = cms.EDAnalyzer(
    "CmgDiFatJetHistograms",
    inputCollection = cms.InputTag("cmgDiFatJet"),
    histograms = fatJets
    )
