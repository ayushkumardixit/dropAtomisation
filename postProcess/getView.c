// making videos

#include "grid/octree.h"
#include "navier-stokes/centered.h"
#include "view.h"

scalar f[];
char filename[80], Imagename[80];
int main(int a, char const *arguments[])
{
  sprintf (filename, "%s", arguments[1]);
  sprintf (Imagename, "%s",arguments[2]);
  restore (file = filename);
  boundary((scalar *){f});
  
view (quat = {-0.206, 0.142, 0.042, 0.967},
      fov = 30, near = 0.01, far = 1000,
      tx = -0.445, ty = 0.105, tz = -1.155,
      width = 1212, height = 833);
  
      
  box ();
  //cells ();
  draw_vof (c = "f");
  save (Imagename);
}
