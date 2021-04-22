#pragma once
#include "jdVector3.h"
namespace jdEngineSDK {
  class jdRTSphere {
   public:
    JDVector3 center;                           /// position of the sphere 
    float radius, radius2;                  /// sphere radius and radius^2 
    JDVector3 surfaceColor, emissionColor;      /// surface color and emission (light) 
    float transparency, reflection;         /// surface transparency and reflectivity 
    jdRTSphere(
      const JDVector3& c,
      const float& r,
      const JDVector3& sc,
      const float& refl = 0,
      const float& transp = 0,
      const JDVector3& ec = {0,0,0}) :
      center(c), radius(r), radius2(r* r), surfaceColor(sc), emissionColor(ec),
      transparency(transp), reflection(refl)
    { /* empty */
    }
    //Compute a ray - sphere intersection using the geometric solution sphere
    bool 
    intersect(const JDVector3& rayorig, const JDVector3& raydir, float& t0, float& t1) const {
      JDVector3 l = center - rayorig;
      float tca = l.dot(raydir);
      if (tca < 0) return false;
      float d2 = l.dot(l) - tca * tca;
      if (d2 > radius2) return false;
      float thc = sqrt(radius2 - d2);
      t0 = tca - thc;
      t1 = tca + thc;
      return true;
    }
  };
}
