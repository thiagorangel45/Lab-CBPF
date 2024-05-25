#include <TFile.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooPlot.h>
#include <RooCBShape.h>
#include <RooExponential.h>
#include <RooAddPdf.h>
#include <RooFitResult.h>
#include <TLegend.h>

void muonVel() {

    TFile *root_file = new TFile("MuonVelocity.root", "READ");
    TH2F *hist2d = dynamic_cast<TH2F*>(root_file->Get("XYBckData"));
    hist2d->GetXaxis()->SetLimits(0, 50);
    hist2d->GetXaxis()->SetRangeUser(0, 20);
    TCanvas *c1 = new TCanvas("c1", "Muon Velocity", 800, 600);
    c1->SetTickx();
    c1->SetTicky();
    
    TProfile *hist = hist2d->ProfileX();
    hist->Rebin(100); 
    hist->Draw();
    
    RooRealVar x("x", "x", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    
    RooDataHist data("data", "Dataset with x", x, RooFit::Import(*hist));
    
    RooRealVar mean("mean", "mean", hist->GetMean(), hist->GetMean() - hist->GetRMS(), hist->GetMean() + hist->GetRMS());
    RooRealVar sigma("sigma", "sigma", hist->GetRMS(), 0.1, hist->GetRMS() * 2);
    RooRealVar alpha("alpha", "alpha", -1.5, 0.1, 10);
    RooRealVar n("n", "n", 2, 0.1, 10);
    RooCBShape cb("cb", "Crystal Ball", x, mean, sigma, alpha, n);
    
    RooRealVar lambda("lambda", "slope", -0.0001, -10., 0.);
    RooExponential exp("exp", "Exponential", x, lambda);
    
    RooRealVar frac("frac", "fraction of signal", 0.2, 0., 1.);
    
    RooAddPdf model("model", "Crystal Ball + Exponential", RooArgList(cb, exp), RooArgList(frac));
    
    RooFitResult *fitResult = model.fitTo(data, RooFit::Save());
    
    double mean_value = mean.getVal();
    double mean_error = mean.getError();
    double sigma_value = sigma.getVal();
    double sigma_error = sigma.getError();
    double alpha_value = alpha.getVal();
    double alpha_error = alpha.getError();
    double n_value = n.getVal();
    double n_error = n.getError();
    double lambda_value = lambda.getVal();
    double lambda_error = lambda.getError();
    double frac_value = frac.getVal();
    double frac_error = frac.getError();

    RooPlot* frame = x.frame(RooFit::Title(""));
    data.plotOn(frame);
    model.plotOn(frame);
    model.plotOn(frame, RooFit::Components(exp), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed));
    model.plotOn(frame, RooFit::Components(cb), RooFit::LineStyle(kDashed), RooFit::LineColor(kGreen));

    TGraph *graph = new TGraph();
    graph->SetLineColor(1); 
    graph->SetLineWidth(2); 
    graph->SetLineStyle(1); 
    graph->SetMarkerColor(0); 
    graph->SetMarkerStyle(0); 
    graph->SetMarkerSize(0); 
    frame->GetXaxis()->SetTitle("#Deltat (ns)");
    frame->SetTitle("");
    frame->Draw();

    TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend->AddEntry(graph, Form("Mean: %.3f +- %.3f (ns)", mean_value, mean_error), "");
    legend->AddEntry(graph, Form("Sigma: %.3f +- %.3f", sigma_value, sigma_error), "");
    legend->AddEntry(graph, Form("Alpha: %.3f +- %.3f", alpha_value, alpha_error), "");
    legend->AddEntry(graph, Form("n: %.3f +- %.3f", n_value, n_error), "");
    legend->AddEntry(graph, Form("P0: %.3f +- %.3f", lambda_value, lambda_error), "");
    legend->SetFillColor(0);
    legend->SetBorderSize(0);
    legend->SetTextSize(0.03);
    legend->Draw("SAME");

    TLegend *fitLegend = new TLegend(0.15, 0.75, 0.45, 0.85);
    fitLegend->AddEntry(frame->getObject(0), "Data", "p");
    fitLegend->AddEntry(frame->getObject(1), "Total Fit", "l");
    fitLegend->AddEntry(frame->getObject(2), "Exponential", "l");
    fitLegend->AddEntry(frame->getObject(3), "Crystal Ball", "l");
    fitLegend->SetBorderSize(0);
    fitLegend->Draw("SAME");
}
