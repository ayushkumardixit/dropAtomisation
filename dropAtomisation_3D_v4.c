/*
Atomisation of axisymmetric viscoelastic drop
*/

#include "grid/octree.h"
#include "navier-stokes/centered.h"
#define FILTERED // Smear density and viscosity jumps
#include "two-phase.h"

// #include "log-conform.h" // main version
// #include "log-conform-elastic_v5.h" // elastic version -> With Deborah number -> \infty
#include "navier-stokes/conserving.h"
#include "tension.h"
// #include "distance.h"
//#include "adapt_wavelet_limited_v2.h"

#define tsnap (0.5) // 0.001 only for some cases. 
// Error tolerancs
#define fErr (1e-2)                                 // error tolerance in f1 VOF
#define KErr (1e-4)                                 // error tolerance in VoF curvature calculated using heigh function method (see adapt event)
#define VelErr (1e-2)                               // error tolerances in velocity -- Use 1e-2 for low Oh and 1e-3 to 5e-3 for high Oh/moderate to high J
#define AErr (1e-3)                             // error tolerances in conformation inside the liquid

// boundary conditions
//u.n[top] = neumann(0.);
//p[top] = dirichlet(0.);
//u.t[top] = dirichlet(1.);

u.n[left]  = dirichlet(1.);
// p[left] = dirichlet(0);

u.n[right] = neumann(0.);
p[right] = dirichlet(0);

int MAXlevel;
// We -> Weber number
// Oh -> Solvent Ohnesorge number
// Oha -> air Ohnesorge number
// De -> Deborah number
// Ec -> Elasto-capillary number

double Oh, Oha, De, We, Ec, tmax, mus;
char nameOut[80], dumpFile[80];
// scalar Gpd[]; scalar lambdad[];

int  main(int argc, char const *argv[]) {
  dtmax = 1e-5; //  BEWARE of this for stability issues. 

  L0 = 20;
  origin (0, -L0/2, -L0/2);

  // Values taken from the terminal
  MAXlevel = atoi(argv[1]);
  // De = atof(argv[2]);
  Ec = atof(argv[2]);
  We = atof(argv[3]);
  Oh = atof(argv[4]);
  tmax = atof(argv[5]);

  // Ensure that all the variables were transferred properly from the terminal or job script.
  if (argc < 5){
    fprintf(ferr, "Lack of command line arguments. Check! Need %d more arguments\n", 5-argc);
    return 1;
  }
  fprintf(ferr, "Level %d, Ec %2.1e, We %2.1e, Oh %2.1e\n", MAXlevel, Ec, We, Oh);
  init_grid (1 << 7);
  // Create a folder named intermediate where all the simulation snapshots are stored.
  char comm[80];
  sprintf (comm, "mkdir -p intermediate");
  system(comm);
  // Name of the restart file. See writingFiles event.
  sprintf (dumpFile, "dump");


  rho1 = 830, rho2 = 1;
  mus = 0.018;
  mu1 = sqrt(rho1)*Oh/sqrt(We);
  mu2 = mus * mu1;

  f.sigma = 1.0/We;

  // polymers
  // Gp = Gpd;
//  lambda = lambdad;

  run();
}

// event properties (i++) {
//   foreach () {
//     Gpd[] = (f[] > 1.-1e-6 ? Ec: 0.); // this is an artificial patch for now. The code has issues with VE terms in the interfacial cells!
// //   lambdad[] = (f[] > 1.-1e-6 ? De: 0.); //De*clamp(f[], 0., 1.);
//   }
// }

event init (t = 0) {
  if (!restore (file = dumpFile)){
   fraction (f, 1 - (x-3)*(x-3) - y*y - z*z);
  }
}

/**
## Adaptive Mesh Refinement
*/
event adapt(i++){
  scalar KAPPA[];
  curvature(f, KAPPA);
   adapt_wavelet ((scalar *){f, u.x, u.y, u.z, KAPPA},
      (double[]){fErr, VelErr, VelErr, VelErr, KErr},
      MAXlevel, MAXlevel-4);
}

/**
## Dumping snapshots
*/
event writingFiles (t = 0; t += tsnap; t <= tmax) {
  dump (file = dumpFile);
  sprintf (nameOut, "intermediate/snapshot-%5.4f", t);
  dump(file=nameOut);
}

/**
## Ending Simulation
*/
event end (t = end) {
  if (pid() == 0)
    fprintf(ferr, "Level %d, Ec %2.1e, We %2.1e, Oh %2.1e, Oha %2.1e\n", MAXlevel, Ec, We, Oh, Oha);
}

/**
## Log writing
*/
event logWriting (i++) {

  double ke = 0.;
  foreach (reduction(+:ke)){
    ke += (0.5*rho(f[])*(sq(u.x[]) + sq(u.y[]) + sq(u.z[])))*(Delta*Delta*Delta);
  }
  static FILE * fp;
  if (pid() == 0) {
    if (i == 0) {
      fprintf (ferr, "Ec We Oh i dt t ke\n");
      fp = fopen ("log", "w");
      fprintf(fp, "Level %d, Ec %2.1e, We %2.1e, Oh %2.1e, Oha %2.1e\n", MAXlevel, Ec, We, Oh, Oha);
      fprintf (fp, "i dt t ke\n");
      fprintf (fp, "%d %g %g %g\n", i, dt, t, ke);
      fclose(fp);
    } else {
      fp = fopen ("log", "a");
      fprintf (fp, "%g %g %g %d %g %g %g\n", Ec, We, Oh, i, dt, t, ke);
      fclose(fp);
    }
    fprintf (ferr, "%g %g %g %d %g %g %g\n", Ec, We, Oh, i, dt, t, ke);
  }

  assert(ke > -1e-10);

  if (ke > 1e4 && i > 1e1){
    if (pid() == 0){
      fprintf(ferr, "The kinetic energy blew up. Stopping simulation\n");
      fp = fopen ("log", "a");
      fprintf(fp, "The kinetic energy blew up. Stopping simulation\n");
      fclose(fp);
      dump(file=dumpFile);
      return 1;
    }
  }
  if (ke < 1e-8 && i > 1e1){
    if (pid() == 0){
      fprintf(ferr, "kinetic energy too small now! Stopping!\n");
      dump(file=dumpFile);
      fp = fopen ("log", "a");
      fprintf(fp, "kinetic energy too small now! Stopping!\n");
      fclose(fp);
      return 1;
    }
  }
}