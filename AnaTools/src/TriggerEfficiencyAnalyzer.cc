#include "../interface/TriggerEfficiencyAnalyzer.h"

TriggerEfficiencyAnalyzer::TriggerEfficiencyAnalyzer (const edm::ParameterSet &cfg) :
  Trigger_ (cfg.getParameter<edm::InputTag> ("Trigger")),
  triggers_  (cfg.getParameter<vector<edm::ParameterSet> >("triggers")) 
{

  timer = new TStopwatch();  
  timer->Start();  

  TH1D::SetDefaultSumw2 ();
  
  //include all trigger paths of interest, divided up into one histogram for each element of "TriggerTypes"
  for (uint iType=0; iType<triggers_.size(); iType++) {  
    string typeName = triggers_.at(iType).getParameter<string>("trigType");
    TriggerTypes.push_back(typeName);  
    vector<string> trigs_ = triggers_.at(iType).getParameter<vector<string> >("trigs");
    for (uint iTrig=0; iTrig<trigs_.size(); iTrig++) {  
      TriggerNameMap[typeName].push_back(trigs_.at(iTrig));  
    }    
  }  

  //Set Axis Labels of histograms to names of triggers
  for (std::vector<string>::const_iterator triggerType = TriggerTypes.begin(); triggerType != TriggerTypes.end(); triggerType++) {
    int nTriggers = TriggerNameMap[*triggerType].size();
    const char* histName = (*triggerType).c_str();
    TString effName = histName;  
    effName += "Eff";
    TriggerHistogramMap[*triggerType] = fs->make<TH1D> (histName, histName, nTriggers+2, 0.0, nTriggers+2);
    TriggerHistEffMap  [*triggerType] = fs->make<TH1D> (effName,  effName,  nTriggers+2, 0.0, nTriggers+2);
    TriggerHistogramMap[*triggerType]->GetXaxis()->SetBinLabel (1,"Total Events");
    TriggerHistEffMap  [*triggerType]->GetXaxis()->SetBinLabel (1,"Total Events");
    for(int bin = 0; bin != nTriggers; bin++){
      TriggerHistogramMap[*triggerType]->GetXaxis()->SetBinLabel (bin+2,(TriggerNameMap[*triggerType].at(bin)).c_str());
      TriggerHistEffMap  [*triggerType]->GetXaxis()->SetBinLabel (bin+2,(TriggerNameMap[*triggerType].at(bin)).c_str());
    }
    TriggerHistogramMap[*triggerType]->GetXaxis()->SetBinLabel (nTriggers+2,"OR of All Triggers");
    TriggerHistEffMap  [*triggerType]->GetXaxis()->SetBinLabel (nTriggers+2,"OR of All Triggers");
    effName.ReplaceAll("Eff", " trigger efficiency");  
    TriggerHistEffMap  [*triggerType]->SetTitle(effName);  
  }

}  

TriggerEfficiencyAnalyzer::~TriggerEfficiencyAnalyzer ()
{
   for (std::vector<string>::const_iterator triggerType = TriggerTypes.begin(); triggerType != TriggerTypes.end(); triggerType++) {
     TH1D* h    = TriggerHistogramMap[*triggerType];  
     TH1D* heff = TriggerHistEffMap  [*triggerType];  
     if (!h || !heff) { clog << "ERROR [TriggerEfficiencyAnalyzer]:  could not find histogram." << endl; }  
     double total = h->GetBinContent(1);  // Total number of events.  
     heff->SetBinContent(1, 1.0);         // Efficiency in first bin (no trigger requirement) is defined to be 100%.  
     heff->SetBinError  (1, h->GetBinError(1) / h->GetBinContent(1));  
     for(int bin = 0; bin<h->GetNbinsX()-1; bin++){
       double eff    = h->GetBinContent(bin+2) / total;  
       double effErr = eff * (h->GetBinError(bin+2) / h->GetBinContent(bin+2));  // assign the relative error in the efficiency as the relative error in the raw yield  
       if (h->GetBinContent(bin+2)==0) effErr = 0;  // protect against division by 0  
       heff->SetBinContent(bin+2, eff);  
       heff->SetBinError  (bin+2, effErr);  
     }
     heff->SetMinimum(0);
     heff->SetMaximum(1.5);
   }

   timer->Print();  
   clog << endl;  
   clog << "Successfully completed TriggerEfficiencyAnalyzer." << endl;  

}

void
TriggerEfficiencyAnalyzer::analyze (const edm::Event &event, const edm::EventSetup &setup)
{

  event.getByLabel (Trigger_ , TriggerCollection);

  //initialize maps
  for(std::vector<string>::const_iterator triggerType = TriggerTypes.begin(); triggerType != TriggerTypes.end(); triggerType++){
    InclusiveORMap[*triggerType] = false;
  }
  
  int nEvts = 0;  
  //fill denominator bin for all trigger types that match generated final state
  for(std::vector<string>::const_iterator triggerType = TriggerTypes.begin(); triggerType != TriggerTypes.end(); triggerType++) {
    TriggerHistogramMap[*triggerType]->Fill(0) ;
    nEvts++;  
  }
  

  //loop over all trigger paths in the event
  for (BNtriggerCollection::const_iterator trigger = TriggerCollection->begin(); trigger != TriggerCollection->end(); trigger++) {    
    if(trigger->pass != 1) continue;
        
    //loop over the different types of triggers of interest
    for(std::vector<string>::const_iterator triggerType = TriggerTypes.begin(); triggerType != TriggerTypes.end(); triggerType++){
      //loop over the different trigger names as specified by the user
      for(std::vector<string>::const_iterator triggerName = TriggerNameMap[*triggerType].begin(); triggerName != TriggerNameMap[*triggerType].end(); triggerName++){
	if(trigger->name.find(*triggerName)!=std::string::npos){
	  TriggerHistogramMap[*triggerType]->Fill(triggerName-TriggerNameMap[*triggerType].begin()+1);
	  InclusiveORMap[*triggerType] = true;
	}
      }
    }
  }  
  //fill final bin if any triggers were passed
  for(std::vector<string>::const_iterator triggerType = TriggerTypes.begin(); triggerType != TriggerTypes.end(); triggerType++)
    if(InclusiveORMap[*triggerType]) TriggerHistogramMap[*triggerType]->Fill(TriggerNameMap[*triggerType].size()+1);

} // void TriggerEfficiencyAnalyzer::analyze (const edm::Event &event, const edm::EventSetup &setup)  


DEFINE_FWK_MODULE(TriggerEfficiencyAnalyzer);


