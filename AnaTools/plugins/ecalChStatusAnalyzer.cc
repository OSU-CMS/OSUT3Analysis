// -*- C++ -*-
//
// Package:    ecalChStatusAnalyzer
// Class:      ecalChStatusAnalyzer
// 
/**\class ecalChStatusAnalyzer ecalChStatusAnalyzer.cc Demo/ecalChStatusAnalyzer/src/ecalChStatusAnalyzer.cc

Description: Creates a text file with eta, phi coordinates of all dead channels in the Ecal (DeadEcalChannels.txt); Makes plots of dead Ecal channels in the barrel and in the e\
ndcap (deadEcalMap.root)                                                                                                                                                          
                                                                                                                                                                                  
 Implementation:                                                                                                                                                                  
 cmsRun ecalChStatusAnalyzer_cfg.py  

*/
//
// Original Author:  Jessica Brinson
//         Created:  Tue Nov 12 05:13:16 EST 2013
// $Id$
//
//


// system include files
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <TFile.h>
#include <TStyle.h>
#include <TH1.h>
#include <TH2.h>
#include <map>
#include <set>
#include <vector>


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//needed for identifying the dead channels in the Ecal                                                                                                                             
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloTopology/interface/EcalTrigTowerConstituentsMap.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

using namespace reco;
using namespace std;
using namespace edm;

//
// class declaration
//

class ecalChStatusAnalyzer : public edm::EDAnalyzer {
   public:
      explicit ecalChStatusAnalyzer(const edm::ParameterSet& );
      ~ecalChStatusAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;


  void getChannelStatusMaps();
  double writeDeadEcalChannels();
  
  // ----------member data ---------------------------                                                                                                                             
  edm::Service<TFileService> fs;

  //needed for identifying the dead channels in the Ecal                                                                                                                           
  edm::ESHandle<EcalChannelStatus>  chanstat_;
  edm::ESHandle<CaloGeometry>       geometry_;
  edm::ESHandle<EcalTrigTowerConstituentsMap> ttMap_;

  std::map<DetId, std::vector<double> > EcalAllDeadChannelsValMap_;
  std::map<DetId, std::vector<int> >    EcalAllDeadChannelsBitMap_;
  std::map<DetId, EcalTrigTowerDetId>   EcalAllDeadChannelsTTMap_;

  const int maskedEcalChannelStatusThreshold_;

  //                                                                                                                                                                               
  //declare histograms                                                                                                                                                             
  //                                                                                                                                                                               
  TH2D* deadBarrelEtaVsPhi;
  TH2D* deadEndcapEtaVsPhi;
  TH2D* deadEcalEtaVsPhi;


};

//
// constructors and destructor
//
ecalChStatusAnalyzer::ecalChStatusAnalyzer(const edm::ParameterSet& iConfig)
:
  maskedEcalChannelStatusThreshold_(iConfig.getUntrackedParameter<int>("maskedEcalChannelStatusThreshold", 3))
{
   //now do what ever initialization is needed

  deadBarrelEtaVsPhi= fs->make<TH2D> ("deadBarrelEtaVsPhi", ";#phi;deadBarrelChannels (#eta)" , 342, -3, 3, 360, -3.14, 3.14);
  deadEndcapEtaVsPhi= fs->make<TH2D> ("deadEndcapEtaVsPhi", ";#phi ;deadEndcapChannels (#eta)", 342, -3, 3, 360, -3.14, 3.14);


}


ecalChStatusAnalyzer::~ecalChStatusAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
ecalChStatusAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  iSetup.get<EcalChannelStatusRcd>().get(chanstat_);
  iSetup.get<CaloGeometryRecord>   ().get(geometry_);
  iSetup.get<IdealGeometryRecord>().get(ttMap_);



  getChannelStatusMaps();
  writeDeadEcalChannels();

  /*#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
   #endif*/
}



void ecalChStatusAnalyzer::getChannelStatusMaps(){
  //written using code from the function getChannelStatusMaps() in http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/RecoMET/METFilters/plugins/EcalDeadCellDeltaRFilter.cc?revision=1.2&view=markup                                                                                                                                                                  
  gStyle->SetOptStat(0000000);
  EcalAllDeadChannelsValMap_.clear();
  EcalAllDeadChannelsBitMap_.clear();
  // double deltaRLowestBarrel = 99.;
  // double deltaRLowestEndcap = 99.;
  //looping over the barrel                                                                                                                                                         
  for( int ieta=-85; ieta<=85; ieta++ ){
   for( int iphi=0; iphi<=360; iphi++ ){
     if(! EBDetId::validDetId( ieta, iphi ) )  continue;
     const EBDetId detid = EBDetId( ieta, iphi, EBDetId::ETAPHIMODE );
     EcalChannelStatus::const_iterator chit = chanstat_->find( detid );
     // refer https://twiki.cern.ch/twiki/bin/viewauth/CMS/EcalChannelStatus                                                                                                       

     int status = ( chit != chanstat_->end() ) ? chit->getStatusCode() & 0x1F : -1;
     const CaloSubdetectorGeometry*  subGeom = geometry_->getSubdetectorGeometry (detid);
     const CaloCellGeometry*        cellGeom = subGeom->getGeometry (detid);
     double eta = cellGeom->getPosition ().eta ();
     double phi = cellGeom->getPosition ().phi ();
     if(-0.68 > eta && -0.71 < eta && phi > -2.3 && phi < -2.0)
       std::cout << "Eta, pho coord in barrel: " << eta << ", " << phi << " with status " << status <<std::endl;

     double theta = cellGeom->getPosition().theta();
     if(status >= maskedEcalChannelStatusThreshold_){
       std::vector<double> valVec; std::vector<int> bitVec;
       valVec.push_back(eta);
       valVec.push_back(phi);
       valVec.push_back(theta);
       bitVec.push_back(1); bitVec.push_back(ieta); bitVec.push_back(iphi); bitVec.push_back(status);

       deadBarrelEtaVsPhi->Fill(eta, phi);

       EcalAllDeadChannelsValMap_.insert( std::make_pair(detid, valVec) );
       EcalAllDeadChannelsBitMap_.insert( std::make_pair(detid, bitVec) );
     }

   }//ieta                                                                                                                                                                         
 }//iphi                                                                                                                                                                           
 //looping over the endcap      
 for( int ix=0; ix<=100; ix++ ){
   for( int iy=0; iy<=100; iy++ ){
     for( int iz=-1; iz<=1; iz++ ){
       if(iz==0)  continue;
       if(! EEDetId::validDetId( ix, iy, iz ) )  continue;

       const EEDetId detid = EEDetId( ix, iy, iz, EEDetId::XYMODE );
       EcalChannelStatus::const_iterator chit = chanstat_->find( detid );
       int status = ( chit !=chanstat_->end() ) ? chit->getStatusCode() & 0x1F : -1;

       const CaloSubdetectorGeometry*  subGeom = geometry_->getSubdetectorGeometry (detid);
       const CaloCellGeometry*        cellGeom = subGeom->getGeometry (detid);
       double eta = cellGeom->getPosition ().eta () ;
       double phi = cellGeom->getPosition ().phi () ;
       double theta = cellGeom->getPosition().theta();
       if(-0.68 > eta && -0.71 < eta && phi > -2.3 && phi < -2.0)
	 std::cout << "Eta, pho coord in endcap: " << eta << ", " << phi << " with status " << status <<std::endl;
       if(status >= maskedEcalChannelStatusThreshold_){
	 std::vector<double> valVec; std::vector<int> bitVec;
         valVec.push_back(eta); valVec.push_back(phi); valVec.push_back(theta);
         bitVec.push_back(2); bitVec.push_back(ix); bitVec.push_back(iy); bitVec.push_back(iz); bitVec.push_back(status);
         deadEndcapEtaVsPhi->Fill(eta, phi);

         EcalAllDeadChannelsValMap_.insert( std::make_pair(detid, valVec) );
         EcalAllDeadChannelsBitMap_.insert( std::make_pair(detid, bitVec) );
       }
     } // end loop iz                                                                                                                                                              
   } // end loop iy                                                                                                                                                                
 } // end loop ix                                                                                                                                                                  

 EcalAllDeadChannelsTTMap_.clear();
 std::map<DetId, std::vector<int> >::iterator bitItor;
 for(bitItor = EcalAllDeadChannelsBitMap_.begin(); bitItor != EcalAllDeadChannelsBitMap_.end(); bitItor++){
   const DetId id = bitItor->first;

   EcalTrigTowerDetId ttDetId = ttMap_->towerOf(id);
   EcalAllDeadChannelsTTMap_.insert(std::make_pair(id, ttDetId) );
 }

}

double ecalChStatusAnalyzer::writeDeadEcalChannels() {
  ofstream EcalMap;
  EcalMap.open("DeadEcalChannelsLatest.txt");
  for (std::map<DetId, std::vector<double> >::const_iterator ecal = EcalAllDeadChannelsValMap_.begin();
       ecal != EcalAllDeadChannelsValMap_.end();
       ecal++) {
    double etaEcal = ecal->second.at(0);
    double phiEcal = ecal->second.at(1);
    EcalMap<<etaEcal<<" "<<phiEcal<<"\n";
  }
  EcalMap.close();
  return 0;

}

// ------------ method called once each job just before starting event loop  ------------
void 
ecalChStatusAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ecalChStatusAnalyzer::endJob() 
{

  deadEcalEtaVsPhi = (TH2D*) deadEndcapEtaVsPhi->Clone("deadEcalEtaVsPhi");
  deadEcalEtaVsPhi ->SetTitle("Dead or Noisy ECAL Channels;#eta;#phi");
  deadEcalEtaVsPhi ->Add(deadBarrelEtaVsPhi);
  
  deadEcalEtaVsPhi ->SetMarkerStyle(5);
  deadEcalEtaVsPhi ->SetMarkerSize(0.7);
  deadEcalEtaVsPhi ->SetMarkerColor(12);
  deadEcalEtaVsPhi ->Write();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ecalChStatusAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

}

//define this as a plug-in
DEFINE_FWK_MODULE(ecalChStatusAnalyzer);
