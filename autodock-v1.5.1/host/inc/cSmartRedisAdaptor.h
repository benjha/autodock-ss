#ifndef __SMARTREDIS_ADAPTOR__
#define __SMARTREDIS_ADAPTOR__

#include <string>
#include <iomanip> 
#include <sstream>

#include "client.h"

// Autodock data structures
#include "getparameters.h"
#include "processresult.h"

class cSmartRedisAdaptor {

public:
    
    // mypars -> name of the dataset (ligand), ligand metada
    // my grid -> grid metadata 
    cSmartRedisAdaptor(bool cluster)
    {
        // Initialize a SmartRedis client
        // false - no redis cluster
        client  = new SmartRedis::Client(cluster);
    }

    void setStructures (const Liganddata* _ligand, const Dockpars* _pars, const Gridinfo* _grid, const Ligandresult* _results)
    {
        ligand  = _ligand;
        pars    = _pars;
        grid    = _grid;
        results = _results;
        
        // TODO: review dataset name, 
        // meanwhile using filename + my suffix
        std::string tmp(pars->ligandfile);
        std::string dataSetName = tmp.substr( tmp.find_last_of("/") + 1 ) + "_" + "testing";
        printf("<SMARTREDISADAPTOR> Dataset name: %s\n", dataSetName.data());

        dataset = new SmartRedis::DataSet(dataSetName);
        setMetaData();
        setTimeSteps ();
        client->put_dataset((SmartRedis::DataSet &)*dataset);

    }

    ~cSmartRedisAdaptor()
    {
        delete client;
        delete dataset;
    }

private:

    SmartRedis::DataSet *dataset;
    SmartRedis::Client *client;
    
    // Autodock structures
    const Liganddata*   ligand;
    const Dockpars*     pars;
    const Gridinfo*     grid;
    const Ligandresult* results;

    void setMetaData ()
    {
        dataset->add_meta_string("ligand_file",         std::string(pars->ligandfile));
        dataset->add_meta_string("grid_fld_file",       std::string(pars->fldfile));
        
        dataset->add_meta_scalar("runs",                &pars->num_of_runs,         SmartRedis::MetaDataType::uint64);
        dataset->add_meta_scalar("nrg_evaluations",     &pars->num_of_energy_evals, SmartRedis::MetaDataType::uint64);
        dataset->add_meta_scalar("generations",         &pars->num_of_generations,  SmartRedis::MetaDataType::uint64);
        dataset->add_meta_scalar("population_size",     &pars->pop_size,            SmartRedis::MetaDataType::uint64);
        dataset->add_meta_scalar("crossover_rate",      &pars->crossover_rate,      SmartRedis::MetaDataType::flt);
        dataset->add_meta_scalar("tourn_sel_prob_lim",  &pars->tournament_rate,     SmartRedis::MetaDataType::flt);
        dataset->add_meta_scalar("mutation_rate",       &pars->mutation_rate,       SmartRedis::MetaDataType::flt);
        dataset->add_meta_scalar("max_delta_angle",     &pars->abs_max_dang,        SmartRedis::MetaDataType::flt);
        dataset->add_meta_scalar("local_search_rate",   &pars->lsearch_rate,        SmartRedis::MetaDataType::flt);
        dataset->add_meta_scalar("max_local_search_it", &pars->max_num_of_iters,    SmartRedis::MetaDataType::uint64);
        dataset->add_meta_scalar("rho_lower_bond",      &pars->rho_lower_bound,     SmartRedis::MetaDataType::flt);
        dataset->add_meta_scalar("cons_limit",          &pars->cons_limit,          SmartRedis::MetaDataType::uint64);
        dataset->add_meta_scalar("rmsd_tolerance",      &pars->rmsd_tolerance,      SmartRedis::MetaDataType::flt);
        
        double delta_mov        = pars->abs_max_dmov*grid->spacing;
        double delta_mov_spread = pars->base_dmov_mul_sqrt3*grid->spacing/sqrt(3.0);
        double delta_ang_spread = pars->base_dang_mul_sqrt3/sqrt(3.0);

        dataset->add_meta_scalar("max_delta_movement",  &delta_mov,
                                    SmartRedis::MetaDataType::dbl);
        dataset->add_meta_scalar("local_search_delta_movement_spread", &delta_mov_spread,
                                    SmartRedis::MetaDataType::dbl);
        dataset->add_meta_scalar("local_search_delta_angle_spread", &delta_ang_spread,
                                    SmartRedis::MetaDataType::dbl);
        dataset->add_meta_scalar("handled_symmetry", &pars->handle_symmetry,
                                    SmartRedis::MetaDataType::int32);
    }

    void setTimeSteps ()
    {
        unsigned int line_count = 0;
        unsigned int atom_cnt;
        std::string pdbqt_template, dockedModel;
        std::string tmpStr, lineout, inputLigand;
        std::vector<unsigned int> atom_data;

        while (line_count < ligand->ligand_line_count)
        {
            // extracts ligand's contents from file
            // strcpy(tempstr,ligand->file_content[line_count].c_str());
            tmpStr = ligand->file_content[line_count++];
            inputLigand += tmpStr;
            // creates a template to then be filled with atom x,y,z positions
            if ( !tmpStr.compare(0,4,"ATOM") || !tmpStr.compare(0,6,"HETATM"))
            {
                // check field sizes on
                // https://www.cgl.ucsf.edu/chimera/docs/UsersGuide/tutorials/pdbintro.html
                pdbqt_template.append(tmpStr, 0, 30);
                atom_data.push_back(pdbqt_template.length());
            }
            else
            {
                if ( !tmpStr.compare(0,4,"ROOT"))
                {
                    pdbqt_template += "USER                              x       y       z     vdW  Elec       q    Type\n";
                    pdbqt_template += "USER                           _______ _______ _______ _____ _____    ______ ____\n";
                }
                pdbqt_template += tmpStr;
            }
            //printf ("SMARTREDISADAPTOR: pdbqt_template length %lu\n", pdbqt_template.length());
            }
        std::string tensorName = "input_ligand";
        // printf("SMARTREDISADAPTOR: line_count %d\n",line_count); 
        // printf("\n<SMARTREDISADAPTOR> pdbqt_template %s\n",pdbqt_template.c_str()); 
        std::vector<size_t> tensorSize;
        tensorSize.push_back(inputLigand.size());
        // printf("<SMARTREDISADAPTOR> INPUT LIGAND PDBQT FILE: %s\n", inputLigand.c_str());
        // printf("<SMARTREDISADAPTOR> Putting tensor: %s\n",inputLigand.c_str());
        // printf("<SMARTREDISADAPTOR> Tensor size: %lu\n",inputLigand.size());
        dataset->add_tensor(tensorName, (void*)inputLigand.c_str(), tensorSize, SmartRedis::TensorType::uint8, SmartRedis::MemoryLayout::contiguous);
        
        for (int i=0; i<pars->num_of_runs; i++)
        {
            std::string dt = std::to_string(i+1);
            tensorName = "time_step_"  + dt + "\n";
            tmpStr =  "MODEL         " + dt + "\n";
            tmpStr += "USER    Run = " + dt + "\n";
            tmpStr += "USER\n";
            
            //printf("\n<SMARTREDISADAPTOR> atom_data %d ligand_ref->num_of_atoms %d", atom_data.size(), ligand->num_of_atoms);
            dockedModel = pdbqt_template;
            // inserting text from the end means prior text positions won't shift
            // so there's less to keep track off ;-)
            for(atom_cnt = ligand->num_of_atoms; atom_cnt-->0;)
            {
                //char* line = new  ;
                std::stringstream ss;
                ss << std::fixed << std::setw(8) << std::setprecision(3) << results[i].atom_idxyzq[atom_cnt][1];
                ss << std::fixed << std::setw(8) << std::setprecision(3) << results[i].atom_idxyzq[atom_cnt][2];
                ss << std::fixed << std::setw(8) << std::setprecision(3) << results[i].atom_idxyzq[atom_cnt][3];
                ss << std::fixed << std::setw(6) << std::setprecision(2) << std::showpos
                    << copysign(fmin(fabs(results[i].peratom_vdw[atom_cnt]),99.99),results[i].peratom_vdw[atom_cnt]); // vdw
                ss << std::fixed << std::setw(6) << std::setprecision(2) << std::showpos
                    << copysign(fmin(fabs(results[i].peratom_elec[atom_cnt]),99.99),results[i].peratom_elec[atom_cnt]); // elec
                ss << "    " << std::fixed << std::setw(6) << std::setprecision(3)
                    << results[i].atom_idxyzq[atom_cnt][4]; // q
                ss << " " << std::setw(2) << std::left << ligand->atom_types[((int)results[i].atom_idxyzq[atom_cnt][0])] << "\n"; // type
                dockedModel.insert(atom_data[atom_cnt],ss.str());
            }
            tmpStr+=dockedModel.c_str();
            //dataset->add_tensor(tensorName, tmpStr.c_str(), tmpStr.size(), SmartRedis::TensorType::uint8, SmartRedis::MemoryLayout::contiguous);       
        }
        //printf("\n<SMARTREDISADAPTOR> tmpStr %s\n", tmpStr.c_str());

    }
};

#endif /* __SMARTREDIS_ADAPTOR__ */