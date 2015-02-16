BOOL bSphereTest(CObject3D* obj1, CObject3D* obj2)
{
  //Initialize the return value
  *t = 0.0f;

  // Relative velocity
  D3DVECTOR dv  = obj2->prVelocity - obj1->prVelocity;
  // Relative position
  D3DVECTOR dp  = obj2->prPosition - obj1->prPosition;
  //Minimal distance squared
  float r = obj1->fRadius + obj2->fRadius;
  //dP^2-r^2
  float pp = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z - r*r;
  //(1)Check if the spheres are already intersecting
  if ( pp < 0 ) return true;

  //dP*dV
  float pv  = dp.x * dv.x + dp.y * dv.y + dp.z * dv.z;
  //(2)Check if the spheres are moving away from each other
  if ( pv >= 0 ) return false;

  //dV^2
  float vv = dv.x * dv.x + dv.y * dv.y + dv.z * dv.z;
  //(3)Check if the spheres can intersect within 1 frame
  if ( (pv + vv) <= 0 && (vv + 2 * pv + pp) >= 0 ) return false;

  //tmin = -dP*dV/dV*2 
  //the time when the distance between the spheres is minimal
  float tmin = -pv/vv;

  //Discriminant/(4*dV^2) = -(dp^2-r^2+dP*dV*tmin)
  return ( pp + pv * tmin > 0 );
}
