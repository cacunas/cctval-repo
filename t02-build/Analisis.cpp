{
    gROOT->Reset();

    TFile* file = TFile::Open("LYSO.root");

    TCanvas* can = new TCanvas("can", "Energy & Time Histograms");
    can->Divide(2,1);

    TH1D* h1 = file->Get("Energy");
    TH1D* h2 = file->Get("Time");

    can->cd(1);

    h1->Draw();

    can->cd(2);

    h2->Draw();

    can->Update();
}
