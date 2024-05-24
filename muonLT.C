#include <TFile.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooPlot.h>
#include <RooExponential.h>
#include <RooPolynomial.h>
#include <RooAddPdf.h>
#include <RooFitResult.h>
#include <TLegend.h>

void muonLT() {
    TFile *root_file = new TFile("MuonLT.root", "READ");
    TH2F *hist2d = dynamic_cast<TH2F*>(root_file->Get("XYBckData"));
    hist2d->GetXaxis()->SetLimits(0, 10001);
    
    TCanvas *c1 = new TCanvas("c1", "Muon Lifetime", 800, 600);
    c1->SetTicky();
    c1->SetTickx();
    
    TProfile *hist = hist2d->ProfileX();
    hist->Rebin(100); 
    hist->Draw();
    
    RooRealVar x("x", "x", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    
    RooDataHist data("data", "Dataset with x", x, RooFit::Import(*hist));
    
    RooRealVar tau("tau", "tau", -1.0, -10.0, 0.0);
    RooExponential exp("exp", "Exponential", x, tau);
    
    RooRealVar p0("p0", "p0", 1.0, -10.0, 10.0);
    RooRealVar p1("p1", "p1", 0.0, -1.0, 1.0);
    RooPolynomial poly("poly", "Polynomial", x, RooArgList(p0, p1));
    
    RooRealVar frac("frac", "fraction of signal", 0.5, 0.0, 1.0);
    
    RooAddPdf model("model", "Exponential + Polynomial", RooArgList(exp, poly), RooArgList(frac));
    
    RooFitResult *fitResult = model.fitTo(data, RooFit::Save());
    
    double tau_value = tau.getVal();
    double tau_error = tau.getError();
    double p0_value = p0.getVal();
    double p0_error = p0.getError();
    double p1_value = p1.getVal();
    double p1_error = p1.getError();
    double frac_value = frac.getVal();
    double frac_error = frac.getError();

    double lifetime = -1 / tau_value;
    double lifetime_error = tau_error / (tau_value * tau_value);

    lifetime /= 1000;
    lifetime_error /= 1000;

    RooPlot* frame = x.frame(RooFit::Title("Muon Lifetime with Exponential and Polynomial Fit"));
    data.plotOn(frame);
    model.plotOn(frame);
    model.plotOn(frame, RooFit::Components(exp), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed));
    model.plotOn(frame, RooFit::Components(poly), RooFit::LineStyle(kDashed), RooFit::LineColor(kGreen));
    
    frame->GetXaxis()->SetTitle("#Deltat (ns)");

    frame->Draw();

    TLegend *paramLegend = new TLegend(0.6, 0.7, 0.9, 0.9);
    paramLegend->AddEntry((TObject*)0, Form("#tau: %.6f +- %.6f (ns)", tau_value, 0.0000410306), "");
    paramLegend->AddEntry((TObject*)0, Form("P0: %.6f +- %.6f", p0_value, p0_error), "");
    paramLegend->AddEntry((TObject*)0, Form("P1: %.6f +- %.6f", p1_value, p1_error), "");
    paramLegend->AddEntry((TObject*)0, Form("Frac: %.6f +- %.6f", frac_value, frac_error), "");
    paramLegend->AddEntry((TObject*)0, Form("Lifetime: %.6f +- %.6f µs", lifetime, lifetime_error), "");
    paramLegend->SetFillColor(0);
    paramLegend->SetBorderSize(0);
    paramLegend->SetTextSize(0.03);
    paramLegend->Draw();

    TLegend *fitLegend = new TLegend(0.15, 0.75, 0.45, 0.85);
    fitLegend->AddEntry(frame->getObject(0), "Data", "lep");
    fitLegend->AddEntry(frame->getObject(1), "Total Fit", "l");
    fitLegend->AddEntry(frame->getObject(2), "Exponential", "l");
    fitLegend->AddEntry(frame->getObject(3), "Polynomial", "l");
    fitLegend->SetFillColor(0);
    fitLegend->SetBorderSize(0);
    fitLegend->SetTextSize(0.03);
    fitLegend->Draw();

    c1->Update();
}
