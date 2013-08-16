{
 TFile *_file0 = TFile::Open("MASS_SHAPES_FATJETS_wR9_to_R17_20110525.root");

TObjArray* R = new TObjArray();

// char* Res = "RSqq";
// char* Res = "RSgg";
 char* Res = "Qstar";

 TH1D* QstarDT6_500 = (TH1D* ) _file0->Get("QstarDT6_500_MassPF_Fat11_MassShape;1");
TH1D*  QstarDT6_700 =  (TH1D* )_file0->Get("QstarDT6_700_MassPF_Fat11_MassShape;1");
TH1D*  QstarDT6_1200 =  (TH1D* )_file0->Get("QstarDT6_1200_MassPF_Fat11_MassShape;1");
TH1D*  QstarDT6_2000 =  (TH1D* )_file0->Get("QstarDT6_2000_MassPF_Fat11_MassShape;1");
TH1D*  QstarDT6_3500 =  (TH1D* )_file0->Get("QstarDT6_3500_MassPF_Fat11_MassShape;1");
 TH1D* QstarDT6_500_PF = (TH1D* ) _file0->Get("QstarDT6_500_MassPF_MassShape;1");
TH1D*  QstarDT6_700_PF =  (TH1D* )_file0->Get("QstarDT6_700_MassPF_MassShape;1");
TH1D*  QstarDT6_1200_PF =  (TH1D* )_file0->Get("QstarDT6_1200_MassPF_MassShape;1");
TH1D*  QstarDT6_2000_PF =  (TH1D* )_file0->Get("QstarDT6_2000_MassPF_MassShape;1");
TH1D*  QstarDT6_3500_PF =  (TH1D* )_file0->Get("QstarDT6_3500_MassPF_MassShape;1");
 TH1D* QstarDT6_500_Calo = (TH1D* ) _file0->Get("QstarDT6_500_MassCalo_MassShape;1");
TH1D*  QstarDT6_700_Calo =  (TH1D* )_file0->Get("QstarDT6_700_MassCalo_MassShape;1");
TH1D*  QstarDT6_1200_Calo =  (TH1D* )_file0->Get("QstarDT6_1200_MassCalo_MassShape;1");
TH1D*  QstarDT6_2000_Calo =  (TH1D* )_file0->Get("QstarDT6_2000_MassCalo_MassShape;1");
TH1D*  QstarDT6_3500_Calo =  (TH1D* )_file0->Get("QstarDT6_3500_MassCalo_MassShape;1");

if (Res == "RSgg"){
 QstarDT6_500 = (TH1D*) _file0->Get("RSGravitonDT6_500GG_MassPF_Fat11_MassShape;1");
 QstarDT6_700 =  (TH1D*)_file0->Get("RSGravitonDT6_700GG_MassPF_Fat11_MassShape;1");
 QstarDT6_1200 =  (TH1D*)_file0->Get("RSGravitonDT6_1200GG_MassPF_Fat11_MassShape;1");
 QstarDT6_2000 =  (TH1D*)_file0->Get("RSGravitonDT6_2000GG_MassPF_Fat11_MassShape;1");
 QstarDT6_3500 =  (TH1D*)_file0->Get("RSGravitonDT6_3500GG_MassPF_Fat11_MassShape;1");
 QstarDT6_500_PF = (TH1D*) _file0->Get("RSGravitonDT6_500GG_MassPF_MassShape;1");
 QstarDT6_700_PF =  (TH1D*)_file0->Get("RSGravitonDT6_700GG_MassPF_MassShape;1");
 QstarDT6_1200_PF =  (TH1D*)_file0->Get("RSGravitonDT6_1200GG_MassPF_MassShape;1");
 QstarDT6_2000_PF =  (TH1D*)_file0->Get("RSGravitonDT6_2000GG_MassPF_MassShape;1");
 QstarDT6_3500_PF =  (TH1D*)_file0->Get("RSGravitonDT6_3500GG_MassPF_MassShape;1");
 QstarDT6_500_Calo = (TH1D*) _file0->Get("RSGravitonDT6_500GG_MassCalo_MassShape;1");
 QstarDT6_700_Calo =  (TH1D*)_file0->Get("RSGravitonDT6_700GG_MassCalo_MassShape;1");
 QstarDT6_1200_Calo =  (TH1D*)_file0->Get("RSGravitonDT6_1200GG_MassCalo_MassShape;1");
 QstarDT6_2000_Calo =  (TH1D*)_file0->Get("RSGravitonDT6_2000GG_MassCalo_MassShape;1");
 QstarDT6_3500_Calo =  (TH1D*)_file0->Get("RSGravitonDT6_3500GG_MassCalo_MassShape;1");
   } else if (Res == "RSqq"){


 QstarDT6_500 = (TH1D*) _file0->Get("RSGravitonDT6_500QQ_MassPF_Fat11_MassShape;1");
 QstarDT6_700 =  (TH1D*)_file0->Get("RSGravitonDT6_700QQ_MassPF_Fat11_MassShape;1");
 QstarDT6_1200 =  (TH1D*)_file0->Get("RSGravitonDT6_1200QQ_MassPF_Fat11_MassShape;1");
 QstarDT6_2000 =  (TH1D*)_file0->Get("RSGravitonDT6_2000QQ_MassPF_Fat11_MassShape;1");
 QstarDT6_3500 =  (TH1D*)_file0->Get("RSGravitonDT6_3500QQ_MassPF_Fat11_MassShape;1");
 QstarDT6_500_PF = (TH1D*) _file0->Get("RSGravitonDT6_500QQ_MassPF_MassShape;1");
 QstarDT6_700_PF =  (TH1D*)_file0->Get("RSGravitonDT6_700QQ_MassPF_MassShape;1");
 QstarDT6_1200_PF =  (TH1D*)_file0->Get("RSGravitonDT6_1200QQ_MassPF_MassShape;1");
 QstarDT6_2000_PF =  (TH1D*)_file0->Get("RSGravitonDT6_2000QQ_MassPF_MassShape;1");
 QstarDT6_3500_PF =  (TH1D*)_file0->Get("RSGravitonDT6_3500QQ_MassPF_MassShape;1");
 QstarDT6_500_Calo = (TH1D*) _file0->Get("RSGravitonDT6_500QQ_MassCalo_MassShape;1");
 QstarDT6_700_Calo =  (TH1D*)_file0->Get("RSGravitonDT6_700QQ_MassCalo_MassShape;1");
 QstarDT6_1200_Calo =  (TH1D*)_file0->Get("RSGravitonDT6_1200QQ_MassCalo_MassShape;1");
 QstarDT6_2000_Calo =  (TH1D*)_file0->Get("RSGravitonDT6_2000QQ_MassCalo_MassShape;1");
 QstarDT6_3500_Calo =  (TH1D*)_file0->Get("RSGravitonDT6_3500QQ_MassCalo_MassShape;1");

 }


 R->Add(QstarDT6_500);
 R->Add(QstarDT6_700);
 R->Add(QstarDT6_1200);
 R->Add(QstarDT6_2000);
 R->Add(QstarDT6_3500);

 R->Add(QstarDT6_500_PF);
 R->Add(QstarDT6_700_PF);
 R->Add(QstarDT6_1200_PF);
 R->Add(QstarDT6_2000_PF);
 R->Add(QstarDT6_3500_PF);

 R->Add(QstarDT6_500_Calo);
 R->Add(QstarDT6_700_Calo);
 R->Add(QstarDT6_1200_Calo);
 R->Add(QstarDT6_2000_Calo);
 R->Add(QstarDT6_3500_Calo);

 vector<string> titles;
 titles.push_back(string("QstarDT6_y500_PF_Fat11"));
 titles.push_back("QstarDT6_y700_PF_Fat11");
 titles.push_back("QstarDT6_y1200_PF_Fat11");
 titles.push_back("QstarDT6_y2000_PF_Fat11");
 titles.push_back("QstarDT6_y3500_PF_Fat11");

 titles.push_back(string("QstarDT6_y500_PF"));
 titles.push_back("QstarDT6_y700_PF");
 titles.push_back("QstarDT6_y1200_PF");
 titles.push_back("QstarDT6_y2000_PF");
 titles.push_back("QstarDT6_y3500_PF");

 titles.push_back(string("QstarDT6_y500_Calo"));
 titles.push_back("QstarDT6_y700_Calo");
 titles.push_back("QstarDT6_y1200_Calo");
 titles.push_back("QstarDT6_y2000_Calo");
 titles.push_back("QstarDT6_y3500_Calo");




double QstarDT6_y500[50]= {146, 172, 193, 193, 200, 271, 286, 331, 375, 395, 477, 543, 613, 629, 769, 853, 908, 1033, 1123, 1265, 1391, 1474, 1522, 1657, 1821, 1880, 2076, 2135, 2478, 2764, 3100, 3376, 3592, 3832, 3812, 3430, 3045, 2493, 1954, 1417, 1083, 764, 609, 439, 381, 311, 281, 251, 222, 192};


  double QstarDT6_y700[50] = {172, 195, 202, 247, 249, 334, 368, 403, 479, 522, 630, 652, 733, 827, 929, 1067, 1195, 1377, 1482, 1605, 1764, 1859, 2055, 2092, 2213, 2441, 2517, 2887, 3189, 3598, 4070, 4403, 4878, 5220, 4993, 4577, 3687, 3053, 2198, 1493, 1034, 734, 541, 444, 359, 314, 268, 238, 211, 185};

   double QstarDT6_y1200[50] = {168, 183, 233, 239, 280, 311, 334, 410, 460, 508, 581, 643, 723, 806, 947, 986, 1162, 1328, 1490, 1513, 1661, 1819, 1951, 1917, 2074, 2281, 2400, 2780, 2990, 3386, 3926, 4668, 5247, 5685, 5452, 4709, 3660, 2436, 1633, 958, 643, 470, 349, 284, 223, 242, 173, 157, 136, 97};


   double QstarDT6_y2000[50] = {147, 189, 199, 241, 248, 286, 328, 313, 390, 420, 427, 556, 638, 684, 746, 856, 965, 1095, 1233, 1296, 1437, 1489, 1543, 1677, 1654, 1802, 2033, 2172, 2487, 2857, 3265, 3973, 4542, 4996, 4770, 3921, 2751, 1601, 961, 563, 355, 239, 182, 145, 117, 88, 81, 64, 39, 35};

   double QstarDT6_y3500[50] = { 559, 546, 576, 633, 711, 693, 698, 767, 827, 813, 930, 951, 987, 1068, 1129, 1139, 1262, 1312, 1422, 1499, 1547, 1588, 1708, 1678, 1803, 1768, 1894, 2218, 2481, 2733, 3138, 3742, 4484, 4626, 4130, 2801, 1605, 735, 336, 198, 99, 81, 46, 34, 17, 14, 14, 7, 6, 4};

///////////gg
   if (Res == "RSgg"){
   double QstarDT6_y500[50]= {262, 274, 288, 343, 385, 433, 480, 455, 519, 523, 592, 586, 645, 693, 681, 744, 719, 772, 833, 803, 836, 795, 876, 866, 830, 914, 920, 910, 1111, 1066, 1096, 1061, 1010, 967, 814, 669, 547, 417, 294, 229, 174, 146, 135, 132, 102, 120, 114, 94, 84, 86};
   double QstarDT6_y700[50]= {197, 200, 232, 251, 280, 305, 329, 334, 335, 382, 406, 434, 387, 456, 483, 534, 467, 535, 572, 566, 586, 584, 538, 604, 602, 587, 634, 629, 702, 772, 799, 773, 756, 704, 586, 504, 324, 223, 184, 109, 107, 107, 77, 84, 63, 64, 55, 50, 48, 26};
   double QstarDT6_y1200[50]= {124, 155, 163, 186, 180, 188, 206, 238, 241, 255, 293, 282, 290, 309, 330, 325, 369, 356, 374, 394, 374, 405, 379, 403, 383, 400, 413, 508, 464, 515, 551, 611, 621, 582, 419, 312, 206, 131, 78, 61, 57, 50, 42, 37, 30, 22, 28, 20, 21, 18};
   double QstarDT6_y2000[50]= {91, 96, 100, 101, 104, 98, 106, 133, 140, 148, 140, 149, 169, 168, 152, 160, 181, 187, 216, 198, 224, 210, 203, 237, 190, 219, 225, 246, 240, 284, 342, 322, 349, 333, 270, 160, 94, 56, 45, 22, 18, 16, 10, 8, 6, 11, 10, 4, 4, 3};
   double QstarDT6_y3500[50]= {62, 51, 42, 70, 53, 82, 69, 52, 66, 64, 58, 64, 44, 60, 52, 52, 70, 74, 53, 66, 54, 68, 68, 64, 71, 64, 75, 62, 80, 76, 88, 122, 128, 117, 71, 45, 26, 9, 3, 2, 4, 1, 0, 0, 1, 0, 0, 0, 0, 1};
   } else if (Res == "RSqq"){

/////////////qq

   double QstarDT6_y500[50]= {24, 16, 20, 23, 28, 30, 27, 30, 36, 35, 24, 53, 37, 59, 60, 68, 94, 83, 105, 95, 128, 137, 137, 167, 164, 169, 235, 220, 258, 276, 295, 372, 413, 441, 498,543, 515, 477, 379, 271, 198, 145, 96, 63, 39, 32, 36, 19, 16, 19};
   double QstarDT6_y700[50]= {21, 15, 15, 21, 28, 36, 23, 40, 36, 46, 40, 73, 49, 61, 66, 65, 93, 108, 113, 108, 119, 160, 175, 167, 211, 210, 250, 270, 279, 333, 362, 445, 502, 609, 642, 660, 674, 540, 442, 253, 163, 118, 53, 44, 35, 30, 18, 13, 15, 11};
   double QstarDT6_y1200[50]= {38, 44, 43, 49, 44, 49, 47, 60, 58, 64, 70, 89, 71, 108, 117, 135, 167, 183, 211, 226, 257, 299, 295, 331, 309, 365, 434, 442, 475, 617, 724, 882, 1011, 1319, 1385, 1408, 1165, 931, 521, 277, 135, 86, 57, 26, 23, 23, 14, 17, 11, 14};
   double QstarDT6_y2000[50]= {42, 57, 48, 51, 58, 56, 61, 80, 92, 89, 93, 114, 125, 167, 170, 215, 206, 264, 302, 321, 342, 385, 441, 489, 464, 521, 616, 664, 759, 879, 1067, 1304, 1611, 2048, 2215, 2136, 1622, 1013, 497, 224, 112, 60, 40, 26, 21, 14, 15, 6, 4, 3};
   double QstarDT6_y3500[50]= {103, 75, 107, 96, 92, 109, 151, 124, 149, 169, 170, 201, 161, 214, 249, 277, 287, 362, 344, 407, 414, 466, 493, 537, 554, 617, 680, 769, 930, 968, 1202, 1524, 1947, 2451, 2482, 1890, 1169, 479, 183, 43, 25, 13, 8, 4, 4, 1, 2, 2, 0, 1};

 
   }





   double bincenter[50]={
  0.31,  0.33,  0.35,  0.37,  0.39,  0.41,  0.43,  0.45,  0.47,  0.49,
  0.51,  0.53,  0.55,  0.57,  0.59,  0.61,  0.63,  0.65,  0.67,  0.69,
  0.71,  0.73,  0.75,  0.77,  0.79,  0.81,  0.83,  0.85,  0.87,  0.89,
  0.91,  0.93,  0.95,  0.97,  0.99,  1.01,  1.03,  1.05,  1.07,  1.09,
  1.11,  1.13,  1.15,  1.17,  1.19,  1.21,  1.23,  1.25,  1.27,  1.29};

   
   TH1D* QstarDT6_500_Sertac = new TH1D("QstarDT6_500_Sertac", "M(Q*) = 500 GeV; M_{12}/ 500; ", 49, bincenter);
   TH1D* QstarDT6_700_Sertac = new TH1D("QstarDT6_700_Sertac", "M(Q*) = 700 GeV; M_{12}/ 700;", 49, bincenter);
   TH1D* QstarDT6_1200_Sertac = new TH1D("QstarDT6_1200_Sertac", "M(Q*) = 1200 GeV; M_{12}/ 1200;", 49, bincenter);
   TH1D* QstarDT6_2000_Sertac = new TH1D("QstarDT6_2000_Sertac", "M(Q*) = 2000 GeV; M_{12}/ 2000;", 49, bincenter);
   TH1D* QstarDT6_3500_Sertac = new TH1D("QstarDT6_3500_Sertac", "M(Q*) = 3500 GeV; M_{12}/ 3500;", 49, bincenter);
   
   if (Res == "RSgg"){

   QstarDT6_500_Sertac = new TH1D("QstarDT6_500_Sertac", "M(RSgg) = 500 GeV; M_{12}/ 500; ", 49, bincenter);
   QstarDT6_700_Sertac = new TH1D("QstarDT6_700_Sertac", "M(RSgg) = 700 GeV; M_{12}/ 700;", 49, bincenter);
   QstarDT6_1200_Sertac = new TH1D("QstarDT6_1200_Sertac", "M(RSgg) = 1200 GeV; M_{12}/ 1200;", 49, bincenter);
   QstarDT6_2000_Sertac = new TH1D("QstarDT6_2000_Sertac", "M(RSgg) = 2000 GeV; M_{12}/ 2000;", 49, bincenter);
   QstarDT6_3500_Sertac = new TH1D("QstarDT6_3500_Sertac", "M(RSgg) = 3500 GeV; M_{12}/ 3500;", 49, bincenter);
   } else if (Res == "RSqq"){
     QstarDT6_500_Sertac = new TH1D("QstarDT6_500_Sertac", "M(RSqq) = 500 GeV; M_{12}/ 500; ", 49, bincenter);
   QstarDT6_700_Sertac = new TH1D("QstarDT6_700_Sertac", "M(RSqq) = 700 GeV; M_{12}/ 700;", 49, bincenter);
   QstarDT6_1200_Sertac = new TH1D("QstarDT6_1200_Sertac", "M(RSqq) = 1200 GeV; M_{12}/ 1200;", 49, bincenter);
   QstarDT6_2000_Sertac = new TH1D("QstarDT6_2000_Sertac", "M(RSqq) = 2000 GeV; M_{12}/ 2000;", 49, bincenter);
   QstarDT6_3500_Sertac = new TH1D("QstarDT6_3500_Sertac", "M(RSqq) = 3500 GeV; M_{12}/ 3500;", 49, bincenter);

   }

   QstarDT6_500_Sertac->SetFillColor(kYellow);
   QstarDT6_700_Sertac->SetFillColor(kYellow);
   QstarDT6_1200_Sertac->SetFillColor(kYellow);
   QstarDT6_2000_Sertac->SetFillColor(kYellow);
   QstarDT6_3500_Sertac->SetFillColor(kYellow);

   QstarDT6_500_Sertac->SetLineColor(kYellow);
   QstarDT6_700_Sertac->SetLineColor(kYellow);
   QstarDT6_1200_Sertac->SetLineColor(kYellow);
   QstarDT6_2000_Sertac->SetLineColor(kYellow);
   QstarDT6_3500_Sertac->SetLineColor(kYellow);

   QstarDT6_500_Sertac->SetStats(0);
   QstarDT6_700_Sertac->SetStats(0);
   QstarDT6_1200_Sertac->SetStats(0);
   QstarDT6_2000_Sertac->SetStats(0);
   QstarDT6_3500_Sertac->SetStats(0);

   QstarDT6_500->SetStats(0);
   QstarDT6_700->SetStats(0);
   QstarDT6_1200->SetStats(0);
   QstarDT6_2000->SetStats(0);
   QstarDT6_3500->SetStats(0);

   QstarDT6_500_Calo->SetLineWidth(2);
   QstarDT6_700_Calo->SetLineWidth(2);
   QstarDT6_1200_Calo->SetLineWidth(2);
   QstarDT6_2000_Calo->SetLineWidth(2);
   QstarDT6_3500_Calo->SetLineWidth(2);

   QstarDT6_500_PF->SetLineWidth(2);
   QstarDT6_700_PF->SetLineWidth(2);
   QstarDT6_1200_PF->SetLineWidth(2);
   QstarDT6_2000_PF->SetLineWidth(2);
   QstarDT6_3500_PF->SetLineWidth(2);



   QstarDT6_500_PF->SetLineColor(kMagenta);
   QstarDT6_700_PF->SetLineColor(kMagenta);
   QstarDT6_1200_PF->SetLineColor(kMagenta);
   QstarDT6_2000_PF->SetLineColor(kMagenta);
   QstarDT6_3500_PF->SetLineColor(kMagenta);

   QstarDT6_500->SetLineColor(kBlue);
   QstarDT6_700->SetLineColor(kBlue);
   QstarDT6_1200->SetLineColor(kBlue);
   QstarDT6_2000->SetLineColor(kBlue);
   QstarDT6_3500->SetLineColor(kBlue);


   for (int i = 1; i < 50; i++){
       
     QstarDT6_500_Sertac->SetBinContent(i, QstarDT6_y500[i]);
     QstarDT6_700_Sertac->SetBinContent(i, QstarDT6_y700[i]);
     QstarDT6_1200_Sertac->SetBinContent(i, QstarDT6_y1200[i]);
     QstarDT6_2000_Sertac->SetBinContent(i, QstarDT6_y2000[i]);
     QstarDT6_3500_Sertac->SetBinContent(i, QstarDT6_y3500[i]);

   }

   double IntDen = QstarDT6_500_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_500_Calo->Integral(20, 49);
   QstarDT6_500_Calo->Scale(IntDen/IntNum);
   double IntDen = QstarDT6_700_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_700_Calo->Integral(20, 49);
   QstarDT6_700_Calo->Scale(IntDen/IntNum);
   double IntDen = QstarDT6_1200_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_1200_Calo->Integral(20, 49);
   QstarDT6_1200_Calo->Scale(IntDen/IntNum);
   double IntDen = QstarDT6_2000_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_2000_Calo->Integral(20, 49);
   QstarDT6_2000_Calo->Scale(IntDen/IntNum);
   double IntDen = QstarDT6_3500_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_3500_Calo->Integral(20, 49);
   QstarDT6_3500_Calo->Scale(IntDen/IntNum);

   double IntDen = QstarDT6_500_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_500_PF->Integral(20, 49);
   QstarDT6_500_PF->Scale(IntDen/IntNum);
   double IntDen = QstarDT6_700_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_700_PF->Integral(20, 49);
   QstarDT6_700_PF->Scale(IntDen/IntNum);
   double IntDen = QstarDT6_1200_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_1200_PF->Integral(20, 49);
   QstarDT6_1200_PF->Scale(IntDen/IntNum);
   double IntDen = QstarDT6_2000_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_2000_PF->Integral(20, 49);
   QstarDT6_2000_PF->Scale(IntDen/IntNum);
   double IntDen = QstarDT6_3500_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_3500_PF->Integral(20, 49);
   QstarDT6_3500_PF->Scale(IntDen/IntNum);

   double IntDen = QstarDT6_500_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_500->Integral(20, 49);
   QstarDT6_500->Scale(IntDen/IntNum);
   double IntDen = QstarDT6_700_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_700->Integral(20, 49);
   QstarDT6_700->Scale(IntDen/IntNum);
   double IntDen = QstarDT6_1200_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_1200->Integral(20, 49);
   QstarDT6_1200->Scale(IntDen/IntNum);
   double IntDen = QstarDT6_2000_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_2000->Integral(20, 49);
   QstarDT6_2000->Scale(IntDen/IntNum);
   double IntDen = QstarDT6_3500_Sertac->Integral(20, 49);
   double IntNum = QstarDT6_3500->Integral(20, 49);
   QstarDT6_3500->Scale(IntDen/IntNum);



   TCanvas* canvas = new TCanvas("canvas", "", 1000, 1000);
   canvas->Divide(2,2);
   /*  
   canvas->cd(1);
   QstarDT6_500_Sertac->Draw();
   QstarDT6_500_Calo->Draw("SAME");
   QstarDT6_500_PF->Draw("SAME");
   */
   canvas->cd(1);
   QstarDT6_700_Sertac->Draw();
   QstarDT6_700_Calo->Draw("SAME");
   QstarDT6_700_PF->Draw("SAME");

   canvas->cd(2);
   QstarDT6_1200_Sertac->Draw();
   QstarDT6_1200_Calo->Draw("SAME");
   QstarDT6_1200_PF->Draw("SAME");

   canvas->cd(3);
   QstarDT6_2000_Sertac->Draw();
   QstarDT6_2000_Calo->Draw("SAME");
   QstarDT6_2000_PF->Draw("SAME");

   canvas->cd(4);
   QstarDT6_3500_Sertac->Draw();
   QstarDT6_3500_Calo->Draw("SAME");
   QstarDT6_3500_PF->Draw("SAME");
   canvas->Update();

   TLegend* L = new TLegend(0.11, 0.6, 0.56, 0.85);
   L->SetTextSize(0.03);
   L->AddEntry(QstarDT6_500_Sertac, "Shapes (AN-2011/203)", "f");
   L->AddEntry(QstarDT6_500_Calo, "Calo shapes (\"PF Anal.\")", "l");
   L->AddEntry(QstarDT6_500_PF, "PF shapes", "l");
   L->Draw();



   cout << "Finish" << endl;



   

   if (Res == "Qstar"){
     canvas->SaveAs("Qstar_Shapes.png");
     QstarDT6_500->SetTitle("M(Q*) = 500 GeV; M_{12}/ 500;");
     QstarDT6_700->SetTitle("M(Q*) = 700 GeV; M_{12}/ 700;");
     QstarDT6_1200->SetTitle("M(Q*) = 1200 GeV; M_{12}/ 1200;");
     QstarDT6_2000->SetTitle("M(Q*) = 2000 GeV; M_{12}/ 2000;");
     QstarDT6_3500->SetTitle("M(Q*) = 3500 GeV; M_{12}/ 3500;");
   }
   else if( Res == "RSgg"){
     canvas->SaveAs("RSgg_Shapes.png");
     QstarDT6_500->SetTitle("M(RSgg) = 500 GeV; M_{12}/ 500;");
     QstarDT6_700->SetTitle("M(RSgg) = 700 GeV; M_{12}/ 700;");
     QstarDT6_1200->SetTitle("M(RSgg) = 1200 GeV; M_{12}/ 1200;");
     QstarDT6_2000->SetTitle("M(RSgg) = 2000 GeV; M_{12}/ 2000;");
     QstarDT6_3500->SetTitle("M(RSgg) = 3500 GeV; M_{12}/ 3500;");
   } 
   else if( Res == "RSqq"){
     canvas->SaveAs("RSqq_Shapes.png");
     QstarDT6_500->SetTitle("M(RSqq) = 500 GeV; M_{12}/ 500;");
     QstarDT6_700->SetTitle("M(RSqq) = 700 GeV; M_{12}/ 700;");
     QstarDT6_1200->SetTitle("M(RSqq) = 1200 GeV; M_{12}/ 1200;");
     QstarDT6_2000->SetTitle("M(RSqq) = 2000 GeV; M_{12}/ 2000;");
     QstarDT6_3500->SetTitle("M(RSqq) = 3500 GeV; M_{12}/ 3500;");
   }


   TCanvas* canvas = new TCanvas("canvasFat", "", 1000, 1000);
   canvasFat->Divide(2,2);
   /*
   canvasFat->cd(1);
   QstarDT6_500->Draw();
   QstarDT6_500_Sertac->Draw("SAME");
   QstarDT6_500->Draw("SAME");
   */
   canvasFat->cd(1);
   QstarDT6_700->Draw();
   QstarDT6_700_Sertac->Draw("SAME");
   QstarDT6_700->Draw("SAME");


   canvasFat->cd(2);
   QstarDT6_1200->Draw();
   QstarDT6_1200_Sertac->Draw("SAME");
   QstarDT6_1200->Draw("SAME");


   canvasFat->cd(3);
   QstarDT6_2000->Draw();
   QstarDT6_2000_Sertac->Draw("SAME");
   QstarDT6_2000->Draw("SAME");


   canvasFat->cd(4);
   QstarDT6_3500->Draw();
   QstarDT6_3500_Sertac->Draw("SAME");
   QstarDT6_3500->Draw("SAME");

   canvasFat->Update();

   TLegend* L = new TLegend(0.11, 0.6, 0.56, 0.85);
   L->SetTextSize(0.03);
   L->AddEntry(QstarDT6_500_Sertac, "Calo jet", "f");
   L->AddEntry(QstarDT6_500, "Fat jet shapes", "l");
   L->Draw();



   cout << "Finish" << endl;


 if (Res == "Qstar"){
   canvasFat->SaveAs("Qstar_ShapesFat.png"); 
 }
 else if( Res == "RSgg"){
   canvasFat->SaveAs("RSgg_ShapesFat.png");
 } 
 else if( Res == "RSqq"){
   canvasFat->SaveAs("RSqq_ShapesFat.png");
    
 }
   







}


