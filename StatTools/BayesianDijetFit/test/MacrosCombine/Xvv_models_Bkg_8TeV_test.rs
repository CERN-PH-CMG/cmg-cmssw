mgg[890,2500];
mgg_sig_m0[2000.0, 800.0, 2500.0];
mgg_sig_sigma[100, 50.0, 200.0];
mgg_sig_alpha[0.8, 0.5, 3]; 
mgg_sig_n[13.0, 0.5, 10]; 
mgg_sig_gsigma[100, 50.0, 200.0];
mgg_sig_frac[0.5, 0, 1.0];

MggGaussSig = Gaussian(mgg, mgg_sig_m0, mgg_sig_gsigma);
MggCBSig    = CBShape(mgg, mgg_sig_m0, mgg_sig_sigma, mgg_sig_alpha, mgg_sig_n);
MggSig      = AddPdf(MggGaussSig, MggCBSig, mgg_sig_frac);

mgg_sig_m0_cat0[2000.0, 800.0, 2500.0];
mgg_sig_sigma_cat0[100, 50.0, 200.0];
mgg_sig_alpha_cat0[ 0.8, 0.0, 3.0]; 
mgg_sig_n_cat0[130, 0.00001, 1000.0]; 
mgg_sig_gsigma_cat0[100, 50.0, 200.0];
mgg_sig_frac_cat0[0.5, 0, 1.0];


MggGaussSig_cat0 = Gaussian(mgg, mgg_sig_m0_cat0, mgg_sig_gsigma_cat0);
MggCBSig_cat0    = CBShape(mgg, mgg_sig_m0_cat0, mgg_sig_sigma_cat0, mgg_sig_alpha_cat0, mgg_sig_n_cat0);
MggSig_cat0      = AddPdf(MggGaussSig_cat0, MggCBSig_cat0, mgg_sig_frac_cat0);

mgg_sig_m0_cat1[2000.0, 800.0, 2500.0];
mgg_sig_sigma_cat1[100, 50.0, 200.0];
mgg_sig_alpha_cat1[ 0.8, 0.0, 3.0]; 
mgg_sig_n_cat1[130, 0.00001, 1000.0]; 
mgg_sig_gsigma_cat1[100, 50.0, 200.0];
mgg_sig_frac_cat1[0.5, 0, 1.0];

MggGaussSig_cat1 = Gaussian(mgg, mgg_sig_m0_cat1, mgg_sig_gsigma_cat1);
MggCBSig_cat1    = CBShape(mgg, mgg_sig_m0_cat1, mgg_sig_sigma_cat1, mgg_sig_alpha_cat1, mgg_sig_n_cat1);
MggSig_cat1      = AddPdf(MggGaussSig_cat1, MggCBSig_cat1, mgg_sig_frac_cat1);

mgg_sig_m0_cat2[2000.0, 800.0, 2500.0];
mgg_sig_sigma_cat2[100, 50.0, 1000.0];
mgg_sig_alpha_cat2[ 0.8, 0.0, 3.0]; 
mgg_sig_n_cat2[130, 0.00001, 1000.0]; 
mgg_sig_gsigma_cat2[100, 50.0, 1000.0];
mgg_sig_frac_cat2[0.5, 0, 1.0];

MggGaussSig_cat2 = Gaussian(mgg, mgg_sig_m0_cat2, mgg_sig_gsigma_cat2);
MggCBSig_cat2    = CBShape(mgg, mgg_sig_m0_cat2, mgg_sig_sigma_cat2, mgg_sig_alpha_cat2, mgg_sig_n_cat2);
MggSig_cat2      = AddPdf(MggGaussSig_cat2, MggCBSig_cat2, mgg_sig_frac_cat2);

mgg_sig_m0_cat3[2000.0, 800.0, 2500.0];
mgg_sig_sigma_cat3[100, 50.0, 1000.0];
mgg_sig_alpha_cat3[ 0.8, 0.0, 3.0]; 
mgg_sig_n_cat3[130, 0.00001, 1000.0]; 
mgg_sig_gsigma_cat3[100, 50.0, 1000.0];
mgg_sig_frac_cat3[0.5, 0, 1.0];

MggGaussSig_cat3 = Gaussian(mgg, mgg_sig_m0_cat3, mgg_sig_gsigma_cat3);
MggCBSig_cat3    = CBShape(mgg, mgg_sig_m0_cat3, mgg_sig_sigma_cat3, mgg_sig_alpha_cat3, mgg_sig_n_cat3);
MggSig_cat3      = AddPdf(MggGaussSig_cat3, MggCBSig_cat3, mgg_sig_frac_cat3);

mgg_sig_m0_cat4[2000.0, 800.0, 2500.0];
mgg_sig_sigma_cat4[100, 50.0, 1000.0];
mgg_sig_alpha_cat4[ 0.8, 0.0, 3.0]; 
mgg_sig_n_cat4[130, 0.00001, 1000.0]; 
mgg_sig_gsigma_cat4[100, 50.0, 1000.0];
mgg_sig_frac_cat4[0.5, 0, 1.0];

MggGaussSig_cat4 = Gaussian(mgg, mgg_sig_m0_cat4, mgg_sig_gsigma_cat4);
MggCBSig_cat4    = CBShape(mgg, mgg_sig_m0_cat4, mgg_sig_sigma_cat4, mgg_sig_alpha_cat4, mgg_sig_n_cat4);
MggSig_cat4      = AddPdf(MggGaussSig_cat4, MggCBSig_cat4, mgg_sig_frac_cat4);


mgg_sig_m0_cat5[2000.0, 800.0, 2500.0];
mgg_sig_sigma_cat5[100, 50.0, 1000.0];
mgg_sig_alpha_cat5[ 0.8, 0.0, 3.0]; 
mgg_sig_n_cat5[130, 0.00001, 1000.0]; 
mgg_sig_gsigma_cat5[100, 50.0, 1000.0];
mgg_sig_frac_cat5[0.5, 0, 1.0];

MggGaussSig_cat5 = Gaussian(mgg, mgg_sig_m0_cat5, mgg_sig_gsigma_cat5);
MggCBSig_cat5    = CBShape(mgg, mgg_sig_m0_cat5, mgg_sig_sigma_cat5, mgg_sig_alpha_cat5, mgg_sig_n_cat5);
MggSig_cat5      = AddPdf(MggGaussSig_cat5, MggCBSig_cat5, mgg_sig_frac_cat5);




mgg_bkg_8TeV_slope[1.0,0, 1];
mgg_bkg_8TeV_slope1[7,0.0, 100.0];
mgg_bkg_8TeV_slope2[5,0.0, 100.0];
mgg_bkg_8TeV_slope3[0.,0.0, 0.0];


mgg_bkg_8TeV_slope_cat0[1000.0,0, 10000000];
mgg_bkg_8TeV_slope1_cat0[10., -100.0, 500.0];
mgg_bkg_8TeV_slope2_cat0[5.,0.0, 100.0];
mgg_bkg_8TeV_slope3_cat0[0.,-10.0, 10.0];

mgg_bkg_8TeV_slope_cat1[1000.0,0, 10000000];
mgg_bkg_8TeV_slope1_cat1[10., -100.0, 500.0];
mgg_bkg_8TeV_slope2_cat1[5.,0.0, 100.0];
mgg_bkg_8TeV_slope3_cat1[0.,-10.0, 10.0];

mgg_bkg_8TeV_slope_cat2[1000.0,0, 10000000];
mgg_bkg_8TeV_slope1_cat2[10., -100.0, 500.0];
mgg_bkg_8TeV_slope2_cat2[5.,0.0, 100.0];
mgg_bkg_8TeV_slope3_cat2[0.,-10.0, 10.0];

mgg_bkg_8TeV_slope_cat3[1000.0,0, 10000000];
mgg_bkg_8TeV_slope1_cat3[10., -100.0, 500.0];
mgg_bkg_8TeV_slope2_cat3[5.,0.0, 100.0];
mgg_bkg_8TeV_slope3_cat3[0.,-10.0, 10.0];

mgg_bkg_8TeV_slope_cat4[1000.0,0, 10000000];
mgg_bkg_8TeV_slope1_cat4[10., -100.0, 500.0];
mgg_bkg_8TeV_slope2_cat4[5.,0.0, 100.0];
mgg_bkg_8TeV_slope3_cat4[0.,-10.0, 10.0];

mgg_bkg_8TeV_slope_cat5[1000.0,0, 10000000];
mgg_bkg_8TeV_slope1_cat5[10., -100.0, 500.0];
mgg_bkg_8TeV_slope2_cat5[5.,0.0, 100.0];
mgg_bkg_8TeV_slope3_cat5[0.,-10.0, 10.0];

wei[1,0,10];

sqrtS[8000., 8000., 8000.]