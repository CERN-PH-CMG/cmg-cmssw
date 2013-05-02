// *Br    0 :mgg       : mgg/F                                                  *
// *Entries :     3855 : Total  Size=      15958 bytes  File Size  =       1148 *
// *Baskets :        1 : Basket Size=      32000 bytes  Compression=  13.49     *
// *............................................................................*
// *Br    1 :mtot      : mtot/F                                                 *
// *Entries :     3855 : Total  Size=      15963 bytes  File Size  =        172 *
// *Baskets :        1 : Basket Size=      32000 bytes  Compression=  90.08     *
// *............................................................................*
// *Br    2 :evWeight  : evWeight/F                                             *
// *Entries :     3855 : Total  Size=      15983 bytes  File Size  =       1223 *
// *Baskets :        1 : Basket Size=      32000 bytes  Compression=  12.67     *
// *............................................................................*
// *Br    3 :Weight    : Weight/F                                               *
// *Entries :     3855 : Total  Size=      15973 bytes  File Size  =        967 *
// *Baskets :        1 : Basket Size=      32000 bytes  Compression=  16.02     *
// *............................................................................*
// *Br    4 :cut_based_ct : cut_based_ct/I                                      *
// *Entries :     3855 : Total  Size=      16003 bytes  File Size  =       2152 *
// *Baskets :        1 : Basket Size=      32000 bytes  Compression=   7.20     *
// *............................................................................*
// *Br    5 :BDT_ct    : BDT_ct/I                                               *
// *Entries :     3855 : Total  Size=      15973 bytes  File Size  =       2487 *
// *Baskets :        1 : Basket Size=      32000 bytes  Compression=   6.23     *
// *............................................................................*

{


  double mgg, mjj,evWeight, mtot, normWeight;
 int categories;

 evWeight = 1.0;
 normWeight = 1;

 for (int iSample = 1; iSample < 3; iSample++){
   
   string inFile("WprimeOUT");
   if (iSample == 1) inFile = string("dijetWtag_Moriond_WW");
   if (iSample == 2) inFile = string("dijetWtag_Moriond_ZZ");

   for (int iMass = 0; iMass<21; iMass++){

     string sInFile = "~hinzmann/public/nsubanalysis/" + inFile + "" + Form("HppOUT%d.root", 1000+iMass*100);
     cout << sInFile.c_str() << endl;
     TFile file0(sInFile.c_str(), "read");


     string sOutFile = "MiniTrees/Signal_VV/" + inFile + Form("HppOUT%d_miniTree.root", 1000+iMass*100);
     TFile f1(sOutFile.c_str(), "recreate");
     f1.cd();

     TTree *TCVARS = new TTree("TCVARS", "VV selection");
     TCVARS->Branch("mgg",&mgg,"mgg/D");

     TCVARS->Branch("evWeight",&evWeight,"evWeight/D");
     TCVARS->Branch("normWeight",&normWeight,"normWeight/D");
     
     TCVARS->Branch("categories",&categories,"categories/I");

  
     double dMass = 1000.+iMass*100.;


     
     for (int iCat = 0; iCat < 6; iCat++){
      
       /*
       TH1D* hMass = (TH1D*) file0.Get("dijet_mass_2mtag_0mdtag;1");
       if (iCat == 1) hMass = (TH1D*) file0.Get("dijet_mass_2mtag_1mdtag;1");
       if (iCat == 2) TH1D* hMass = (TH1D*) file0.Get("dijet_mass_2mtag_2mdtag;1");
       if (iCat == 3) TH1D* hMass = (TH1D*) file0.Get("dijet_mass_1mtag_0mdtag;1");
       if (iCat == 4) TH1D* hMass = (TH1D*) file0.Get("dijet_mass_1mtag_1mdtag;1");       
       if (iCat == 5) TH1D* hMass = (TH1D*) file0.Get("dijet_mass_0mtag;1");
       */
       /* 
       TH1D* hMass = (TH1D*) file0.Get("dijet_mass_2mtag_0nstag;1");
       if (iCat == 1) hMass = (TH1D*) file0.Get("dijet_mass_2mtag_1nstag;1");
       if (iCat == 2) TH1D* hMass = (TH1D*) file0.Get("dijet_mass_2mtag_2nstag;1");
       if (iCat == 3) TH1D* hMass = (TH1D*) file0.Get("dijet_mass_1mtag_0nstag;1");
       if (iCat == 4) TH1D* hMass = (TH1D*) file0.Get("dijet_mass_1mtag_1nstag;1");       
       if (iCat == 5) TH1D* hMass = (TH1D*) file0.Get("dijet_mass_0mtag;1");
       */

       TH1D* hMass = (TH1D*) file0.Get("dijet_mass_2mtag_2hptag;1");
       if (iCat == 1) hMass = (TH1D*) file0.Get("dijet_mass_2mtag_1hptag_1lptag;1");
       if (iCat == 2) hMass = (TH1D*) file0.Get("dijet_mass_2mtag_2lptag;1");
       if (iCat == 3) hMass = (TH1D*) file0.Get("dijet_mass_1mtag_1hptag;1");
       if (iCat == 4) hMass = (TH1D*) file0.Get("dijet_mass_1mtag_1lptag;1");
       if (iCat == 5) hMass = (TH1D*) file0.Get("dijet_mass_0mtag;1");

       TAxis* Axis =   hMass->GetXaxis();
       for (int i = 1 ; i < hMass->GetNbinsX()+1; i++){
	 if (hMass->GetBinCenter(i) < dMass*0.7 || hMass->GetBinCenter(i) > dMass*1.3) continue;
	 int N = abs(hMass->GetBinContent(i));
	 if (i%1000 == 0) cout << "i = " << i << endl;
	 
	 mgg = Axis->GetBinCenter(i);
	 
	 for (int k = 0; k < N; k++) {
	   categories = iCat;
	   TCVARS->Fill();
	 }
       }
     }
   

     TCVARS->Write();
     f1.Close();
     file0.Close();
     
   }

 }


}


