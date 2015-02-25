# change the branch names here

vars = {}

vars['run' ] = {'std':'run'      , 'sync':'run'       }
vars['lumi'] = {'std':'lumi'     , 'sync':'lumi'      }
vars['event'] = {'std':'event'    , 'sync':'evt'       }

vars['l1_pt'           ] = {'std':'l1_pt'           , 'sync':'pt_1'        }
vars['l1_phi'          ] = {'std':'l1_phi'          , 'sync':'phi_1'       }
vars['l1_eta'          ] = {'std':'l1_eta'          , 'sync':'eta_1'       }
vars['l1_mass'         ] = {'std':'l1_mass'         , 'sync':'m_1'         }
vars['l1_charge'       ] = {'std':'l1_charge'       , 'sync':'q_1'         }
vars['mtleg1'          ] = {'std':'mtleg1'          , 'sync':'mt_1'        }
vars['l1_triggerWeight'] = {'std':'l1_triggerWeight', 'sync':'trigweight_1'}
        
vars['l2_pt'           ] = {'std':'l2_pt'           , 'sync':'pt_2'        }
vars['l2_phi'          ] = {'std':'l2_phi'          , 'sync':'phi_2'       }
vars['l2_eta'          ] = {'std':'l2_eta'          , 'sync':'eta_2'       }
vars['l2_mass'         ] = {'std':'l2_mass'         , 'sync':'m_2'         }
vars['l2_charge'       ] = {'std':'l2_charge'       , 'sync':'q_2'         }
# vars['l2_pt'           ] = {'std':'l2_pt'           , 'sync':'iso_2'       }
# vars['l2_pt'           ] = {'std':'l2_pt'           , 'sync':'mva_2'       }
# vars['l2_pt'           ] = {'std':'l2_pt'           , 'sync':'passid_2'    }
# vars['l2_pt'           ] = {'std':'l2_pt'           , 'sync':'passiso_2'   }
# vars['l2_pt'           ] = {'std':'l2_pt'           , 'sync':'mt_2'        }
vars['l2_triggerWeight'] = {'std':'l2_triggerWeight', 'sync':'trigweight_2'}
# vars['l2_pt'           ] = {'std':'l2_pt'           , 'sync':'idweight_2'  }
# vars['l2_pt'           ] = {'std':'l2_pt'           , 'sync':'isoweight_2' }

vars['pfmet_pt' ] = {'std':'pfmet_pt' , 'sync':'met'         }
vars['pfmet_phi'] = {'std':'pfmet_phi', 'sync':'metphi'      }
vars['met'      ] = {'std':'met'      , 'sync':'mvamet'      }
vars['metphi'   ] = {'std':'metphi'   , 'sync':'mvametphi'   }
vars['pZetaMET' ] = {'std':'pZetaMET' , 'sync':'pzetavis'    }
vars['pZetaVis' ] = {'std':'pZetaVis' , 'sync':'pzetamiss'   }

vars['metcov00'] = {'std':'metcov00'  , 'sync':'mvacov00'    }
vars['metcov01'] = {'std':'metcov01'  , 'sync':'mvacov01'    }
vars['metcov10'] = {'std':'metcov10'  , 'sync':'mvacov10'    }
vars['metcov11'] = {'std':'metcov11'  , 'sync':'mvacov11'    }

vars['{pName}_mjj'      ] = {'std':'{pName}_mjj'      , 'sync':'mjj'       }
vars['{pName}_deta'     ] = {'std':'{pName}_deta'     , 'sync':'jdeta'     }
vars['{pName}_nCentral' ] = {'std':'{pName}_nCentral' , 'sync':'njetingap' }
vars['{pName}_mva'      ] = {'std':'{pName}_mva'      , 'sync':'mva'       }

vars['{pName}_jdphi'    ] = {'std':'{pName}_jdphi'    , 'sync':'jdphi'     }
vars['{pName}_dijetpt'  ] = {'std':'{pName}_dijetpt'  , 'sync':'dijetpt'   }
vars['{pName}_dijetphi' ] = {'std':'{pName}_dijetphi' , 'sync':'dijetphi'  }
vars['{pName}_hdijetphi'] = {'std':'{pName}_hdijetphi', 'sync':'hdijetphi' }
vars['{pName}_visjeteta'] = {'std':'{pName}_visjeteta', 'sync':'visjeteta' }
vars['{pName}_ptvis'    ] = {'std':'{pName}_ptvis'    , 'sync':'ptvis'     }

vars['nJets'            ] = {'std':'nJets'            , 'sync':'nbtag'     }
vars['nJets20'          ] = {'std':'nJets20'          , 'sync':'njets'     }
vars['nbJets'           ] = {'std':'nbJets'           , 'sync':'njetspt20' }
        
        

