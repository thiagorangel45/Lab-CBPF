import ROOT

root_file = ROOT.TFile("MuonLT.root", "READ")
c1 = ROOT.TCanvas()

hist2d = root_file.Get("XYBckData")
hist2d.GetXaxis().SetLimits(0, 10000)

hist = hist2d.ProfileX()
hist.Rebin(100)
hist.Draw()

x = ROOT.RooRealVar("x", "x", hist.GetXaxis().GetXmin(), hist.GetXaxis().GetXmax())
data = ROOT.RooDataHist("data", "data", ROOT.RooArgList(x), ROOT.RooFit.Import(hist))

tau = ROOT.RooRealVar("tau", "tau", -1.0, -10.0, 0.0)
exp = ROOT.RooExponential("exp", "Exponential", x, tau)

p0 = ROOT.RooRealVar("p0", "p0", 1.0, -10.0, 10.0)
p1 = ROOT.RooRealVar("p1", "p1", 0.0, -1.0, 1.0)
poly = ROOT.RooPolynomial("poly", "Polynomial", x, ROOT.RooArgList(p0, p1))

frac = ROOT.RooRealVar("frac", "fraction of signal", 0.5, 0.0, 1.0)
model = ROOT.RooAddPdf("model", "Exponential + Polynomial", ROOT.RooArgList(exp, poly), ROOT.RooArgList(frac))

fitResult = model.fitTo(data, ROOT.RooFit.Save())

frame = x.frame(ROOT.RooFit.Title("Muon Lifetime with Exponential and Polynomial Fit"))
data.plotOn(frame)
model.plotOn(frame)
model.plotOn(frame, ROOT.RooFit.Components("exp"), ROOT.RooFit.LineStyle(ROOT.kDashed), ROOT.RooFit.LineColor(ROOT.kRed))
model.plotOn(frame, ROOT.RooFit.Components("poly"), ROOT.RooFit.LineStyle(ROOT.kDashed), ROOT.RooFit.LineColor(ROOT.kGreen))

frame.GetXaxis().SetTitle("#Deltat (ns)")
frame.Draw()
