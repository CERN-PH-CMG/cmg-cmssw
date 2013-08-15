mgg[890,2500];

mgg_ZZ_sig_m0[2000.0, 900.0, 3100.0];
mgg_ZZ_sig_sigma[100, 50.0, 1000.0];
mgg_ZZ_sig_alpha[0.8, 0.5, 3]; 
mgg_ZZ_sig_n[13.0, 0.5, 10]; 
mgg_ZZ_sig_gsigma[100, 50.0, 1000.0];
mgg_ZZ_sig_frac[0.5, 0.3, 1.0];

MggGaussSigZZ = Gaussian(mgg, mgg_ZZ_sig_m0, mgg_ZZ_sig_gsigma);
MggCBSigZZ    = CBShape(mgg, mgg_ZZ_sig_m0, mgg_ZZ_sig_sigma, mgg_ZZ_sig_alpha, mgg_ZZ_sig_n);
MggSigZZ      = AddPdf(MggGaussSigZZ, MggCBSigZZ, mgg_ZZ_sig_frac);

mgg_ZZ_sig_m0_cat0[2000.0, 900.0, 3100.0];
mgg_ZZ_sig_sigma_cat0[100, 50.0, 1000.0];
mgg_ZZ_sig_alpha_cat0[ 0.8, 0.0, 3.0]; 
mgg_ZZ_sig_n_cat0[130, 0.00001, 1000.0]; 
mgg_ZZ_sig_gsigma_cat0[100, 50.0, 1000.0];
mgg_ZZ_sig_frac_cat0[0.5, 0.3, 1.0];


MggGaussSigZZ_cat0 = Gaussian(mgg, mgg_ZZ_sig_m0_cat0, mgg_ZZ_sig_gsigma_cat0);
MggCBSigZZ_cat0    = CBShape(mgg, mgg_ZZ_sig_m0_cat0, mgg_ZZ_sig_sigma_cat0, mgg_ZZ_sig_alpha_cat0, mgg_ZZ_sig_n_cat0);
MggSigZZ_cat0      = AddPdf(MggGaussSigZZ_cat0, MggCBSigZZ_cat0, mgg_ZZ_sig_frac_cat0);

mgg_ZZ_sig_m0_cat1[2000.0, 900.0, 3100.0];
mgg_ZZ_sig_sigma_cat1[100, 50.0, 1000.0];
mgg_ZZ_sig_alpha_cat1[ 0.8, 0.0, 3.0]; 
mgg_ZZ_sig_n_cat1[130, 0.00001, 1000.0]; 
mgg_ZZ_sig_gsigma_cat1[100, 50.0, 1000.0];
mgg_ZZ_sig_frac_cat1[0.5, 0.3, 1.0];

MggGaussSigZZ_cat1 = Gaussian(mgg, mgg_ZZ_sig_m0_cat1, mgg_ZZ_sig_gsigma_cat1);
MggCBSigZZ_cat1    = CBShape(mgg, mgg_ZZ_sig_m0_cat1, mgg_ZZ_sig_sigma_cat1, mgg_ZZ_sig_alpha_cat1, mgg_ZZ_sig_n_cat1);
MggSigZZ_cat1      = AddPdf(MggGaussSigZZ_cat1, MggCBSigZZ_cat1, mgg_ZZ_sig_frac_cat1);

mgg_ZZ_sig_m0_cat2[2000.0, 900.0, 3100.0];
mgg_ZZ_sig_sigma_cat2[100, 50.0, 1000.0];
mgg_ZZ_sig_alpha_cat2[ 0.8, 0.0, 3.0]; 
mgg_ZZ_sig_n_cat2[130, 0.00001, 1000.0]; 
mgg_ZZ_sig_gsigma_cat2[100, 50.0, 1000.0];
mgg_ZZ_sig_frac_cat2[0.5, 0.3, 1.0];

MggGaussSigZZ_cat2 = Gaussian(mgg, mgg_ZZ_sig_m0_cat2, mgg_ZZ_sig_gsigma_cat2);
MggCBSigZZ_cat2    = CBShape(mgg, mgg_ZZ_sig_m0_cat2, mgg_ZZ_sig_sigma_cat2, mgg_ZZ_sig_alpha_cat2, mgg_ZZ_sig_n_cat2);
MggSigZZ_cat2      = AddPdf(MggGaussSigZZ_cat2, MggCBSigZZ_cat2, mgg_ZZ_sig_frac_cat2);

mgg_ZZ_sig_m0_cat3[2000.0, 900.0, 3100.0];
mgg_ZZ_sig_sigma_cat3[100, 50.0, 1000.0];
mgg_ZZ_sig_alpha_cat3[ 0.8, 0.0, 3.0]; 
mgg_ZZ_sig_n_cat3[130, 0.00001, 1000.0]; 
mgg_ZZ_sig_gsigma_cat3[100, 50.0, 1000.0];
mgg_ZZ_sig_frac_cat3[0.5, 0.3, 1.0];

MggGaussSigZZ_cat3 = Gaussian(mgg, mgg_ZZ_sig_m0_cat3, mgg_ZZ_sig_gsigma_cat3);
MggCBSigZZ_cat3    = CBShape(mgg, mgg_ZZ_sig_m0_cat3, mgg_ZZ_sig_sigma_cat3, mgg_ZZ_sig_alpha_cat3, mgg_ZZ_sig_n_cat3);
MggSigZZ_cat3      = AddPdf(MggGaussSigZZ_cat3, MggCBSigZZ_cat3, mgg_ZZ_sig_frac_cat3);

mgg_ZZ_sig_m0_cat4[2000.0, 900.0, 3100.0];
mgg_ZZ_sig_sigma_cat4[100, 50.0, 1000.0];
mgg_ZZ_sig_alpha_cat4[ 0.8, 0.0, 3.0]; 
mgg_ZZ_sig_n_cat4[130, 0.00001, 1000.0]; 
mgg_ZZ_sig_gsigma_cat4[100, 50.0, 1000.0];
mgg_ZZ_sig_frac_cat4[0.5, 0.3, 1.0];

MggGaussSigZZ_cat4 = Gaussian(mgg, mgg_ZZ_sig_m0_cat4, mgg_ZZ_sig_gsigma_cat4);
MggCBSigZZ_cat4    = CBShape(mgg, mgg_ZZ_sig_m0_cat4, mgg_ZZ_sig_sigma_cat4, mgg_ZZ_sig_alpha_cat4, mgg_ZZ_sig_n_cat4);
MggSigZZ_cat4      = AddPdf(MggGaussSigZZ_cat4, MggCBSigZZ_cat4, mgg_ZZ_sig_frac_cat4);


mgg_ZZ_sig_m0_cat5[2000.0, 900.0, 3100.0];
mgg_ZZ_sig_sigma_cat5[100, 50.0, 1000.0];
mgg_ZZ_sig_alpha_cat5[ 0.8, 0.0, 3.0]; 
mgg_ZZ_sig_n_cat5[130, 0.00001, 1000.0]; 
mgg_ZZ_sig_gsigma_cat5[100, 50.0, 1000.0];
mgg_ZZ_sig_frac_cat5[0.5, 0.3, 1.0];

MggGaussSigZZ_cat5 = Gaussian(mgg, mgg_ZZ_sig_m0_cat5, mgg_ZZ_sig_gsigma_cat5);
MggCBSigZZ_cat5    = CBShape(mgg, mgg_ZZ_sig_m0_cat5, mgg_ZZ_sig_sigma_cat5, mgg_ZZ_sig_alpha_cat5, mgg_ZZ_sig_n_cat5);
MggSigZZ_cat5      = AddPdf(MggGaussSigZZ_cat5, MggCBSigZZ_cat5, mgg_ZZ_sig_frac_cat5);






mgg_WW_sig_m0[2000.0, 900.0, 3100.0];
mgg_WW_sig_sigma[100, 50.0, 1000.0];
mgg_WW_sig_alpha[0.8, 0.5, 3]; 
mgg_WW_sig_n[13.0, 0.5, 10]; 
mgg_WW_sig_gsigma[100, 50.0, 1000.0];
mgg_WW_sig_frac[0.5, 0.3, 1.0];

MggGaussSigWW = Gaussian(mgg, mgg_WW_sig_m0, mgg_WW_sig_gsigma);
MggCBSigWW    = CBShape(mgg, mgg_WW_sig_m0, mgg_WW_sig_sigma, mgg_WW_sig_alpha, mgg_WW_sig_n);
MggSigWW      = AddPdf(MggGaussSigWW, MggCBSigWW, mgg_WW_sig_frac);

mgg_WW_sig_m0_cat0[2000.0, 900.0, 3100.0];
mgg_WW_sig_sigma_cat0[100, 50.0, 1000.0];
mgg_WW_sig_alpha_cat0[ 0.8, 0.0, 3.0]; 
mgg_WW_sig_n_cat0[130, 0.00001, 1000.0]; 
mgg_WW_sig_gsigma_cat0[100, 50.0, 1000.0];
mgg_WW_sig_frac_cat0[0.5, 0.3, 1.0];


MggGaussSigWW_cat0 = Gaussian(mgg, mgg_WW_sig_m0_cat0, mgg_WW_sig_gsigma_cat0);
MggCBSigWW_cat0    = CBShape(mgg, mgg_WW_sig_m0_cat0, mgg_WW_sig_sigma_cat0, mgg_WW_sig_alpha_cat0, mgg_WW_sig_n_cat0);
MggSigWW_cat0      = AddPdf(MggGaussSigWW_cat0, MggCBSigWW_cat0, mgg_WW_sig_frac_cat0);

mgg_WW_sig_m0_cat1[2000.0, 900.0, 3100.0];
mgg_WW_sig_sigma_cat1[100, 50.0, 1000.0];
mgg_WW_sig_alpha_cat1[ 0.8, 0.0, 3.0]; 
mgg_WW_sig_n_cat1[130, 0.00001, 1000.0]; 
mgg_WW_sig_gsigma_cat1[100, 50.0, 1000.0];
mgg_WW_sig_frac_cat1[0.5, 0.3, 1.0];

MggGaussSigWW_cat1 = Gaussian(mgg, mgg_WW_sig_m0_cat1, mgg_WW_sig_gsigma_cat1);
MggCBSigWW_cat1    = CBShape(mgg, mgg_WW_sig_m0_cat1, mgg_WW_sig_sigma_cat1, mgg_WW_sig_alpha_cat1, mgg_WW_sig_n_cat1);
MggSigWW_cat1      = AddPdf(MggGaussSigWW_cat1, MggCBSigWW_cat1, mgg_WW_sig_frac_cat1);

mgg_WW_sig_m0_cat2[2000.0, 900.0, 3100.0];
mgg_WW_sig_sigma_cat2[100, 50.0, 1000.0];
mgg_WW_sig_alpha_cat2[ 0.8, 0.0, 3.0]; 
mgg_WW_sig_n_cat2[130, 0.00001, 1000.0]; 
mgg_WW_sig_gsigma_cat2[100, 50.0, 1000.0];
mgg_WW_sig_frac_cat2[0.5, 0.3, 1.0];

MggGaussSigWW_cat2 = Gaussian(mgg, mgg_WW_sig_m0_cat2, mgg_WW_sig_gsigma_cat2);
MggCBSigWW_cat2    = CBShape(mgg, mgg_WW_sig_m0_cat2, mgg_WW_sig_sigma_cat2, mgg_WW_sig_alpha_cat2, mgg_WW_sig_n_cat2);
MggSigWW_cat2      = AddPdf(MggGaussSigWW_cat2, MggCBSigWW_cat2, mgg_WW_sig_frac_cat2);

mgg_WW_sig_m0_cat3[2000.0, 900.0, 3100.0];
mgg_WW_sig_sigma_cat3[100, 50.0, 1000.0];
mgg_WW_sig_alpha_cat3[ 0.8, 0.0, 3.0]; 
mgg_WW_sig_n_cat3[130, 0.00001, 1000.0]; 
mgg_WW_sig_gsigma_cat3[100, 50.0, 1000.0];
mgg_WW_sig_frac_cat3[0.5, 0.3, 1.0];

MggGaussSigWW_cat3 = Gaussian(mgg, mgg_WW_sig_m0_cat3, mgg_WW_sig_gsigma_cat3);
MggCBSigWW_cat3    = CBShape(mgg, mgg_WW_sig_m0_cat3, mgg_WW_sig_sigma_cat3, mgg_WW_sig_alpha_cat3, mgg_WW_sig_n_cat3);
MggSigWW_cat3      = AddPdf(MggGaussSigWW_cat3, MggCBSigWW_cat3, mgg_WW_sig_frac_cat3);

mgg_WW_sig_m0_cat4[2000.0, 900.0, 3100.0];
mgg_WW_sig_sigma_cat4[100, 50.0, 1000.0];
mgg_WW_sig_alpha_cat4[ 0.8, 0.0, 3.0]; 
mgg_WW_sig_n_cat4[130, 0.00001, 1000.0]; 
mgg_WW_sig_gsigma_cat4[100, 50.0, 1000.0];
mgg_WW_sig_frac_cat4[0.5, 0.3, 1.0];

MggGaussSigWW_cat4 = Gaussian(mgg, mgg_WW_sig_m0_cat4, mgg_WW_sig_gsigma_cat4);
MggCBSigWW_cat4    = CBShape(mgg, mgg_WW_sig_m0_cat4, mgg_WW_sig_sigma_cat4, mgg_WW_sig_alpha_cat4, mgg_WW_sig_n_cat4);
MggSigWW_cat4      = AddPdf(MggGaussSigWW_cat4, MggCBSigWW_cat4, mgg_WW_sig_frac_cat4);


mgg_WW_sig_m0_cat5[2000.0, 900.0, 3100.0];
mgg_WW_sig_sigma_cat5[100, 50.0, 1000.0];
mgg_WW_sig_alpha_cat5[ 0.8, 0.0, 3.0]; 
mgg_WW_sig_n_cat5[130, 0.00001, 1000.0]; 
mgg_WW_sig_gsigma_cat5[100, 50.0, 1000.0];
mgg_WW_sig_frac_cat5[0.5, 0.3, 1.0];

MggGaussSigWW_cat5 = Gaussian(mgg, mgg_WW_sig_m0_cat5, mgg_WW_sig_gsigma_cat5);
MggCBSigWW_cat5    = CBShape(mgg, mgg_WW_sig_m0_cat5, mgg_WW_sig_sigma_cat5, mgg_WW_sig_alpha_cat5, mgg_WW_sig_n_cat5);
MggSigWW_cat5      = AddPdf(MggGaussSigWW_cat5, MggCBSigWW_cat5, mgg_WW_sig_frac_cat5);






mgg_WZ_sig_m0[2000.0, 900.0, 3100.0];
mgg_WZ_sig_sigma[100, 50.0, 1000.0];
mgg_WZ_sig_alpha[0.8, 0.5, 3]; 
mgg_WZ_sig_n[13.0, 0.5, 10]; 
mgg_WZ_sig_gsigma[100, 50.0, 1000.0];
mgg_WZ_sig_frac[0.5, 0.3, 1.0];

MggGaussSigWZ = Gaussian(mgg, mgg_WZ_sig_m0, mgg_WZ_sig_gsigma);
MggCBSigWZ    = CBShape(mgg, mgg_WZ_sig_m0, mgg_WZ_sig_sigma, mgg_WZ_sig_alpha, mgg_WZ_sig_n);
MggSigWZ      = AddPdf(MggGaussSigWZ, MggCBSigWZ, mgg_WZ_sig_frac);

mgg_WZ_sig_m0_cat0[2000.0, 900.0, 3100.0];
mgg_WZ_sig_sigma_cat0[100, 50.0, 1000.0];
mgg_WZ_sig_alpha_cat0[ 0.8, 0.0, 3.0]; 
mgg_WZ_sig_n_cat0[130, 0.00001, 1000.0]; 
mgg_WZ_sig_gsigma_cat0[100, 50.0, 1000.0];
mgg_WZ_sig_frac_cat0[0.5, 0.3, 1.0];


MggGaussSigWZ_cat0 = Gaussian(mgg, mgg_WZ_sig_m0_cat0, mgg_WZ_sig_gsigma_cat0);
MggCBSigWZ_cat0    = CBShape(mgg, mgg_WZ_sig_m0_cat0, mgg_WZ_sig_sigma_cat0, mgg_WZ_sig_alpha_cat0, mgg_WZ_sig_n_cat0);
MggSigWZ_cat0      = AddPdf(MggGaussSigWZ_cat0, MggCBSigWZ_cat0, mgg_WZ_sig_frac_cat0);

mgg_WZ_sig_m0_cat1[2000.0, 900.0, 3100.0];
mgg_WZ_sig_sigma_cat1[100, 50.0, 1000.0];
mgg_WZ_sig_alpha_cat1[ 0.8, 0.0, 3.0]; 
mgg_WZ_sig_n_cat1[130, 0.00001, 1000.0]; 
mgg_WZ_sig_gsigma_cat1[100, 50.0, 1000.0];
mgg_WZ_sig_frac_cat1[0.5, 0.3, 1.0];

MggGaussSigWZ_cat1 = Gaussian(mgg, mgg_WZ_sig_m0_cat1, mgg_WZ_sig_gsigma_cat1);
MggCBSigWZ_cat1    = CBShape(mgg, mgg_WZ_sig_m0_cat1, mgg_WZ_sig_sigma_cat1, mgg_WZ_sig_alpha_cat1, mgg_WZ_sig_n_cat1);
MggSigWZ_cat1      = AddPdf(MggGaussSigWZ_cat1, MggCBSigWZ_cat1, mgg_WZ_sig_frac_cat1);

mgg_WZ_sig_m0_cat2[2000.0, 900.0, 3100.0];
mgg_WZ_sig_sigma_cat2[100, 50.0, 1000.0];
mgg_WZ_sig_alpha_cat2[ 0.8, 0.0, 3.0]; 
mgg_WZ_sig_n_cat2[130, 0.00001, 1000.0]; 
mgg_WZ_sig_gsigma_cat2[100, 50.0, 1000.0];
mgg_WZ_sig_frac_cat2[0.5, 0.3, 1.0];

MggGaussSigWZ_cat2 = Gaussian(mgg, mgg_WZ_sig_m0_cat2, mgg_WZ_sig_gsigma_cat2);
MggCBSigWZ_cat2    = CBShape(mgg, mgg_WZ_sig_m0_cat2, mgg_WZ_sig_sigma_cat2, mgg_WZ_sig_alpha_cat2, mgg_WZ_sig_n_cat2);
MggSigWZ_cat2      = AddPdf(MggGaussSigWZ_cat2, MggCBSigWZ_cat2, mgg_WZ_sig_frac_cat2);

mgg_WZ_sig_m0_cat3[2000.0, 900.0, 3100.0];
mgg_WZ_sig_sigma_cat3[100, 50.0, 1000.0];
mgg_WZ_sig_alpha_cat3[ 0.8, 0.0, 3.0]; 
mgg_WZ_sig_n_cat3[130, 0.00001, 1000.0]; 
mgg_WZ_sig_gsigma_cat3[100, 50.0, 1000.0];
mgg_WZ_sig_frac_cat3[0.5, 0.3, 1.0];

MggGaussSigWZ_cat3 = Gaussian(mgg, mgg_WZ_sig_m0_cat3, mgg_WZ_sig_gsigma_cat3);
MggCBSigWZ_cat3    = CBShape(mgg, mgg_WZ_sig_m0_cat3, mgg_WZ_sig_sigma_cat3, mgg_WZ_sig_alpha_cat3, mgg_WZ_sig_n_cat3);
MggSigWZ_cat3      = AddPdf(MggGaussSigWZ_cat3, MggCBSigWZ_cat3, mgg_WZ_sig_frac_cat3);

mgg_WZ_sig_m0_cat4[2000.0, 900.0, 3100.0];
mgg_WZ_sig_sigma_cat4[100, 50.0, 1000.0];
mgg_WZ_sig_alpha_cat4[ 0.8, 0.0, 3.0]; 
mgg_WZ_sig_n_cat4[130, 0.00001, 1000.0]; 
mgg_WZ_sig_gsigma_cat4[100, 50.0, 1000.0];
mgg_WZ_sig_frac_cat4[0.5, 0.3, 1.0];

MggGaussSigWZ_cat4 = Gaussian(mgg, mgg_WZ_sig_m0_cat4, mgg_WZ_sig_gsigma_cat4);
MggCBSigWZ_cat4    = CBShape(mgg, mgg_WZ_sig_m0_cat4, mgg_WZ_sig_sigma_cat4, mgg_WZ_sig_alpha_cat4, mgg_WZ_sig_n_cat4);
MggSigWZ_cat4      = AddPdf(MggGaussSigWZ_cat4, MggCBSigWZ_cat4, mgg_WZ_sig_frac_cat4);


mgg_WZ_sig_m0_cat5[2000.0, 900.0, 3100.0];
mgg_WZ_sig_sigma_cat5[100, 50.0, 1000.0];
mgg_WZ_sig_alpha_cat5[ 0.8, 0.0, 3.0]; 
mgg_WZ_sig_n_cat5[130, 0.00001, 1000.0]; 
mgg_WZ_sig_gsigma_cat5[100, 50.0, 1000.0];
mgg_WZ_sig_frac_cat5[0.5, 0.3, 1.0];

MggGaussSigWZ_cat5 = Gaussian(mgg, mgg_WZ_sig_m0_cat5, mgg_WZ_sig_gsigma_cat5);
MggCBSigWZ_cat5    = CBShape(mgg, mgg_WZ_sig_m0_cat5, mgg_WZ_sig_sigma_cat5, mgg_WZ_sig_alpha_cat5, mgg_WZ_sig_n_cat5);
MggSigWZ_cat5      = AddPdf(MggGaussSigWZ_cat5, MggCBSigWZ_cat5, mgg_WZ_sig_frac_cat5);






mgg_qW_sig_m0[2000.0, 900.0, 4100.0];
mgg_qW_sig_sigma[100, 50.0, 1000.0];
mgg_qW_sig_alpha[0.8, 0.5, 3]; 
mgg_qW_sig_n[13.0, 0.5, 10]; 
mgg_qW_sig_gsigma[100, 50.0, 1000.0];
mgg_qW_sig_frac[0.5, 0.3, 1.0];

MggGaussSigqW = Gaussian(mgg, mgg_qW_sig_m0, mgg_qW_sig_gsigma);
MggCBSigqW    = CBShape(mgg, mgg_qW_sig_m0, mgg_qW_sig_sigma, mgg_qW_sig_alpha, mgg_qW_sig_n);
MggSigqW      = AddPdf(MggGaussSigqW, MggCBSigqW, mgg_qW_sig_frac);

mgg_qW_sig_m0_cat0[2000.0, 900.0, 4100.0];
mgg_qW_sig_sigma_cat0[100, 50.0, 1000.0];
mgg_qW_sig_alpha_cat0[ 0.8, 0.0, 3.0]; 
mgg_qW_sig_n_cat0[130, 0.00001, 1000.0]; 
mgg_qW_sig_gsigma_cat0[100, 50.0, 1000.0];
mgg_qW_sig_frac_cat0[0.5, 0.3, 1.0];


MggGaussSigqW_cat0 = Gaussian(mgg, mgg_qW_sig_m0_cat0, mgg_qW_sig_gsigma_cat0);
MggCBSigqW_cat0    = CBShape(mgg, mgg_qW_sig_m0_cat0, mgg_qW_sig_sigma_cat0, mgg_qW_sig_alpha_cat0, mgg_qW_sig_n_cat0);
MggSigqW_cat0      = AddPdf(MggGaussSigqW_cat0, MggCBSigqW_cat0, mgg_qW_sig_frac_cat0);

mgg_qW_sig_m0_cat1[2000.0, 900.0, 4100.0];
mgg_qW_sig_sigma_cat1[100, 50.0, 1000.0];
mgg_qW_sig_alpha_cat1[ 0.8, 0.0, 3.0]; 
mgg_qW_sig_n_cat1[130, 0.00001, 1000.0]; 
mgg_qW_sig_gsigma_cat1[100, 50.0, 1000.0];
mgg_qW_sig_frac_cat1[0.5, 0.3, 1.0];

MggGaussSigqW_cat1 = Gaussian(mgg, mgg_qW_sig_m0_cat1, mgg_qW_sig_gsigma_cat1);
MggCBSigqW_cat1    = CBShape(mgg, mgg_qW_sig_m0_cat1, mgg_qW_sig_sigma_cat1, mgg_qW_sig_alpha_cat1, mgg_qW_sig_n_cat1);
MggSigqW_cat1      = AddPdf(MggGaussSigqW_cat1, MggCBSigqW_cat1, mgg_qW_sig_frac_cat1);

mgg_qW_sig_m0_cat2[2000.0, 900.0, 4100.0];
mgg_qW_sig_sigma_cat2[100, 50.0, 1000.0];
mgg_qW_sig_alpha_cat2[ 0.8, 0.0, 3.0]; 
mgg_qW_sig_n_cat2[130, 0.00001, 1000.0]; 
mgg_qW_sig_gsigma_cat2[100, 50.0, 1000.0];
mgg_qW_sig_frac_cat2[0.5, 0.3, 1.0];

MggGaussSigqW_cat2 = Gaussian(mgg, mgg_qW_sig_m0_cat2, mgg_qW_sig_gsigma_cat2);
MggCBSigqW_cat2    = CBShape(mgg, mgg_qW_sig_m0_cat2, mgg_qW_sig_sigma_cat2, mgg_qW_sig_alpha_cat2, mgg_qW_sig_n_cat2);
MggSigqW_cat2      = AddPdf(MggGaussSigqW_cat2, MggCBSigqW_cat2, mgg_qW_sig_frac_cat2);

mgg_qW_sig_m0_cat3[2000.0, 900.0, 4100.0];
mgg_qW_sig_sigma_cat3[100, 50.0, 1000.0];
mgg_qW_sig_alpha_cat3[ 0.8, 0.0, 3.0]; 
mgg_qW_sig_n_cat3[130, 0.00001, 1000.0]; 
mgg_qW_sig_gsigma_cat3[100, 50.0, 1000.0];
mgg_qW_sig_frac_cat3[0.5, 0.3, 1.0];

MggGaussSigqW_cat3 = Gaussian(mgg, mgg_qW_sig_m0_cat3, mgg_qW_sig_gsigma_cat3);
MggCBSigqW_cat3    = CBShape(mgg, mgg_qW_sig_m0_cat3, mgg_qW_sig_sigma_cat3, mgg_qW_sig_alpha_cat3, mgg_qW_sig_n_cat3);
MggSigqW_cat3      = AddPdf(MggGaussSigqW_cat3, MggCBSigqW_cat3, mgg_qW_sig_frac_cat3);

mgg_qW_sig_m0_cat4[2000.0, 900.0, 4100.0];
mgg_qW_sig_sigma_cat4[100, 50.0, 1000.0];
mgg_qW_sig_alpha_cat4[ 0.8, 0.0, 3.0]; 
mgg_qW_sig_n_cat4[130, 0.00001, 1000.0]; 
mgg_qW_sig_gsigma_cat4[100, 50.0, 1000.0];
mgg_qW_sig_frac_cat4[0.5, 0.3, 1.0];

MggGaussSigqW_cat4 = Gaussian(mgg, mgg_qW_sig_m0_cat4, mgg_qW_sig_gsigma_cat4);
MggCBSigqW_cat4    = CBShape(mgg, mgg_qW_sig_m0_cat4, mgg_qW_sig_sigma_cat4, mgg_qW_sig_alpha_cat4, mgg_qW_sig_n_cat4);
MggSigqW_cat4      = AddPdf(MggGaussSigqW_cat4, MggCBSigqW_cat4, mgg_qW_sig_frac_cat4);


mgg_qW_sig_m0_cat5[2000.0, 900.0, 4100.0];
mgg_qW_sig_sigma_cat5[100, 50.0, 1000.0];
mgg_qW_sig_alpha_cat5[ 0.8, 0.0, 3.0]; 
mgg_qW_sig_n_cat5[130, 0.00001, 1000.0]; 
mgg_qW_sig_gsigma_cat5[100, 50.0, 1000.0];
mgg_qW_sig_frac_cat5[0.5, 0.3, 1.0];

MggGaussSigqW_cat5 = Gaussian(mgg, mgg_qW_sig_m0_cat5, mgg_qW_sig_gsigma_cat5);
MggCBSigqW_cat5    = CBShape(mgg, mgg_qW_sig_m0_cat5, mgg_qW_sig_sigma_cat5, mgg_qW_sig_alpha_cat5, mgg_qW_sig_n_cat5);
MggSigqW_cat5      = AddPdf(MggGaussSigqW_cat5, MggCBSigqW_cat5, mgg_qW_sig_frac_cat5);






mgg_qZ_sig_m0[2000.0, 900.0, 4100.0];
mgg_qZ_sig_sigma[100, 50.0, 1000.0];
mgg_qZ_sig_alpha[0.8, 0.5, 3]; 
mgg_qZ_sig_n[13.0, 0.5, 10]; 
mgg_qZ_sig_gsigma[100, 50.0, 1000.0];
mgg_qZ_sig_frac[0.5, 0.3, 1.0];

MggGaussSigqZ = Gaussian(mgg, mgg_qZ_sig_m0, mgg_qZ_sig_gsigma);
MggCBSigqZ    = CBShape(mgg, mgg_qZ_sig_m0, mgg_qZ_sig_sigma, mgg_qZ_sig_alpha, mgg_qZ_sig_n);
MggSigqZ      = AddPdf(MggGaussSigqZ, MggCBSigqZ, mgg_qZ_sig_frac);

mgg_qZ_sig_m0_cat0[2000.0, 900.0, 4100.0];
mgg_qZ_sig_sigma_cat0[100, 50.0, 1000.0];
mgg_qZ_sig_alpha_cat0[ 0.8, 0.0, 3.0]; 
mgg_qZ_sig_n_cat0[130, 0.00001, 1000.0]; 
mgg_qZ_sig_gsigma_cat0[100, 50.0, 1000.0];
mgg_qZ_sig_frac_cat0[0.5, 0.3, 1.0];


MggGaussSigqZ_cat0 = Gaussian(mgg, mgg_qZ_sig_m0_cat0, mgg_qZ_sig_gsigma_cat0);
MggCBSigqZ_cat0    = CBShape(mgg, mgg_qZ_sig_m0_cat0, mgg_qZ_sig_sigma_cat0, mgg_qZ_sig_alpha_cat0, mgg_qZ_sig_n_cat0);
MggSigqZ_cat0      = AddPdf(MggGaussSigqZ_cat0, MggCBSigqZ_cat0, mgg_qZ_sig_frac_cat0);

mgg_qZ_sig_m0_cat1[2000.0, 900.0, 4100.0];
mgg_qZ_sig_sigma_cat1[100, 50.0, 1000.0];
mgg_qZ_sig_alpha_cat1[ 0.8, 0.0, 3.0]; 
mgg_qZ_sig_n_cat1[130, 0.00001, 1000.0]; 
mgg_qZ_sig_gsigma_cat1[100, 50.0, 1000.0];
mgg_qZ_sig_frac_cat1[0.5, 0.3, 1.0];

MggGaussSigqZ_cat1 = Gaussian(mgg, mgg_qZ_sig_m0_cat1, mgg_qZ_sig_gsigma_cat1);
MggCBSigqZ_cat1    = CBShape(mgg, mgg_qZ_sig_m0_cat1, mgg_qZ_sig_sigma_cat1, mgg_qZ_sig_alpha_cat1, mgg_qZ_sig_n_cat1);
MggSigqZ_cat1      = AddPdf(MggGaussSigqZ_cat1, MggCBSigqZ_cat1, mgg_qZ_sig_frac_cat1);

mgg_qZ_sig_m0_cat2[2000.0, 900.0, 4100.0];
mgg_qZ_sig_sigma_cat2[100, 50.0, 1000.0];
mgg_qZ_sig_alpha_cat2[ 0.8, 0.0, 3.0]; 
mgg_qZ_sig_n_cat2[130, 0.00001, 1000.0]; 
mgg_qZ_sig_gsigma_cat2[100, 50.0, 1000.0];
mgg_qZ_sig_frac_cat2[0.5, 0.3, 1.0];

MggGaussSigqZ_cat2 = Gaussian(mgg, mgg_qZ_sig_m0_cat2, mgg_qZ_sig_gsigma_cat2);
MggCBSigqZ_cat2    = CBShape(mgg, mgg_qZ_sig_m0_cat2, mgg_qZ_sig_sigma_cat2, mgg_qZ_sig_alpha_cat2, mgg_qZ_sig_n_cat2);
MggSigqZ_cat2      = AddPdf(MggGaussSigqZ_cat2, MggCBSigqZ_cat2, mgg_qZ_sig_frac_cat2);

mgg_qZ_sig_m0_cat3[2000.0, 900.0, 4100.0];
mgg_qZ_sig_sigma_cat3[100, 50.0, 1000.0];
mgg_qZ_sig_alpha_cat3[ 0.8, 0.0, 3.0]; 
mgg_qZ_sig_n_cat3[130, 0.00001, 1000.0]; 
mgg_qZ_sig_gsigma_cat3[100, 50.0, 1000.0];
mgg_qZ_sig_frac_cat3[0.5, 0.3, 1.0];

MggGaussSigqZ_cat3 = Gaussian(mgg, mgg_qZ_sig_m0_cat3, mgg_qZ_sig_gsigma_cat3);
MggCBSigqZ_cat3    = CBShape(mgg, mgg_qZ_sig_m0_cat3, mgg_qZ_sig_sigma_cat3, mgg_qZ_sig_alpha_cat3, mgg_qZ_sig_n_cat3);
MggSigqZ_cat3      = AddPdf(MggGaussSigqZ_cat3, MggCBSigqZ_cat3, mgg_qZ_sig_frac_cat3);

mgg_qZ_sig_m0_cat4[2000.0, 900.0, 4100.0];
mgg_qZ_sig_sigma_cat4[100, 50.0, 1000.0];
mgg_qZ_sig_alpha_cat4[ 0.8, 0.0, 3.0]; 
mgg_qZ_sig_n_cat4[130, 0.00001, 1000.0]; 
mgg_qZ_sig_gsigma_cat4[100, 50.0, 1000.0];
mgg_qZ_sig_frac_cat4[0.5, 0.3, 1.0];

MggGaussSigqZ_cat4 = Gaussian(mgg, mgg_qZ_sig_m0_cat4, mgg_qZ_sig_gsigma_cat4);
MggCBSigqZ_cat4    = CBShape(mgg, mgg_qZ_sig_m0_cat4, mgg_qZ_sig_sigma_cat4, mgg_qZ_sig_alpha_cat4, mgg_qZ_sig_n_cat4);
MggSigqZ_cat4      = AddPdf(MggGaussSigqZ_cat4, MggCBSigqZ_cat4, mgg_qZ_sig_frac_cat4);


mgg_qZ_sig_m0_cat5[2000.0, 900.0, 4100.0];
mgg_qZ_sig_sigma_cat5[100, 50.0, 1000.0];
mgg_qZ_sig_alpha_cat5[ 0.8, 0.0, 3.0]; 
mgg_qZ_sig_n_cat5[130, 0.00001, 1000.0]; 
mgg_qZ_sig_gsigma_cat5[100, 50.0, 1000.0];
mgg_qZ_sig_frac_cat5[0.5, 0.3, 1.0];

MggGaussSigqZ_cat5 = Gaussian(mgg, mgg_qZ_sig_m0_cat5, mgg_qZ_sig_gsigma_cat5);
MggCBSigqZ_cat5    = CBShape(mgg, mgg_qZ_sig_m0_cat5, mgg_qZ_sig_sigma_cat5, mgg_qZ_sig_alpha_cat5, mgg_qZ_sig_n_cat5);
MggSigqZ_cat5      = AddPdf(MggGaussSigqZ_cat5, MggCBSigqZ_cat5, mgg_qZ_sig_frac_cat5);




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
