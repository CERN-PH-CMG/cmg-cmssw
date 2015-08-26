# CMS_lumi
#   Initiated by: Gautier Hamel de Monchenault (Saclay)
#   Translated in Python by: Joshua Hardenbrook (Princeton)
#   Updated by:   Dinko Ferencek (Rutgers)
#

import ROOT

cmsText = "CMS"
cmsTextFont = 61

writeExtraText = True
extraText = "Preliminary"
extraTextFont = 52

lumiTextSize = 0.6
lumiTextOffset = 0.2

cmsTextSize = 0.75
cmsTextOffset = 0.1

relPosX = 0.045
relPosY = 0.035
relExtraDY = 1.2

extraOverCmsTextSize = 0.76

drawLogo = False

def CMS_lumi(pad, lumiText, iPosX):
    outOfFrame = False
    if(iPosX/10 == 0):
        outOfFrame = True

    alignY_ = 3
    alignX_ = 2
    if(iPosX/10 == 0):
        alignX_ = 1
    if(iPosX == 0):
        alignY_ = 1
    if(iPosX/10 == 1):
        alignX_ = 1
    if(iPosX/10 == 2):
        alignX_ = 2
    if(iPosX/10 == 3):
        alignX_ = 3
    align_ = 10*alignX_ + alignY_

    H = pad.GetWh()
    W = pad.GetWw()
    l = pad.GetLeftMargin()
    t = pad.GetTopMargin()
    r = pad.GetRightMargin()
    b = pad.GetBottomMargin()

    pad.cd()


    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(ROOT.kBlack)

    extraTextSize = extraOverCmsTextSize*cmsTextSize

    latex.SetTextFont(42)
    latex.SetTextAlign(31)
    latex.SetTextSize(lumiTextSize*t)

    latex.DrawLatex(1-r, 1-t+lumiTextOffset*t, lumiText)

    if(outOfFrame):
        latex.SetTextFont(cmsTextFont)
        latex.SetTextAlign(11)
        latex.SetTextSize(cmsTextSize*t)
        latex.DrawLatex(l, 1-t+lumiTextOffset*t, cmsText)

    pad.cd()

    posX_ = 0
    if(iPosX % 10 <= 1):
        posX_ = l + relPosX*(1-l-r)
    elif(iPosX % 10 == 2):
        posX_ = l + 0.5*(1-l-r)
    elif(iPosX % 10 == 3):
        posX_ = 1-r - relPosX*(1-l-r)

    posY_ = 1-t - relPosY*(1-t-b)

    if not outOfFrame:
        if drawLogo:
            posX_ = l + 0.045*(1-l-r)*W/H
            posY_ = 1-t - 0.045*(1-t-b)
            xl_0 = posX_
            yl_0 = posY_ - 0.15
            xl_1 = posX_ + 0.15*H/W
            yl_1 = posY_
            CMS_logo = ROOT.TASImage("CMS-BW-label.png")
            pad_logo = ROOT.TPad("logo", "logo", xl_0, yl_0, xl_1, yl_1)
            pad_logo.Draw()
            pad_logo.cd()
            CMS_logo.Draw("X")
            pad_logo.Modified()
            pad.cd()
        else:
            latex.SetTextFont(cmsTextFont)
            latex.SetTextSize(cmsTextSize*t)
            latex.SetTextAlign(align_)
            latex.DrawLatex(posX_, posY_, cmsText)
            if(writeExtraText):
                latex.SetTextFont(extraTextFont)
                latex.SetTextAlign(align_)
                latex.SetTextSize(extraTextSize*t)
                latex.DrawLatex(posX_, posY_ - relExtraDY*cmsTextSize*t, extraText)
    elif(writeExtraText):
        if(iPosX == 0):
            posX_ = l + relPosX*(1-l-r)
            posY_ = 1-t+lumiTextOffset*t

        latex.SetTextFont(extraTextFont)
        latex.SetTextSize(extraTextSize*t)
        latex.SetTextAlign(align_)
        latex.DrawLatex(posX_+0.05, posY_, extraText)

    pad.Update()
