cms35( float xmin, float ymin, float xmax, float ymax, const char* addText) {
  
  TPaveText* pave = new TPaveText(xmin,ymin,xmax,ymax, "NDC");
  pave->SetFillStyle(0);
  pave->SetLineColor(0);
  pave->SetLineWidth(0);
  pave->SetShadowColor(0);

  pave->AddText("CMS Preliminary");
  pave->AddText("36 pb-1");
  pave->AddText(addText);
  
  pave->Draw();
}
