#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!stop valueLookup
double
ValueLookup::valueLookup (const BNstop &object, string variable){

  double value = numeric_limits<int>::min ();
  BNstop *obj = new BNstop (object);

  try
    {
      value = getMember ("BNstop", obj, variable);
    }
  catch (...)
    {
      // derived quantities
      if (variable == "d0"){
        double vx = object.vx - chosenVertex ()->x,
          vy = object.vy - chosenVertex ()->y,
          px = object.px,
          py = object.py,
          pt = object.pt;
        value = (-vx * py + vy * px) / pt;
      }

      else if (variable == "dz"){
        double vx = object.vx - chosenVertex ()->x,
          vy = object.vy - chosenVertex ()->y,
          vz = object.vz - chosenVertex ()->z,
          px = object.px,
          py = object.py,
          pz = object.pz,
          pt = object.pt;
        value = vz - (vx * px + vy * py)/pt * (pz/pt);
      }

      else if (variable == "minD0"){
        double minD0=999;
        for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
          double vx = object.vx - vertex->x,
            vy = object.vy - vertex->y,
            px = object.px,
            py = object.py,
            pt = object.pt;
          value = (-vx * py + vy * px) / pt;
          if(abs(value) < abs(minD0)) minD0 = value;
        }
        value = minD0;
      }
      else if (variable == "minDz"){
        double minDz=999;
        for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
          double vx = object.vx - vertex->x,
            vy = object.vy - vertex->y,
            vz = object.vz - vertex->z,
            px = object.px,
            py = object.py,
            pz = object.pz,
            pt = object.pt;
          value = vz - (vx * px + vy * py)/pt * (pz/pt);
          if(abs(value) < abs(minDz)) minDz = value;
        }
        value = minDz;
      }
      else if(variable == "distToVertex"){
        value = sqrt((object.vx-chosenVertex()->x)*(object.vx-chosenVertex()->x) + \
                     (object.vy-chosenVertex()->y)*(object.vy-chosenVertex()->y) + \
                     (object.vz-chosenVertex()->z)*(object.vz-chosenVertex()->z));
      }
      else if (variable == "minDistToVertex"){
        double minDistToVertex=999;
        for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
          value = sqrt((object.vx-vertex->x)*(object.vx-vertex->x) + \
                       (object.vy-vertex->y)*(object.vy-vertex->y) + \
                       (object.vz-vertex->z)*(object.vz-vertex->z));

          if(abs(value) < abs(minDistToVertex)) minDistToVertex = value;
        }
        value = minDistToVertex;
      }
      else if (variable == "distToVertexDifference"){
        double minDistToVertex=999;
        for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
          value = sqrt((object.vx-vertex->x)*(object.vx-vertex->x) + \
                       (object.vy-vertex->y)*(object.vy-vertex->y) + \
                       (object.vz-vertex->z)*(object.vz-vertex->z));

          if(abs(value) < abs(minDistToVertex)) minDistToVertex = value;
        }
        double distToChosenVertex = sqrt((object.vx-chosenVertex()->x)*(object.vx-chosenVertex()->x) + \
                                         (object.vy-chosenVertex()->y)*(object.vy-chosenVertex()->y) + \
                                         (object.vz-chosenVertex()->z)*(object.vz-chosenVertex()->z));

        value = distToChosenVertex - minDistToVertex;
      }

      else if (variable == "closestVertexRank"){
        double minDistToVertex=999;
        int vertex_rank = 0;
        for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
          vertex_rank++;
          int dist = sqrt((object.vx-vertex->x)*(object.vx-vertex->x) + \
                          (object.vy-vertex->y)*(object.vy-vertex->y) + \
                          (object.vz-vertex->z)*(object.vz-vertex->z));

          if(abs(dist) < abs(minDistToVertex)){
            value = vertex_rank;
            minDistToVertex = dist;
          }
        }
      }

      else
        clog << "WARNING: invalid stop variable '" << variable << "'\n";
    }

  delete obj;
  return value;
} // end stop valueLookup
