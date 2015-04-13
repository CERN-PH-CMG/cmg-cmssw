1) edit ../run_susyMT2.cfg to run over your favorite components

2) edit launchall.py to change the CMG-version/tag and the production name

3) run!!!
> voms-proxy-init -voms cms --valid=50:00
Enter GRID pass phrase for this identity: pandolfiniGay
> python launchall.py


Notes: 
- debugging: choose a single component and set debug=True in heppy_crab_config_env.py

- modify heppy_crab_config.py to run only on your favorite sites
