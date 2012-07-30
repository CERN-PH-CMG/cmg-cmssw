void qqtt_forpaper(int bb=0) {

// first define histograms 
  if (bb==0)
    {
   TH1F *constrained_massQQ = new TH1F("constrained_massQQ","constrained_massQQ",80,40,200);
   constrained_massQQ->SetBinContent(1,4.168056);
   constrained_massQQ->SetBinContent(2,0.521007);
   constrained_massQQ->SetBinContent(3,0.521007);
   constrained_massQQ->SetBinContent(5,0.06512588);
   constrained_massQQ->SetBinContent(6,0.1953776);
   constrained_massQQ->SetBinContent(7,0.1953776);
   constrained_massQQ->SetBinContent(8,0.06512588);
   constrained_massQQ->SetBinContent(9,0.1302518);
   constrained_massQQ->SetBinContent(10,0.2605035);
   constrained_massQQ->SetBinContent(11,0.521007);
   constrained_massQQ->SetBinContent(12,0.7815105);
   constrained_massQQ->SetBinContent(13,3.516797);
   constrained_massQQ->SetBinContent(14,3.647049);
   constrained_massQQ->SetBinContent(15,2.21428);
   constrained_massQQ->SetBinContent(16,3.126042);
   constrained_massQQ->SetBinContent(17,3.647049);
   constrained_massQQ->SetBinContent(18,4.949566);
   constrained_massQQ->SetBinContent(19,6.382336);
   constrained_massQQ->SetBinContent(20,7.42435);
   constrained_massQQ->SetBinContent(21,9.009079);
   constrained_massQQ->SetBinContent(22,4.537103);
   constrained_massQQ->SetBinContent(23,1.193974);
   constrained_massQQ->SetBinContent(24,2.670161);
   constrained_massQQ->SetBinContent(25,1.042014);
   constrained_massQQ->SetBinContent(26,1.302518);
   constrained_massQQ->SetBinContent(27,1.693273);
   constrained_massQQ->SetBinContent(28,4.949566);
   constrained_massQQ->SetBinContent(29,4.168056);
   constrained_massQQ->SetBinContent(30,4.949566);
   constrained_massQQ->SetBinContent(31,7.815105);
   constrained_massQQ->SetBinContent(32,8.336112);
   constrained_massQQ->SetBinContent(33,2.605035);
   constrained_massQQ->SetBinContent(34,0.2605035);
   constrained_massQQ->SetBinContent(35,4.168056);
   constrained_massQQ->SetBinContent(36,8.336112);
   constrained_massQQ->SetBinContent(37,7.033595);
   constrained_massQQ->SetBinContent(38,1.042014);
   constrained_massQQ->SetBinContent(39,1.042014);
   constrained_massQQ->SetBinContent(40,7.294098);
   constrained_massQQ->SetBinContent(41,6.903343);
   constrained_massQQ->SetBinContent(42,2.21428);
   constrained_massQQ->SetBinContent(43,3.668758);
   constrained_massQQ->SetBinContent(44,6.143541);
   constrained_massQQ->SetBinContent(45,4.10293);
   constrained_massQQ->SetBinContent(46,0.7598019);
   constrained_massQQ->SetBinContent(47,0.694676);
   constrained_massQQ->SetBinContent(48,0.7815105);
   constrained_massQQ->SetBinContent(49,4.25489);
   constrained_massQQ->SetBinContent(50,6.121832);
   constrained_massQQ->SetBinContent(51,5.340322);
   constrained_massQQ->SetBinContent(52,3.386545);
   constrained_massQQ->SetBinContent(53,0.6512588);
   constrained_massQQ->SetBinContent(54,0.521007);
   constrained_massQQ->SetBinContent(55,2.084028);
   constrained_massQQ->SetBinContent(56,1.042014);
   constrained_massQQ->SetBinContent(57,0.2605035);
   constrained_massQQ->SetBinContent(58,0.1302518);
   constrained_massQQ->SetBinContent(81,8.336112);

   TH1F *constrained_massWW = new TH1F("constrained_massWW","constrained_massWW",80,40,200);
   constrained_massWW->SetBinContent(0,10.72081);
   constrained_massWW->SetBinContent(2,4.913705);
   constrained_massWW->SetBinContent(3,7.928934);
   constrained_massWW->SetBinContent(4,6.142132);
   constrained_massWW->SetBinContent(5,7.147208);
   constrained_massWW->SetBinContent(6,7.370558);
   constrained_massWW->SetBinContent(7,5.248731);
   constrained_massWW->SetBinContent(8,5.025381);
   constrained_massWW->SetBinContent(9,5.807106);
   constrained_massWW->SetBinContent(10,7.147208);
   constrained_massWW->SetBinContent(11,17.64467);
   constrained_massWW->SetBinContent(12,34.91709);
   constrained_massWW->SetBinContent(13,40.84828);
   constrained_massWW->SetBinContent(14,42.61027);
   constrained_massWW->SetBinContent(15,39.58263);
   constrained_massWW->SetBinContent(16,30.59899);
   constrained_massWW->SetBinContent(17,36.96447);
   constrained_massWW->SetBinContent(18,55.05584);
   constrained_massWW->SetBinContent(19,57.84772);
   constrained_massWW->SetBinContent(20,49.58376);
   constrained_massWW->SetBinContent(21,50.36549);
   constrained_massWW->SetBinContent(22,43.66498);
   constrained_massWW->SetBinContent(23,34.84264);
   constrained_massWW->SetBinContent(24,40.76143);
   constrained_massWW->SetBinContent(25,43.66498);
   constrained_massWW->SetBinContent(26,47.90863);
   constrained_massWW->SetBinContent(27,49.91879);
   constrained_massWW->SetBinContent(28,43.32995);
   constrained_massWW->SetBinContent(29,52.26396);
   constrained_massWW->SetBinContent(30,63.87818);
   constrained_massWW->SetBinContent(31,56.73097);
   constrained_massWW->SetBinContent(32,43.5533);
   constrained_massWW->SetBinContent(33,41.09645);
   constrained_massWW->SetBinContent(34,48.61591);
   constrained_massWW->SetBinContent(35,50.99831);
   constrained_massWW->SetBinContent(36,47.94586);
   constrained_massWW->SetBinContent(37,42.88325);
   constrained_massWW->SetBinContent(38,46.45686);
   constrained_massWW->SetBinContent(39,72.58882);
   constrained_massWW->SetBinContent(40,87.32993);
   constrained_massWW->SetBinContent(41,78.39593);
   constrained_massWW->SetBinContent(42,73.92892);
   constrained_massWW->SetBinContent(43,77.27917);
   constrained_massWW->SetBinContent(44,71.69543);
   constrained_massWW->SetBinContent(45,64.32487);
   constrained_massWW->SetBinContent(46,68.56852);
   constrained_massWW->SetBinContent(47,68.12183);
   constrained_massWW->SetBinContent(48,61.19797);
   constrained_massWW->SetBinContent(49,55.61421);
   constrained_massWW->SetBinContent(50,63.87817);
   constrained_massWW->SetBinContent(51,73.92892);
   constrained_massWW->SetBinContent(52,76.38578);
   constrained_massWW->SetBinContent(53,71.49068);
   constrained_massWW->SetBinContent(54,41.18951);
   constrained_massWW->SetBinContent(55,9.622678);
   constrained_massWW->SetBinContent(56,0.1675114);
   constrained_massWW->SetBinContent(57,0.4467005);
   constrained_massWW->SetBinContent(59,0.05583756);
   constrained_massWW->SetBinContent(60,0.1116751);
   constrained_massWW->SetBinContent(61,0.05583756);
   constrained_massWW->SetBinContent(81,35.73604);

   TH1F *constrained_massZZ = new TH1F("constrained_massZZ","constrained_massZZ",80,40,200);
   constrained_massZZ->SetBinContent(0,6.559476);
   constrained_massZZ->SetBinContent(1,2.915323);
   constrained_massZZ->SetBinContent(2,2.733115);
   constrained_massZZ->SetBinContent(3,3.735257);
   constrained_massZZ->SetBinContent(4,4.145224);
   constrained_massZZ->SetBinContent(5,3.735257);
   constrained_massZZ->SetBinContent(6,3.871913);
   constrained_massZZ->SetBinContent(7,4.782951);
   constrained_massZZ->SetBinContent(8,5.192919);
   constrained_massZZ->SetBinContent(9,5.785094);
   constrained_massZZ->SetBinContent(10,8.928175);
   constrained_massZZ->SetBinContent(11,11.16022);
   constrained_massZZ->SetBinContent(12,10.02142);
   constrained_massZZ->SetBinContent(13,11.88905);
   constrained_massZZ->SetBinContent(14,13.48337);
   constrained_massZZ->SetBinContent(15,14.07554);
   constrained_massZZ->SetBinContent(16,16.17093);
   constrained_massZZ->SetBinContent(17,15.6319);
   constrained_massZZ->SetBinContent(18,19.53418);
   constrained_massZZ->SetBinContent(19,28.52309);
   constrained_massZZ->SetBinContent(20,37.46646);
   constrained_massZZ->SetBinContent(21,52.29363);
   constrained_massZZ->SetBinContent(22,72.1087);
   constrained_massZZ->SetBinContent(23,96.93442);
   constrained_massZZ->SetBinContent(24,140.9376);
   constrained_massZZ->SetBinContent(25,190.9084);
   constrained_massZZ->SetBinContent(26,224.2527);
   constrained_massZZ->SetBinContent(27,232.4976);
   constrained_massZZ->SetBinContent(28,206.214);
   constrained_massZZ->SetBinContent(29,171.7309);
   constrained_massZZ->SetBinContent(30,154.0566);
   constrained_massZZ->SetBinContent(31,134.8336);
   constrained_massZZ->SetBinContent(32,112.0576);
   constrained_massZZ->SetBinContent(33,97.48105);
   constrained_massZZ->SetBinContent(34,79.48811);
   constrained_massZZ->SetBinContent(35,66.41475);
   constrained_massZZ->SetBinContent(36,62.31507);
   constrained_massZZ->SetBinContent(37,51.92921);
   constrained_massZZ->SetBinContent(38,43.77541);
   constrained_massZZ->SetBinContent(39,38.94689);
   constrained_massZZ->SetBinContent(40,35.66714);
   constrained_massZZ->SetBinContent(41,33.70842);
   constrained_massZZ->SetBinContent(42,28.6066);
   constrained_massZZ->SetBinContent(43,28.10552);
   constrained_massZZ->SetBinContent(44,27.5589);
   constrained_massZZ->SetBinContent(45,27.65001);
   constrained_massZZ->SetBinContent(46,29.01657);
   constrained_massZZ->SetBinContent(47,22.6393);
   constrained_massZZ->SetBinContent(48,22.18378);
   constrained_massZZ->SetBinContent(49,24.68913);
   constrained_massZZ->SetBinContent(50,18.31187);
   constrained_massZZ->SetBinContent(51,14.43995);
   constrained_massZZ->SetBinContent(52,13.66557);
   constrained_massZZ->SetBinContent(53,10.38584);
   constrained_massZZ->SetBinContent(54,6.377269);
   constrained_massZZ->SetBinContent(55,2.186492);
   constrained_massZZ->SetBinContent(56,0.09110388);
   constrained_massZZ->SetBinContent(57,0.1822077);
   constrained_massZZ->SetBinContent(59,0.02277596);
   constrained_massZZ->SetBinContent(60,0.04555191);
   constrained_massZZ->SetBinContent(61,0.02277596);
   constrained_massZZ->SetBinContent(81,13.11895);

   TH1F *constrained_massHZothers = new TH1F("constrained_massHZothers","constrained_massHZothers",80,40,200);
   constrained_massHZothers->SetBinContent(0,1.030928);
   constrained_massHZothers->SetBinContent(1,1.030928);
   constrained_massHZothers->SetBinContent(3,1.030928);
   constrained_massHZothers->SetBinContent(6,1.030928);
   constrained_massHZothers->SetBinContent(7,1.030928);
   constrained_massHZothers->SetBinContent(10,1.030928);
   constrained_massHZothers->SetBinContent(12,1.030928);
   constrained_massHZothers->SetBinContent(13,3.092783);
   constrained_massHZothers->SetBinContent(14,2.061856);
   constrained_massHZothers->SetBinContent(15,3.092783);
   constrained_massHZothers->SetBinContent(16,3.092783);
   constrained_massHZothers->SetBinContent(17,2.061856);
   constrained_massHZothers->SetBinContent(18,2.061856);
   constrained_massHZothers->SetBinContent(19,1.030928);
   constrained_massHZothers->SetBinContent(20,1.030928);
   constrained_massHZothers->SetBinContent(21,2.061856);
   constrained_massHZothers->SetBinContent(22,2.061856);
   constrained_massHZothers->SetBinContent(24,3.092783);
   constrained_massHZothers->SetBinContent(26,1.030928);
   constrained_massHZothers->SetBinContent(28,2.061856);
   constrained_massHZothers->SetBinContent(29,2.061856);
   constrained_massHZothers->SetBinContent(31,2.061856);
   constrained_massHZothers->SetBinContent(32,4.123711);
   constrained_massHZothers->SetBinContent(33,4.123711);
   constrained_massHZothers->SetBinContent(34,5.154639);
   constrained_massHZothers->SetBinContent(35,3.092783);
   constrained_massHZothers->SetBinContent(36,4.123711);
   constrained_massHZothers->SetBinContent(37,10.30928);
   constrained_massHZothers->SetBinContent(38,9.27835);
   constrained_massHZothers->SetBinContent(39,11.34021);
   constrained_massHZothers->SetBinContent(40,13.40206);
   constrained_massHZothers->SetBinContent(41,12.37113);
   constrained_massHZothers->SetBinContent(42,7.216494);
   constrained_massHZothers->SetBinContent(43,17.52577);
   constrained_massHZothers->SetBinContent(44,13.40206);
   constrained_massHZothers->SetBinContent(45,10.30928);
   constrained_massHZothers->SetBinContent(46,7.216494);
   constrained_massHZothers->SetBinContent(47,4.123711);
   constrained_massHZothers->SetBinContent(48,7.216494);
   constrained_massHZothers->SetBinContent(49,3.092783);
   constrained_massHZothers->SetBinContent(50,3.092783);
   constrained_massHZothers->SetBinContent(51,1.030928);
   constrained_massHZothers->SetBinContent(52,2.061856);
   constrained_massHZothers->SetBinContent(53,2.061856);
   constrained_massHZothers->SetBinContent(54,2.061856);
   constrained_massHZothers->SetBinContent(81,4.123711);
   TH1F *constrained_massHZqqtt = new TH1F("constrained_massHZqqtt","constrained_massHZqqtt",80,40,200);
   constrained_massHZqqtt->SetBinContent(26,3.092783);
   constrained_massHZqqtt->SetBinContent(28,1.030928);
   constrained_massHZqqtt->SetBinContent(29,1.030928);
   constrained_massHZqqtt->SetBinContent(32,2.061856);
   constrained_massHZqqtt->SetBinContent(33,2.061856);
   constrained_massHZqqtt->SetBinContent(34,2.061856);
   constrained_massHZqqtt->SetBinContent(35,4.123711);
   constrained_massHZqqtt->SetBinContent(36,1.030928);
   constrained_massHZqqtt->SetBinContent(37,3.092783);
   constrained_massHZqqtt->SetBinContent(38,10.30928);
   constrained_massHZqqtt->SetBinContent(39,19.58763);
   constrained_massHZqqtt->SetBinContent(40,38.14434);
   constrained_massHZqqtt->SetBinContent(41,62.88665);
   constrained_massHZqqtt->SetBinContent(42,150.5157);
   constrained_massHZqqtt->SetBinContent(43,200.0003);
   constrained_massHZqqtt->SetBinContent(44,132.9899);
   constrained_massHZqqtt->SetBinContent(45,86.59802);
   constrained_massHZqqtt->SetBinContent(46,51.54642);
   constrained_massHZqqtt->SetBinContent(47,35.05155);
   constrained_massHZqqtt->SetBinContent(48,22.68041);
   constrained_massHZqqtt->SetBinContent(49,10.30928);
   constrained_massHZqqtt->SetBinContent(50,10.30928);
   constrained_massHZqqtt->SetBinContent(51,5.154639);
   constrained_massHZqqtt->SetBinContent(52,6.185566);
   constrained_massHZqqtt->SetBinContent(53,5.154639);
   constrained_massHZqqtt->SetBinContent(54,1.030928);
    }
  else
    {
   TH1F *constrained_massQQ = new TH1F("constrained_massQQ","constrained_massQQ",80,40,200);
   TH1F *constrained_massWW = new TH1F("constrained_massWW","constrained_massWW",80,40,200);
   TH1F *constrained_massZZ = new TH1F("constrained_massZZ","constrained_massZZ",80,40,200);
   constrained_massZZ->SetBinContent(0,0.7288306);
   constrained_massZZ->SetBinContent(1,0.7288306);
   constrained_massZZ->SetBinContent(2,0.1935956);
   constrained_massZZ->SetBinContent(3,0.6035629);
   constrained_massZZ->SetBinContent(4,0.6035629);
   constrained_massZZ->SetBinContent(5,0.3074754);
   constrained_massZZ->SetBinContent(6,0.3871913);
   constrained_massZZ->SetBinContent(7,0.1366557);
   constrained_massZZ->SetBinContent(8,0.02277596);
   constrained_massZZ->SetBinContent(9,0.2277596);
   constrained_massZZ->SetBinContent(10,0.3188634);
   constrained_massZZ->SetBinContent(11,0.1024918);
   constrained_massZZ->SetBinContent(12,0.1024918);
   constrained_massZZ->SetBinContent(13,0.7402186);
   constrained_massZZ->SetBinContent(14,1.150186);
   constrained_massZZ->SetBinContent(15,1.093246);
   constrained_massZZ->SetBinContent(16,1.047694);
   constrained_massZZ->SetBinContent(17,1.138798);
   constrained_massZZ->SetBinContent(18,2.049836);
   constrained_massZZ->SetBinContent(19,2.550907);
   constrained_massZZ->SetBinContent(20,2.915323);
   constrained_massZZ->SetBinContent(21,4.190776);
   constrained_massZZ->SetBinContent(22,5.739542);
   constrained_massZZ->SetBinContent(23,10.29473);
   constrained_massZZ->SetBinContent(24,18.08411);
   constrained_massZZ->SetBinContent(25,26.28345);
   constrained_massZZ->SetBinContent(26,27.5589);
   constrained_massZZ->SetBinContent(27,24.37027);
   constrained_massZZ->SetBinContent(28,23.2011);
   constrained_massZZ->SetBinContent(29,17.67414);
   constrained_massZZ->SetBinContent(30,16.32277);
   constrained_massZZ->SetBinContent(31,19.17735);
   constrained_massZZ->SetBinContent(32,16.59608);
   constrained_massZZ->SetBinContent(33,13.71112);
   constrained_massZZ->SetBinContent(34,10.65915);
   constrained_massZZ->SetBinContent(35,8.472656);
   constrained_massZZ->SetBinContent(36,8.06269);
   constrained_massZZ->SetBinContent(37,6.103957);
   constrained_massZZ->SetBinContent(38,3.507498);
   constrained_massZZ->SetBinContent(39,2.733115);
   constrained_massZZ->SetBinContent(40,2.801443);
   constrained_massZZ->SetBinContent(41,2.14094);
   constrained_massZZ->SetBinContent(42,2.459804);
   constrained_massZZ->SetBinContent(43,3.473334);
   constrained_massZZ->SetBinContent(44,2.995039);
   constrained_massZZ->SetBinContent(45,2.300372);
   constrained_massZZ->SetBinContent(46,2.288984);
   constrained_massZZ->SetBinContent(47,2.14094);
   constrained_massZZ->SetBinContent(48,2.015672);
   constrained_massZZ->SetBinContent(49,2.106776);
   constrained_massZZ->SetBinContent(50,2.243432);
   constrained_massZZ->SetBinContent(51,2.118164);
   constrained_massZZ->SetBinContent(52,1.207126);
   constrained_massZZ->SetBinContent(53,0.6377268);
   constrained_massZZ->SetBinContent(54,0.8199345);
   constrained_massZZ->SetBinContent(55,0.2733115);
   constrained_massZZ->SetBinContent(56,0.1138798);
   constrained_massZZ->SetBinContent(58,0.02277596);
   constrained_massZZ->SetBinContent(81,0.7288306);
   TH1F *constrained_massHZothers = new TH1F("constrained_massHZothers","constrained_massHZothers",80,40,200);
   constrained_massHZothers->SetBinContent(6,1.030928);
   constrained_massHZothers->SetBinContent(10,1.030928);
   constrained_massHZothers->SetBinContent(13,1.030928);
   constrained_massHZothers->SetBinContent(31,1.030928);
   constrained_massHZothers->SetBinContent(32,1.030928);
   constrained_massHZothers->SetBinContent(33,1.030928);
   constrained_massHZothers->SetBinContent(36,2.061856);
   constrained_massHZothers->SetBinContent(37,2.061856);
   constrained_massHZothers->SetBinContent(38,2.061856);
   constrained_massHZothers->SetBinContent(39,4.123711);
   constrained_massHZothers->SetBinContent(41,4.123711);
   constrained_massHZothers->SetBinContent(42,1.030928);
   constrained_massHZothers->SetBinContent(43,4.123711);
   constrained_massHZothers->SetBinContent(44,3.092783);
   constrained_massHZothers->SetBinContent(45,3.092783);
   constrained_massHZothers->SetBinContent(46,1.030928);
   constrained_massHZothers->SetBinContent(49,1.030928);
   constrained_massHZothers->SetBinContent(51,1.030928);
   constrained_massHZothers->SetBinContent(52,1.030928);
   constrained_massHZothers->SetBinContent(53,1.030928);
   TH1F *constrained_massHZqqtt = new TH1F("constrained_massHZqqtt","constrained_massHZqqtt",80,40,200);
   constrained_massHZqqtt->SetBinContent(34,1.030928);
   constrained_massHZqqtt->SetBinContent(35,1.030928);
   constrained_massHZqqtt->SetBinContent(38,3.092783);
   constrained_massHZqqtt->SetBinContent(39,1.030928);
   constrained_massHZqqtt->SetBinContent(40,2.061856);
   constrained_massHZqqtt->SetBinContent(41,8.247422);
   constrained_massHZqqtt->SetBinContent(42,18.5567);
   constrained_massHZqqtt->SetBinContent(43,23.71134);
   constrained_massHZqqtt->SetBinContent(44,10.30928);
   constrained_massHZqqtt->SetBinContent(45,12.37113);
   constrained_massHZqqtt->SetBinContent(46,1.030928);
   constrained_massHZqqtt->SetBinContent(47,5.154639);
   constrained_massHZqqtt->SetBinContent(48,3.092783);
   constrained_massHZqqtt->SetBinContent(50,5.154639);
   constrained_massHZqqtt->SetBinContent(51,1.030928);

    }

// then plot them (copied from PJ)

  gStyle->SetTitleX(0.2); // to set the x start position in NDC
  gStyle->SetTitleY(1.0); // to set the y position in NDC [0,1]
  gStyle->SetTitleW(0.3); //to set the title box width
  gStyle->SetTitleH(0.07); //to set the title box height
  gStyle->SetTitleFont(62); //to set the title box height
  gStyle->SetTitleBorderSize(2); //to set the title box height

  TCanvas* c0 = new TCanvas();
  
  constrained_massWW->Add(constrained_massQQ);
  constrained_massZZ->Add(constrained_massWW);
  constrained_massHZothers->Add(constrained_massZZ);
  constrained_massHZqqtt->Add(constrained_massHZothers);

//  TF1* fh = new TF1("fh","[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))+[3]+[4]*x+[5]*x*x+[6]*x*x*x+[0]*[7]*exp(-(x-[8])*(x-[8])/(2*[9]*[9]))",100,150);
//  fh->SetParameters(250,125.,3.,0.,0.,0.,0.,0.54,125.,10.);
////  //  fh->FixParameter(7,0.54);
  //  fh->FixParameter(3,2242.);
  //  fh->FixParameter(4,-97.32);
  //  fh->FixParameter(5,1.186);
  //  fh->FixParameter(6,-4.236E-3);

  //  TF1* fh = new TF1("fh","[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))+[3]+[4]*x+[5]*x*x+[6]*x*x*x",100,150);
  
  
  TF1* fh = new TF1("fh","[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))+[3]+[4]*x+[5]*x*x+[6]*x*x*x",100,150);
  fh->SetParameters(180,125.,2.,0, 0,0,0);
  fh->SetLineColor(1);
  fh->SetLineWidth(4);

  //
  //
  constrained_massHZqqtt->Fit("fh","EL","",105,145);  
  constrained_massHZqqtt->Fit("fh","M","",105,145);  
  //  constrained_massHZqqtt->Fit("fh","M","",105,145);  
  //  constrained_massHZqqtt->Fit("fh","M","",100,150);  
  // constrained_massHZqqtt->Fit("fh","E","",105,145);  
  //  constrained_massHZqqtt->Fit("fh","EL","",102,147);  
  // constrained_massHZqqtt->Fit("fh","EL","",102,147);  
  //  higgs->Fit("fh","EL","",102,147);  
  //  higgs->Fit("fh","E","",102,147);  
  //  higgs->Fit("fh","L","",102,147);  

  constrained_massZZ->SetLineColor(4);
  constrained_massZZ->SetLineWidth(4);
  constrained_massZZ->SetFillColor(4);
  constrained_massZZ->SetFillStyle(3013);

  constrained_massHZqqtt->SetMarkerStyle(22);
  constrained_massHZqqtt->SetMarkerColor(2);
  constrained_massHZqqtt->SetMarkerSize(1);
  constrained_massHZqqtt->SetLineColor(2);
  constrained_massHZqqtt->SetLineWidth(4);
 
  constrained_massWW->SetLineColor(1);
  constrained_massWW->SetLineWidth(4);

  constrained_massQQ->SetLineColor(6);
  constrained_massQQ->SetLineWidth(4);

  if (bb==00)
    {
      constrained_massHZqqtt->SetTitle( "H#rightarrow#tau#tau with Z#rightarrow q#bar{q}" );
    }
  else
    {
      constrained_massHZqqtt->SetTitle( "H#rightarrow#tau#tau with Z#rightarrow b#bar{b}" );
    }

  constrained_massHZqqtt->SetXTitle( "Higgs mass (GeV)" );
  constrained_massHZqqtt->SetYTitle( "Events / 2 GeV" );
  // constrained_massHZqqtt->GetYaxis()->SetTitleOffset(1.4);
  constrained_massHZqqtt->GetYaxis()->SetTitleSize(0.045);
  constrained_massHZqqtt->GetYaxis()->SetLabelSize(0.045);
  constrained_massHZqqtt->GetXaxis()->SetLabelSize(0.045);
  constrained_massHZqqtt->GetXaxis()->SetTitleSize(0.045);
  constrained_massHZqqtt->SetStats(0);

  //hmass_before->SetMaximum(6000);
  constrained_massHZqqtt->SetAxisRange(40,150);
  if (bb==0)
    {
      constrained_massHZqqtt->SetMaximum(400);
    }
  else
    {
      constrained_massHZqqtt->SetMaximum(40);
    }
    
  constrained_massHZqqtt->Draw("");
  //   constrained_massHZqqtt->Draw("sameerro");
  constrained_massQQ->Draw("same");
  constrained_massZZ->Draw("same");
  constrained_massWW->Draw("same");

  TLegend *leg0=new TLegend(0.15,0.60,0.35,0.85);
  leg0->AddEntry( constrained_massHZqqtt, "Signal", "l0");
  leg0->AddEntry( constrained_massZZ, "All backgrounds", "lf");
  leg0->AddEntry( constrained_massZZ, "ZZ", "l");
  leg0->AddEntry( constrained_massWW, "WW", "l");
  leg0->AddEntry( constrained_massQQ, "qqbar", "l");
  leg0->SetTextSize(0.03);
  leg0->Draw();


//   heta->SetLineColor(2);
//   zeta->SetLineColor(1);
//   qeta->Add(zeta);
//   weta->Add(qeta);
//   heta->Add(weta);
//   weta->SetLineColor(4);
//   qeta->SetLineColor(6);
//   heta->Draw();
//   zeta->Draw("same");
//   qeta->Draw("same");
//   weta->Draw("same");


  TPaveText* cmslumi = new TPaveText(0.6, 0.85, 0.85, 0.9, "NDC");
  cmslumi->AddText("LEP3, 500 fb^{-1},  #sqrt{s}=240 GeV");
  cmslumi->SetBorderSize(   0 );
  cmslumi->SetFillStyle(    0 );
  cmslumi->SetTextAlign(   12 );
  cmslumi->SetTextSize ( 0.03 );
  cmslumi->SetTextColor(    1 );
  cmslumi->SetTextFont (   62 );
  cmslumi->Draw("same");

  TPaveText* cmssimu = new TPaveText(0.6, 0.9, 0.85, 0.95, "NDC");
  cmssimu->AddText("CMS Simulation");
  cmssimu->SetBorderSize(   0 );
  cmssimu->SetFillStyle(    0 );
  cmssimu->SetTextAlign(   12 );
  cmssimu->SetTextSize ( 0.05 );
  cmssimu->SetTextColor(    1 );
  cmssimu->SetTextFont (   62 );
  cmssimu->Draw("same");

  gPad->SetGridx();
  gPad->SetGridy();
  if (bb==0)
    {
      gPad->SaveAs("qqtt.png");
      gPad->SaveAs("qqtt.pdf");
    }
  else
    {
      gPad->SaveAs("bbtt.png");
      gPad->SaveAs("bbtt.pdf");
    }

}
// here it ends

