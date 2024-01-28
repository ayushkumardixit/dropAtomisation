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

  foreach ()
  {
    if (f[] > 0.5)
    {
      if (x < xmin)
        xmin = x;
      if (x > xmax && y < 0.1)
        xmax = x;
    }
  }
  FILE * fp = ferr;
  fprintf(ferr, "%f %7.6e %7.6e\n", t, xmin, xmax);
  fflush(fp);
  fclose(fp);
}