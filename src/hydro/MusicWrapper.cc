/*******************************************************************************
 * Copyright (c) The JETSCAPE Collaboration, 2017
 *
 * For the full list of contributors see AUTHORS.
 * Report issues at https://github.com/JETSCAPE/JETSCAPE/issues
 * or via email to bugs.jetscape.org@gmail.com
 *
 * Distributed under the GNU General Public License 3.0 (GPLv3 or later).
 * See COPYING for details.
 ******************************************************************************/
// Copyright @ Bjoern Schenke, Sangyong Jeon, Charles Gale, and Chun Shen
#include <stdio.h>
#include <sys/stat.h>
#include <MakeUniqueHelper.h>

#include <string>
#include <vector>

#include "JetScapeLogger.h"
#include "MusicWrapper.h"

using namespace Jetscape;

MPI_MUSIC::MPI_MUSIC() {
    hydro_status = NOT_START;
    doCooperFrye = 0;
    SetId("MUSIC");
}


MPI_MUSIC::~MPI_MUSIC() {
    if (hydro_status != NOT_START) {
        delete music_hydro_ptr;
    }
}


void MPI_MUSIC::initialize_hydro(Parameter parameter_list) {
    INFO << "Initialize MUSIC ...";
    VERBOSE(8);
    tinyxml2::XMLElement *para =
                    GetHydroXML()->FirstChildElement("MUSIC");
    if (!para) {
        WARN << " : MUSIC not properly initialized in XML file ...";
        exit(-1);
    }
    string input_file = para->FirstChildElement("MUSIC_input_file")->GetText();
    para->FirstChildElement("Perform_CooperFrye_Feezeout")->QueryIntText(
                                                                &doCooperFrye);
    int argc = 2;
    char **argv = new char* [argc];
    argv[0] = new char[9];
    strcpy(argv[0], "mpihydro");
    argv[1] = new char[input_file.length() + 1];
    strcpy(argv[1], input_file.c_str());
    std::cout << "check input for MUSIC: " << std::endl;
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << "  ";
    }
    std::cout << endl;
    music_hydro_ptr = new MUSIC(argc, argv);

    for (int i = 0; i < argc; i++) {
        delete[] argv[i];
    }
    delete[] argv;
}


void MPI_MUSIC::evolve_hydro() {
    VERBOSE(8);
    INFO << "Initialize density profiles in MUSIC ...";
    std::vector<double> entropy_density = ini->entropy_density_distribution_;
    double dx = ini->get_x_step();
    if (pre_eq_ptr == nullptr) {
        music_hydro_ptr->initialize_hydro_from_vector(entropy_density, dx);
    } else {
        music_hydro_ptr->initialize_hydro_from_pre_equilibrium_vectors(dx,
                                                        pre_eq_ptr->e_,
                                                        pre_eq_ptr->utau_,
                                                        pre_eq_ptr->ux_,
                                                        pre_eq_ptr->uy_,
                                                        pre_eq_ptr->ueta_,
                                                        pre_eq_ptr->pi00_,
                                                        pre_eq_ptr->pi01_,
                                                        pre_eq_ptr->pi02_,
                                                        pre_eq_ptr->pi03_,
                                                        pre_eq_ptr->pi11_,
                                                        pre_eq_ptr->pi12_,
                                                        pre_eq_ptr->pi13_,
                                                        pre_eq_ptr->pi22_,
                                                        pre_eq_ptr->pi23_,
                                                        pre_eq_ptr->pi33_,
                                                        pre_eq_ptr->bulk_Pi_);
    }

    INFO << "initial density profile dx = " << dx << " fm";
    hydro_status = INITIALIZED;
    if (hydro_status == INITIALIZED) {
        INFO << "running MUSIC ...";
        music_hydro_ptr->run_hydro();
        hydro_status = FINISHED;
    }
    if (hydro_status == FINISHED && doCooperFrye == 1) {
        music_hydro_ptr->run_Cooper_Frye(1);
    }
}


void MPI_MUSIC::get_hydro_info(
        Jetscape::real t, Jetscape::real x, Jetscape::real y, Jetscape::real z,
        std::unique_ptr<FluidCellInfo>& fluid_cell_info_ptr) {
    fluid_cell_info_ptr = std::make_unique<FluidCellInfo>();
    fluidCell *fluidCell_ptr = new fluidCell;
    music_hydro_ptr->get_hydro_info(x, y, z, t, fluidCell_ptr);
    fluid_cell_info_ptr->energy_density = fluidCell_ptr->ed;
    fluid_cell_info_ptr->entropy_density = fluidCell_ptr->sd;
    fluid_cell_info_ptr->temperature = fluidCell_ptr->temperature;
    fluid_cell_info_ptr->pressure = fluidCell_ptr->pressure;
    fluid_cell_info_ptr->vx = fluidCell_ptr->vx;
    fluid_cell_info_ptr->vy = fluidCell_ptr->vy;
    fluid_cell_info_ptr->vz = fluidCell_ptr->vz;
    fluid_cell_info_ptr->mu_B = 0.0;
    fluid_cell_info_ptr->mu_C = 0.0;
    fluid_cell_info_ptr->mu_S = 0.0;
    fluid_cell_info_ptr->qgp_fraction = 0.0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fluid_cell_info_ptr->pi[i][j] = fluidCell_ptr->pi[i][j];
        }
    }
    fluid_cell_info_ptr->bulk_Pi = fluidCell_ptr->bulkPi;
    delete fluidCell_ptr;
}

