#pragma once

#include "data.hpp"
#include "tree_manip.hpp"
#include "gtr_model.hpp"
#include "xstrom.hpp"
#include <fstream>

namespace strom
    {

    class OutputManager
        {
        public:
                                                                OutputManager();
                                                                ~OutputManager();

            void                                                setModel(GTRModel::SharedPtr gtr) {_gtr_model = gtr;}
            void                                                setTreeManip(TreeManip::SharedPtr tm) {_tree_manip = tm;}

            void                                                openTreeFile(std::string filename, Data::SharedPtr data);
            void                                                openParameterFile(std::string filename, GTRModel::SharedPtr gtr);

            void                                                closeTreeFile();
            void                                                closeParameterFile();

            void                                                outputConsole(std::string s);
            void                                                outputTree(unsigned iter, TreeManip::SharedPtr tm);
            void                                                outputParameters(unsigned iter, double lnL, double lnP, double TL, GTRModel::SharedPtr gtr);


        private:

            TreeManip::SharedPtr                                _tree_manip;
            GTRModel::SharedPtr                                 _gtr_model;
            std::ofstream                                       _treefile;
            std::ofstream                                       _parameterfile;
            std::string                                         _tree_file_name;
            std::string                                         _param_file_name;

        public:

            typedef std::shared_ptr< OutputManager >            SharedPtr;
    };

inline OutputManager::OutputManager()
    {
    //std::cout << "Constructing an OutputManager" << std::endl;
    _tree_file_name = "trees.t";
    _param_file_name = "params.p";
    }

inline OutputManager::~OutputManager()
    {
    //std::cout << "Destroying an OutputManager" << std::endl;
    }

inline void OutputManager::openTreeFile(std::string filename, Data::SharedPtr data)
    {
    assert(!_treefile.is_open());
    _tree_file_name = filename;
    _treefile.open(_tree_file_name.c_str());
    if (!_treefile.is_open())
        throw XStrom(boost::str(boost::format("Could not open tree file \"%s\"") % _tree_file_name));

    _treefile << "#nexus\n\n";
    _treefile << data->createTaxaBlock() << std::endl;

    _treefile << "begin trees;\n";
    _treefile << data->createTranslateStatement() << std::endl;
    }

inline void OutputManager::closeTreeFile()
    {
    assert(_treefile.is_open());
    _treefile << "end;\n";
    _treefile.close();
    }

inline void OutputManager::openParameterFile(std::string filename, GTRModel::SharedPtr gtr)
    {
    assert(gtr);
    assert(!_parameterfile.is_open());
    _param_file_name = filename;
    _parameterfile.open(_param_file_name.c_str());
    if (!_parameterfile.is_open())
        throw XStrom(boost::str(boost::format("Could not open parameter file \"%s\"") % _param_file_name));
    _parameterfile << boost::str(boost::format("%s\t%s\t%s\t%s\t%s") % "iter" % "lnL" % "lnPr" % "TL" % gtr->paramNamesAsString("\t")) << std::endl;
    }

inline void OutputManager::closeParameterFile()
    {
    assert(_parameterfile.is_open());
    _parameterfile.close();
    }

inline void OutputManager::outputConsole(std::string s)
    {
    std::cout << s << std::endl;
    }

inline void OutputManager::outputTree(unsigned iter, TreeManip::SharedPtr tm)
    {
    assert(_treefile.is_open());
    assert(tm);
    _treefile << boost::str(boost::format("  tree iter_%d = %s;") % iter % tm->makeNewick(5)) << std::endl;
    }

inline void OutputManager::outputParameters(unsigned iter, double lnL, double lnP, double TL, GTRModel::SharedPtr gtr)
    {
    assert(gtr);
    assert(_parameterfile.is_open());
    _parameterfile << boost::str(boost::format("%d\t%.5f\t%.5f\t%.5f\t%s") % iter % lnL % lnP % TL % gtr->paramValuesAsString("\t")) << std::endl;
    }

}
