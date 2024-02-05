// Title: getting bag length from simulation snapshot

#include "axi.h"
#include "navier-stokes/centered.h"
#include "vof.h"
#include "tag.h"
#include "heights.h"

char filename[80], nameTrack[80];
scalar f[], *interfaces = {f};

// First argument should be filename.
int main(int a, char const *arguments[])
{
  sprintf(filename, "%s", arguments[1]);
  sprintf(nameTrack, "%s", arguments[2]);

  restore(file = filename);
  boundary((scalar *){f, u.x, u.y});

  double xmin = HUGE, xmax = -HUGE;
  double ymin = 0, ymax = 0;
  foreach ()
  {
    if (f[] > 0.5)
    {
      if (x < xmin){
        xmin = x; ymin = y;}
      if (x > xmax){
        xmax = x; ymax = y;}
    }
  }
  FILE * fp = ferr;
  fprintf(ferr, "%f %7.6e %7.6e %7.6e %7.6e\n", t, xmin, ymin, xmax, ymax);
  fflush(fp);
  fclose(fp);
}