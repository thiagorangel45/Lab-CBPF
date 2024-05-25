import ROOT

root_file = ROOT.TFile("MuonVelocity.root", "READ")
c1 = ROOT.TCanvas("c1", "Muon Velocity", 800, 600)
c1.SetTickx()
c1.SetTicky()

hist2d = root_file.Get("XYBckData")
hist2d.GetXaxis().SetLimits(0, 50)
hist2d.GetXaxis().SetRangeUser(0, 20)

hist = hist2d.ProfileX()
hist.Rebin(100)
hist.Draw()

x = ROOT.RooRealVar("x", "x", hist.GetXaxis().GetXmin(), hist.GetXaxis().GetXmax())
data = ROOT.RooDataHist("data", "Dataset with x", ROOT.RooArgList(x), ROOT.RooFit.Import(hist))

mean = ROOT.RooRealVar("mean", "mean", hist.GetMean(), hist.GetMean() - hist.GetRMS(), hist.GetMean() + hist.GetRMS())
sigma = ROOT.RooRealVar("sigma", "sigma", hist.GetRMS(), 0.1, hist.GetRMS() * 2)
alpha = ROOT.RooRealVar("alpha", "alpha", -1.5, 0.1, 10)
n = ROOT.RooRealVar("n", "n", 2, 0.1, 10)
cb = ROOT.RooCBShape("cb", "Crystal Ball", x, mean, sigma, alpha, n)

lambda_exp = ROOT.RooRealVar("lambda", "slope", -0.0001, -10., 0.)
exp = ROOT.RooExponential("exp", "Exponential", x, lambda_exp)

frac = ROOT.RooRealVar("frac", "fraction of signal", 0.2, 0.0, 1.0)
model = ROOT.RooAddPdf("model", "Crystal Ball + Exponential", ROOT.RooArgList(cb, exp), ROOT.RooArgList(frac))

fitResult = model.fitTo(data, ROOT.RooFit.Save())

frame = x.frame(ROOT.RooFit.Title(""))
data.plotOn(frame)
model.plotOn(frame)
model.plotOn(frame, ROOT.RooFit.Components("exp"), ROOT.RooFit.LineStyle(ROOT.kDashed), ROOT.RooFit.LineColor(ROOT.kRed))
model.plotOn(frame, ROOT.RooFit.Components("cb"), ROOT.RooFit.LineStyle(ROOT.kDashed), ROOT.RooFit.LineColor(ROOT.kGreen))

frame.GetXaxis().SetTitle("#Deltat (ns)")
frame.Draw()