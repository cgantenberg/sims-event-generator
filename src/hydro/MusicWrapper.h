/*******************************************************************************
 * Copyright (c) The JETSCAPE Collaboration, 2017
 *
 * For the full list of contributors see AUTHORS.
 * Report issues at https://github.com/amajumder/JETSCAPE-COMP/issues
 * or via email to bugs.jetscape.org@gmail.com
 *
 * Distributed under the GNU General Public License 3.0 (GPLv3 or later).
 * See COPYING for details.
 ******************************************************************************/
// Copyright @ Bjoern Schenke, Sangyong Jeon, Charles Gale, and Chun Shen
#ifndef MUSICWRAPPER_H
#define MUSICWRAPPER_H

#include "FluidDynamics.h"
#include "music.h"

using namespace Jetscape;

//! this is wrapper class for MUSIC so that it can be used as a external
//! library for the JETSCAPE integrated framework
class MPI_MUSIC: public FluidDynamics {
 private:
    // int mode;            //!< records running mode
    MUSIC *music_hydro_ptr;
    int doCooperFrye;    //!< flag to run Cooper-Frye freeze-out
                         //!< for soft particles

 public:
     MPI_MUSIC();
     ~MPI_MUSIC();

     void initialize_hydro(Parameter parameter_list);

     void evolve_hydro();
     void get_hydro_info(
        Jetscape::real t, Jetscape::real x, Jetscape::real y, Jetscape::real z,
		std::unique_ptr<FluidCellInfo>& fluid_cell_info_ptr);
     void get_hypersurface(Jetscape::real T_cut,
                           SurfaceCellInfo* surface_list_ptr) {};
};

#endif // MUSICWRAPPER_H
