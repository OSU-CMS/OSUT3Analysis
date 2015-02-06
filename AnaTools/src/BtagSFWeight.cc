#include "../interface/BtagSFWeight.h"

bool BtagSFWeight::filter(int t, int minTags)
{
  return (t >= minTags);
}



double BtagSFWeight::weight(vector<double> jets, int minTags)
{
  int njets=jets.size();
  int comb= 1 << njets;
  float pMC=0;
  for(int i=0;i < comb; i++){
    float mc=1.;
    int ntagged=0;
    for(int j=0;j<njets;j++){
      bool tagged = ((i >> j) & 0x1) == 1;
      if(tagged){
        ntagged++;
        mc*=jets[j];
      }
      else{
        mc*=(1.-jets[j]);
      }
    }

    if(filter(ntagged, minTags)){
      pMC+=mc;
    }
  }
  if( pMC > 0)
      return pMC;
  else{
     pMC = 1;
     return pMC;
     }
}

double BtagSFWeight::sflookup(double jetCSV, double pt, double flavor, double jetEta)
{
    double jetSF = 1;

    if(abs(flavor) == 4 || abs(flavor) == 5)
       {
            if(jetCSV > 0.244 && jetCSV <= 0.679)
               {
                 if( pt <= 20 )
                      {
                           pt = 20;
                      }
                else if( pt >= 800 )
                  {
                      pt = 800;
                   }
                 jetSF = 0.981149*((1.+(-0.000713295*pt))/(1.+(-0.000703264*pt)));
        }
             if(jetCSV > 0.679 && jetCSV <= 0.898 )
               {
                 if( pt <= 20 )
                      {
                    pt = 20;
                 }
                else if( pt >= 800 )
                   {
                     pt = 800;
                   }
                jetSF = 0.726981*((1.+(0.253238*pt))/(1.+(0.188389*pt)));
         }
         if(jetCSV > 0.898 )
         {
                 if( pt <= 20 )
                  {
                     pt = 20;
                  }
                 else if( pt >= 800 )
                  {
                     pt = 800;
                  }
                 jetSF = 0.869965*((1.+(0.0335062*pt))/(1.+(0.0304598*pt)));
          }
      }
    else if (abs(flavor) == 1 || abs(flavor) == 2 || abs(flavor) == 3 || abs(flavor) == 0)
          {
            if ( jetCSV >= 0.898 )
                {
                    if( pt <= 20 )
                      {
                        pt = 20;
                      }
                    else if( pt >= 800 )
                      {
                        pt = 800;
                      }
                      jetSF = ((1.01739+(0.00283619*pt))+(-7.93013e-06*(pt*pt)))+(5.97491e-09*(pt*(pt*pt)));
                }
            if ( jetCSV >= 0.679 && jetCSV < 0.898)
                {
                      if( jetEta >= 0 && jetEta < 0.8 )
                          {
                             if( pt <= 20 )
                                {
                                  pt = 20;
                                }
                             else if( pt >= 800 )
                                {
                                  pt = 800;
                                }
                             jetSF =((1.06238+(0.00198635*pt))+(-4.89082e-06*(pt*pt)))+(3.29312e-09*(pt*(pt*pt)));
                           }
                       if( jetEta >= 0.8 && jetEta < 1.6 )
                          {
                             if( pt <= 20 )
                                {
                                  pt = 20;
                                }
                             else if( pt >= 800 )
                                {
                                  pt = 800;
                                }
                              jetSF = ((1.08048+(0.00110831*pt))+(-2.96189e-06*(pt*pt)))+(2.16266e-09*(pt*(pt*pt)));
                           }
                        if( jetEta >= 1.6 && jetEta < 2.4 )
                          {
                             if( pt <= 20 )
                                {
                                  pt = 20;
                                }
                             else if( pt >= 700 )
                                {
                                  pt = 700;
                                }
                             jetSF = ((1.09145+(0.000687171*pt))+(-2.45054e-06*(pt*pt)))+(1.7844e-09*(pt*(pt*pt)));
                           }
                       }
            if ( jetCSV >= 0.244 && jetCSV < 0.679)
                {
                      if( jetEta >= 0 && jetEta < 0.5 )
                          {
                             if( pt <= 20 )
                                {
                                  pt = 20;
                                }
                             else if( pt >= 800 )
                                {
                                  pt = 800;
                                }
                             jetSF = ((1.04901+(0.00152181*pt))+(-3.43568e-06*(pt*pt)))+(2.17219e-09*(pt*(pt*pt)));
                           }
                       if( jetEta >= 0.5 && jetEta < 1.0 )
                          {
                             if( pt <= 20 )
                                {
                                  pt = 20;
                                }
                             else if( pt >= 800 )
                                {
                                  pt = 800;
                                }
                              jetSF = ((0.991915+(0.00172552*pt))+(-3.92652e-06*(pt*pt)))+(2.56816e-09*(pt*(pt*pt)));
                           }
                        if( jetEta >= 1.0 && jetEta < 1.5 )
                          {
                             if( pt <= 20 )
                                {
                                  pt = 20;
                                }
                             else if( pt >= 800 )
                                {
                                  pt = 800;
                                }
                             jetSF = ((0.962127+(0.00192796*pt))+(-4.53385e-06*(pt*pt)))+(3.0605e-09*(pt*(pt*pt)));
                           }
                        if( jetEta >= 1.5 && jetEta < 2.4 )
                          {
                             if( pt <= 20 )
                                {
                                  pt = 20;
                                }
                             else if( pt >= 700 )
                                {
                                  pt = 700;
                                }
                             jetSF = ((1.06121+(0.000332747*pt))+(-8.81201e-07*(pt*pt)))+(7.43896e-10*(pt*(pt*pt)));
                           }
                       }

        }

    return jetSF;
}












