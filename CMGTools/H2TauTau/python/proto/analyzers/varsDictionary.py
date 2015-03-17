# change the branch names here

vars = {}

# Event ID
vars['run'                                        ] = {'std':'run'                                        , 'sync':'run'                                       }
vars['lumi'                                       ] = {'std':'lumi'                                       , 'sync':'lumi'                                      }
vars['event'                                      ] = {'std':'event'                                      , 'sync':'evt'                                       }

# Generator info
vars['isZtt'                                      ] = {'std':'isZtt'                                      , 'sync':'isZtt'                                     }
vars['isZmt'                                      ] = {'std':'isZmt'                                      , 'sync':'isZmt'                                     }
vars['isZet'                                      ] = {'std':'isZet'                                      , 'sync':'isZet'                                     }
vars['isZee'                                      ] = {'std':'isZee'                                      , 'sync':'isZee'                                     }
vars['isZmm'                                      ] = {'std':'isZmm'                                      , 'sync':'isZmm'                                     }
vars['isZem'                                      ] = {'std':'isZem'                                      , 'sync':'isZem'                                     }
vars['isZEE'                                      ] = {'std':'isZEE'                                      , 'sync':'isZEE'                                     }
vars['isZMM'                                      ] = {'std':'isZMM'                                      , 'sync':'isZMM'                                     }
vars['isZLL'                                      ] = {'std':'isZLL'                                      , 'sync':'isZLL'                                     }
vars['isFake'                                     ] = {'std':'isFake'                                     , 'sync':'isFake'                                    }

# Weights
vars['weight'                                     ] = {'std':'weight'                                     , 'sync':'weight'                                    }
vars['vertexWeight'                               ] = {'std':'vertexWeight'                               , 'sync':'puweight'                                  }

# PileUp
vars['NUP'                                        ] = {'std':'NUP'                                        , 'sync':'NUP'                                       }
vars['nVert'                                      ] = {'std':'nVert'                                      , 'sync':'npv'                                       }
vars['npu'                                        ] = {'std':'npu'                                        , 'sync':'npu'                                       }
vars['rho'                                        ] = {'std':'rho'                                        , 'sync':'rho'                                       }

# Leg 1 (tau, mu, ele)
vars['l1_pt'                                      ] = {'std':'l1_pt'                                      , 'sync':'pt_1'                                      }
vars['l1_phi'                                     ] = {'std':'l1_phi'                                     , 'sync':'phi_1'                                     }
vars['l1_eta'                                     ] = {'std':'l1_eta'                                     , 'sync':'eta_1'                                     }
vars['l1_mass'                                    ] = {'std':'l1_mass'                                    , 'sync':'m_1'                                       }
vars['l1_charge'                                  ] = {'std':'l1_charge'                                  , 'sync':'q_1'                                       }
vars['l1_dxy'                                     ] = {'std':'l1_dxy'                                     , 'sync':'d0_1'                                      }
vars['l1_dz'                                      ] = {'std':'l1_dz'                                      , 'sync':'dZ_1'                                      }
vars['mtleg1'                                     ] = {'std':'mtleg1'                                     , 'sync':'mt_1'                                      }
vars['l1_relIso05'                                ] = {'std':'l1_relIso05'                                , 'sync':'iso_1'                                     }
vars['l1_MlooseId'                                ] = {'std':'l1_MlooseId'                                , 'sync':'id_m_loose_1'                              }
vars['l1_MmediumId'                               ] = {'std':'l1_MmediumId'                               , 'sync':'id_m_medium_1'                             }
vars['l1_MtightId'                                ] = {'std':'l1_MtightId'                                , 'sync':'id_m_tight_1'                              }
vars['l1_MtightNoVtxId'                           ] = {'std':'l1_MtightNoVtxId'                           , 'sync':'id_m_tightnovtx_1'                         }
vars['l1_MhighPtId'                               ] = {'std':'l1_MhighPtId'                               , 'sync':'id_m_highpt_1'                             }
vars['l1_EmvaNonTrigLoose'                        ] = {'std':'l1_EmvaNonTrigLoose'                        , 'sync':'id_e_mva_nt_loose_1'                       }
vars['l1_EmvaNonTrigTight'                        ] = {'std':'l1_EmvaNonTrigTight'                        , 'sync':'id_e_mva_nt_loose_1'                       }
vars['l1_EvetoId'                                 ] = {'std':'l1_EvetoId'                                 , 'sync':'id_e_cut_veto_1'                           }
vars['l1_ElooseId'                                ] = {'std':'l1_ElooseId'                                , 'sync':'id_e_cut_loose_1'                          }
vars['l1_EmediumId'                               ] = {'std':'l1_EmediumId'                               , 'sync':'id_e_cut_medium_1'                         }
vars['l1_EtightId'                                ] = {'std':'l1_EtightId'                                , 'sync':'id_e_cut_tight_1'                          }
vars['l1_triggerWeight'                           ] = {'std':'l1_triggerWeight'                           , 'sync':'trigweight_1'                              }
vars['l1_againstElectronLooseMVA5'                ] = {'std':'l1_againstElectronLooseMVA5'                , 'sync':'againstElectronLooseMVA5_1'                }
vars['l1_againstElectronMediumMVA5'               ] = {'std':'l1_againstElectronMediumMVA5'               , 'sync':'againstElectronMediumMVA5_1'               }
vars['l1_againstElectronTightMVA5'                ] = {'std':'l1_againstElectronTightMVA5'                , 'sync':'againstElectronTightMVA5_1'                }
vars['l1_againstElectronVLooseMVA5'               ] = {'std':'l1_againstElectronVLooseMVA5'               , 'sync':'againstElectronVLooseMVA5_1'               }
vars['l1_againstElectronVTightMVA5'               ] = {'std':'l1_againstElectronVTightMVA5'               , 'sync':'againstElectronVTightMVA5_1'               }
vars['l1_againstMuonLoose3'                       ] = {'std':'l1_againstMuonLoose3'                       , 'sync':'againstMuonLoose3_1'                       }
vars['l1_againstMuonTight3'                       ] = {'std':'l1_againstMuonTight3'                       , 'sync':'againstMuonTight3_1'                       }
vars['l1_byCombinedIsolationDeltaBetaCorrRaw3Hits'] = {'std':'l1_byCombinedIsolationDeltaBetaCorrRaw3Hits', 'sync':'byCombinedIsolationDeltaBetaCorrRaw3Hits_1'}
vars['l1_byIsolationMVA3newDMwoLTraw'             ] = {'std':'l1_byIsolationMVA3newDMwoLTraw'             , 'sync':'byIsolationMVA3newDMwoLTraw_1'             }
vars['l1_byIsolationMVA3oldDMwoLTraw'             ] = {'std':'l1_byIsolationMVA3oldDMwoLTraw'             , 'sync':'byIsolationMVA3oldDMwoLTraw_1'             }
vars['l1_byIsolationMVA3newDMwLTraw'              ] = {'std':'l1_byIsolationMVA3newDMwLTraw'              , 'sync':'byIsolationMVA3newDMwLTraw_1'              }
vars['l1_byIsolationMVA3oldDMwLTraw'              ] = {'std':'l1_byIsolationMVA3oldDMwLTraw'              , 'sync':'byIsolationMVA3oldDMwLTraw_1'              }
vars['l1_chargedIsoPtSum'                         ] = {'std':'l1_chargedIsoPtSum'                         , 'sync':'chargedIsoPtSum_1'                         }
vars['l1_decayModeFinding'                        ] = {'std':'l1_decayModeFinding'                        , 'sync':'decayModeFinding_1'                        }
vars['l1_decayModeFindingNewDMs'                  ] = {'std':'l1_decayModeFindingNewDMs'                  , 'sync':'decayModeFindingNewDMs_1'                  }
vars['l1_neutralIsoPtSum'                         ] = {'std':'l1_neutralIsoPtSum'                         , 'sync':'neutralIsoPtSum_1'                         }
vars['l1_puCorrPtSum'                             ] = {'std':'l1_puCorrPtSum'                             , 'sync':'puCorrPtSum_1'                             }

# Leg 2 (tau, mu, ele)
vars['l2_pt'                                      ] = {'std':'l2_pt'                                      , 'sync':'pt_2'                                      }
vars['l2_phi'                                     ] = {'std':'l2_phi'                                     , 'sync':'phi_2'                                     }
vars['l2_eta'                                     ] = {'std':'l2_eta'                                     , 'sync':'eta_2'                                     }
vars['l2_mass'                                    ] = {'std':'l2_mass'                                    , 'sync':'m_2'                                       }
vars['l2_charge'                                  ] = {'std':'l2_charge'                                  , 'sync':'q_2'                                       }
vars['l2_dxy'                                     ] = {'std':'l2_dxy'                                     , 'sync':'d0_2'                                      }
vars['l2_dz'                                      ] = {'std':'l2_dz'                                      , 'sync':'dZ_2'                                      }
vars['mtleg2'                                     ] = {'std':'mtleg2'                                     , 'sync':'mt_2'                                      }
vars['l2_relIso05'                                ] = {'std':'l2_relIso05'                                , 'sync':'iso_2'                                     }
vars['l2_MlooseId'                                ] = {'std':'l2_MlooseId'                                , 'sync':'id_m_loose_2'                              }
vars['l2_MmediumId'                               ] = {'std':'l2_MmediumId'                               , 'sync':'id_m_medium_2'                             }
vars['l2_MtightId'                                ] = {'std':'l2_MtightId'                                , 'sync':'id_m_tight_2'                              }
vars['l2_MtightNoVtxId'                           ] = {'std':'l2_MtightNoVtxId'                           , 'sync':'id_m_tightnovtx_2'                         }
vars['l2_MhighPtId'                               ] = {'std':'l2_MhighPtId'                               , 'sync':'id_m_highpt_2'                             }
vars['l2_EmvaNonTrigLoose'                        ] = {'std':'l2_EmvaNonTrigLoose'                        , 'sync':'id_e_mva_nt_loose_2'                       }
vars['l2_EmvaNonTrigTight'                        ] = {'std':'l2_EmvaNonTrigTight'                        , 'sync':'id_e_mva_nt_loose_2'                       }
vars['l2_EvetoId'                                 ] = {'std':'l2_EvetoId'                                 , 'sync':'id_e_cut_veto_2'                           }
vars['l2_ElooseId'                                ] = {'std':'l2_ElooseId'                                , 'sync':'id_e_cut_loose_2'                          }
vars['l2_EmediumId'                               ] = {'std':'l2_EmediumId'                               , 'sync':'id_e_cut_medium_2'                         }
vars['l2_EtightId'                                ] = {'std':'l2_EtightId'                                , 'sync':'id_e_cut_tight_2'                          }
vars['l2_triggerWeight'                           ] = {'std':'l2_triggerWeight'                           , 'sync':'trigweight_2'                              }
vars['l2_againstElectronLooseMVA5'                ] = {'std':'l2_againstElectronLooseMVA5'                , 'sync':'againstElectronLooseMVA5_2'                }
vars['l2_againstElectronMediumMVA5'               ] = {'std':'l2_againstElectronMediumMVA5'               , 'sync':'againstElectronMediumMVA5_2'               }
vars['l2_againstElectronTightMVA5'                ] = {'std':'l2_againstElectronTightMVA5'                , 'sync':'againstElectronTightMVA5_2'                }
vars['l2_againstElectronVLooseMVA5'               ] = {'std':'l2_againstElectronVLooseMVA5'               , 'sync':'againstElectronVLooseMVA5_2'               }
vars['l2_againstElectronVTightMVA5'               ] = {'std':'l2_againstElectronVTightMVA5'               , 'sync':'againstElectronVTightMVA5_2'               }
vars['l2_againstMuonLoose3'                       ] = {'std':'l2_againstMuonLoose3'                       , 'sync':'againstMuonLoose3_2'                       }
vars['l2_againstMuonTight3'                       ] = {'std':'l2_againstMuonTight3'                       , 'sync':'againstMuonTight3_2'                       }
vars['l2_byCombinedIsolationDeltaBetaCorrRaw3Hits'] = {'std':'l2_byCombinedIsolationDeltaBetaCorrRaw3Hits', 'sync':'byCombinedIsolationDeltaBetaCorrRaw3Hits_2'}
vars['l2_byIsolationMVA3newDMwoLTraw'             ] = {'std':'l2_byIsolationMVA3newDMwoLTraw'             , 'sync':'byIsolationMVA3newDMwoLTraw_2'             }
vars['l2_byIsolationMVA3oldDMwoLTraw'             ] = {'std':'l2_byIsolationMVA3oldDMwoLTraw'             , 'sync':'byIsolationMVA3oldDMwoLTraw_2'             }
vars['l2_byIsolationMVA3newDMwLTraw'              ] = {'std':'l2_byIsolationMVA3newDMwLTraw'              , 'sync':'byIsolationMVA3newDMwLTraw_2'              }
vars['l2_byIsolationMVA3oldDMwLTraw'              ] = {'std':'l2_byIsolationMVA3oldDMwLTraw'              , 'sync':'byIsolationMVA3oldDMwLTraw_2'              }
vars['l2_chargedIsoPtSum'                         ] = {'std':'l2_chargedIsoPtSum'                         , 'sync':'chargedIsoPtSum_2'                         }
vars['l2_decayModeFinding'                        ] = {'std':'l2_decayModeFinding'                        , 'sync':'decayModeFinding_2'                        }
vars['l2_decayModeFindingNewDMs'                  ] = {'std':'l2_decayModeFindingNewDMs'                  , 'sync':'decayModeFindingNewDMs_2'                  }
vars['l2_neutralIsoPtSum'                         ] = {'std':'l2_neutralIsoPtSum'                         , 'sync':'neutralIsoPtSum_2'                         }
vars['l2_puCorrPtSum'                             ] = {'std':'l2_puCorrPtSum'                             , 'sync':'puCorrPtSum_2'                             }

# di-tau pair
vars['pthiggs'                                    ] = {'std':'pthiggs'                                    , 'sync':'pth'                                       }
vars['visMass'                                    ] = {'std':'visMass'                                    , 'sync':'m_vis'                                     }
vars['svfitMass'                                  ] = {'std':'svfitMass'                                  , 'sync':'m_sv'                                      }
vars['svfitPt'                                    ] = {'std':'svfitPt'                                    , 'sync':'pt_sv'                                     }
vars['svfitEta'                                   ] = {'std':'svfitEta'                                   , 'sync':'eta_sv'                                    }
vars['svfitPhi'                                   ] = {'std':'svfitPhi'                                   , 'sync':'phi_sv'                                    }
vars['svfitMET'                                   ] = {'std':'svfitMET'                                   , 'sync':'met_sv'                                    }

# MET
vars['pfmet_pt'                                   ] = {'std':'pfmet_pt'                                   , 'sync':'met'                                       }
vars['pfmet_phi'                                  ] = {'std':'pfmet_phi'                                  , 'sync':'metphi'                                    }
vars['met'                                        ] = {'std':'met'                                        , 'sync':'mvamet'                                    }
vars['metphi'                                     ] = {'std':'metphi'                                     , 'sync':'mvametphi'                                 }
vars['pZetaVis'                                   ] = {'std':'pZetaVis'                                   , 'sync':'pzetavis'                                  }
vars['pZetaMET'                                   ] = {'std':'pZetaMET'                                   , 'sync':'pzetamiss'                                 }

vars['metcov00'                                   ] = {'std':'metcov00'                                   , 'sync':'mvacov00'                                  }
vars['metcov01'                                   ] = {'std':'metcov01'                                   , 'sync':'mvacov01'                                  }
vars['metcov10'                                   ] = {'std':'metcov10'                                   , 'sync':'mvacov10'                                  }
vars['metcov11'                                   ] = {'std':'metcov11'                                   , 'sync':'mvacov11'                                  }

# VBF
vars['ditau_mjj'                                  ] = {'std':'ditau_mjj'                                  , 'sync':'mjj'                                       }
vars['ditau_deta'                                 ] = {'std':'ditau_deta'                                 , 'sync':'jdeta'                                     }
vars['ditau_nCentral'                             ] = {'std':'ditau_nCentral'                             , 'sync':'njetingap'                                 }
vars['ditau_mva'                                  ] = {'std':'ditau_mva'                                  , 'sync':'mva'                                       }

vars['ditau_jdphi'                                ] = {'std':'ditau_jdphi'                                , 'sync':'jdphi'                                     }
vars['ditau_dijetpt'                              ] = {'std':'ditau_dijetpt'                              , 'sync':'dijetpt'                                   }
vars['ditau_dijetphi'                             ] = {'std':'ditau_dijetphi'                             , 'sync':'dijetphi'                                  }
vars['ditau_hdijetphi'                            ] = {'std':'ditau_hdijetphi'                            , 'sync':'hdijetphi'                                 }
vars['ditau_visjeteta'                            ] = {'std':'ditau_visjeteta'                            , 'sync':'visjeteta'                                 }
vars['ditau_ptvis'                                ] = {'std':'ditau_ptvis'                                , 'sync':'ptvis'                                     }

# N Jets
vars['nJets'                                      ] = {'std':'nJets'                                      , 'sync':'nbtag'                                     }
vars['nJets20'                                    ] = {'std':'nJets20'                                    , 'sync':'njets'                                     }
vars['nbJets'                                     ] = {'std':'nbJets'                                     , 'sync':'njetspt20'                                 }

# Jet 1
vars['jet1_pt'                                    ] = {'std':'jet1_pt'                                    , 'sync':'jpt_1'                                     }
vars['jet1_eta'                                   ] = {'std':'jet1_eta'                                   , 'sync':'jeta_1'                                    }
vars['jet1_phi'                                   ] = {'std':'jet1_phi'                                   , 'sync':'jphi_1'                                    }
vars['jet1_rawFactor'                             ] = {'std':'jet1_rawFactor'                             , 'sync':'jrawf_1'                                   }
vars['jet1_puMva'                                 ] = {'std':'jet1_puMva'                                 , 'sync':'jmva_1'                                    }
vars['jet1_looseJetId'                            ] = {'std':'jet1_looseJetId'                            , 'sync':'jpfid_1'                                   }
vars['jet1_puJetId'                               ] = {'std':'jet1_puJetId'                               , 'sync':'jpuid_1'                                   }
vars['jet1_csv'                                   ] = {'std':'jet1_csv'                                   , 'sync':'jcsv_1'                                    }

# Jet 2
vars['jet2_pt'                                    ] = {'std':'jet2_pt'                                    , 'sync':'jpt_2'                                     }
vars['jet2_eta'                                   ] = {'std':'jet2_eta'                                   , 'sync':'jeta_2'                                    }
vars['jet2_phi'                                   ] = {'std':'jet2_phi'                                   , 'sync':'jphi_2'                                    }
vars['jet2_rawFactor'                             ] = {'std':'jet2_rawFactor'                             , 'sync':'jrawf_2'                                   }
vars['jet2_puMva'                                 ] = {'std':'jet2_puMva'                                 , 'sync':'jmva_2'                                    }
vars['jet2_looseJetId'                            ] = {'std':'jet2_looseJetId'                            , 'sync':'jpfid_2'                                   }
vars['jet2_puJetId'                               ] = {'std':'jet2_puJetId'                               , 'sync':'jpuid_2'                                   }
vars['jet2_csv'                                   ] = {'std':'jet2_csv'                                   , 'sync':'jcsv_2'                                    }

# bJet 1
vars['bjet1_pt'                                   ] = {'std':'bjet1_pt'                                   , 'sync':'bjpt_1'                                     }
vars['bjet1_eta'                                  ] = {'std':'bjet1_eta'                                  , 'sync':'bjeta_1'                                    }
vars['bjet1_phi'                                  ] = {'std':'bjet1_phi'                                  , 'sync':'bjphi_1'                                    }
vars['bjet1_rawFactor'                            ] = {'std':'bjet1_rawFactor'                            , 'sync':'bjrawf_1'                                   }
vars['bjet1_puMva'                                ] = {'std':'bjet1_puMva'                                , 'sync':'bjmva_1'                                    }
vars['bjet1_looseJetId'                           ] = {'std':'bjet1_looseJetId'                           , 'sync':'bjpfid_1'                                   }
vars['bjet1_puJetId'                              ] = {'std':'bjet1_puJetId'                              , 'sync':'bjpuid_1'                                   }
vars['bjet1_csv'                                  ] = {'std':'bjet1_csv'                                  , 'sync':'bjcsv_1'                                    }

# bJet 2
vars['bjet2_pt'                                   ] = {'std':'bjet2_pt'                                   , 'sync':'bjpt_2'                                     }
vars['bjet2_eta'                                  ] = {'std':'bjet2_eta'                                  , 'sync':'bjeta_2'                                    }
vars['bjet2_phi'                                  ] = {'std':'bjet2_phi'                                  , 'sync':'bjphi_2'                                    }
vars['bjet2_rawFactor'                            ] = {'std':'bjet2_rawFactor'                            , 'sync':'bjrawf_2'                                   }
vars['bjet2_puMva'                                ] = {'std':'bjet2_puMva'                                , 'sync':'bjmva_2'                                    }
vars['bjet2_looseJetId'                           ] = {'std':'bjet2_looseJetId'                           , 'sync':'bjpfid_2'                                   }
vars['bjet2_puJetId'                              ] = {'std':'bjet2_puJetId'                              , 'sync':'bjpuid_2'                                   }
vars['bjet2_csv'                                  ] = {'std':'bjet2_csv'                                  , 'sync':'bjcsv_2'                                    }



