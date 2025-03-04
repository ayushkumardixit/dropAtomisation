// Get stresses from snapshots

#include "axi.h"
#include "navier-stokes/centered.h"
#include "fractions.h"
#include "log-conform.h"

char filename[80];
int nx, ny, len;
double xmin, ymin, xmax, ymax, Deltax, Deltay, Ohs, Ohp, Oha;
scalar f[], D2c[], vel[], APolyRR[], APolyQQ[], APolyZZ[], APolyRZ[], Gpd[], taup[];
scalar * list = NULL;

int main(int a, char const *arguments[])
{
  sprintf (filename, "%s", arguments[1]);
  xmin = atof(arguments[2]); ymin = atof(arguments[3]);
  xmax = atof(arguments[4]); ymax = atof(arguments[5]);
  ny = atoi(arguments[6]);
  Ohs = atof(arguments[7]); Ohp = atof(arguments[8]);
  Oha = atof(arguments[9]);

  list = list_add (list, D2c);
  list = list_add (list, vel);
  list = list_add (list, u.x);
  list = list_add (list, u.y);
  list = list_add(list, APolyRR);
  list = list_add(list, APolyRZ);
  list = list_add(list, APolyZZ);
  list = list_add(list, APolyQQ);
  list = list_add(list, taup);

  // boundary conditions
  u.n[right] = neumann(0.);
  p[right] = dirichlet(0.);

  /*
  Actual run and codes!
  */
  restore (file = filename);
  f.prolongation = fraction_refine;
  boundary((scalar *){f, u.x, u.y});

  foreach() {
    double D11 = (u.y[0,1] - u.y[0,-1])/(2*Delta);
    double D22 = (u.y[]/y);
    double D33 = (u.x[1,0] - u.x[-1,0])/(2*Delta);
    double D13 = 0.5*( (u.y[1,0] - u.y[-1,0] + u.x[0,1] - u.x[0,-1])/(2*Delta) );
    double D2 = sqrt(sq(D11)+sq(D22)+sq(D33)+2.0*sq(D13))/sqrt(2.0);
    D2c[] = clamp(f[], 0., 1.)*D2;
    vel[] = sqrt(sq(u.x[])+sq(u.y[]));


    if (D2c[] > 0.){
      D2c[] = log(D2c[])/log(10);
    } else {
      D2c[] = -10;
    }

    double fa = (Gpd[] != 0 ? 1.0/Gpd[]: 0.);

    pseudo_t A;
    A.x.y = fa*tau_p.x.y[];
    foreach_dimension(){
      A.x.x = (fa*tau_p.x.x[] + 1.);
    }

    
    double Aqq = (1. + fa*tau_qq[]);
    APolyQQ[] = log (Aqq); 

    pseudo_v Lambda;
    pseudo_t R;
    diagonalization_2D (&Lambda, &R, &A);

    APolyRZ[] = R.x.x*R.y.x*log(Lambda.x) + R.y.y*R.x.y*log(Lambda.y);

	  APolyZZ[] = sq(R.x.x)*log(Lambda.x) + sq(R.x.y)*log(Lambda.y);
    APolyRR[] = sq(R.y.y)*log(Lambda.y) + sq(R.y.x)*log(Lambda.x);
    taup[] = tau_p.x.x[];
    if (taup[] > 0.){
      taup[] = log(taup[])/log(10);
    } else {
      taup[] = -10;
    }
  }
  boundary ((scalar *){D2c, vel});

  FILE * fp = ferr;
  Deltay = (double)(ymax-ymin)/(ny);
  // fprintf(ferr, "%g\n", Deltay);
  nx = (int)(xmax - xmin)/Deltay;
  // fprintf(ferr, "%d\n", nx);
  Deltax = (double)(xmax-xmin)/(nx);
  // fprintf(ferr, "%g\n", Deltax);
  len = list_len(list);
  // fprintf(ferr, "%d\n", len);
  double ** field = (double **) matrix_new (nx, ny+1, len*sizeof(double));
  for (int i = 0; i < nx; i++) {
    double x = Deltax*(i+1./2) + xmin;
    for (int j = 0; j < ny; j++) {
      double y = Deltay*(j+1./2) + ymin;
      int k = 0;
      for (scalar s in list){
        field[i][len*j + k++] = interpolate (s, x, y);
      }
    }
  }

  for (int i = 0; i < nx; i++) {
    double x = Deltax*(i+1./2) + xmin;
    for (int j = 0; j < ny; j++) {
      double y = Deltay*(j+1./2) + ymin;
      fprintf (fp, "%g %g", x, y);
      int k = 0;
      for (scalar s in list){
        fprintf (fp, " %g", field[i][len*j + k++]);
      }
      fputc ('\n', fp);
    }
  }
  fflush (fp);
  fclose (fp);
  matrix_free (field);
}
