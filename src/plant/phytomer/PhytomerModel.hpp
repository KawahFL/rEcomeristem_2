/**
 * @file ecomeristem/phytomer/Model.hpp
 * @author The Ecomeristem Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * Copyright (C) 2005-2016 Cirad http://www.cirad.fr
 * Copyright (C) 2012-2016 ULCO http://www.univ-littoral.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <defines.hpp>
#include <plant/phytomer/InternodeModel.hpp>
#include <plant/phytomer/LeafModel.hpp>

namespace model {

class PhytomerModel : public CoupledModel < PhytomerModel >
{
public:
    enum submodels { LEAF, INTERNODE };

    enum internals { KILL_LEAF, LEAF_PREDIM,
                     LEAF_BIOMASS, LEAF_BLADE_AREA, LEAF_VISIBLE_BLADE_AREA, LEAF_DEMAND,
                     LEAF_LAST_DEMAND, REALLOC_BIOMASS, SENESC_DW, SENESC_DW_SUM, LEAF_LEN,
                     INTERNODE_LAST_DEMAND, INTERNODE_DEMAND, INTERNODE_BIOMASS,
                     INTERNODE_LEN};

    enum externals { DD, DELTA_T, FTSW, FCSTR, PREDIM_LEAF_ON_MAINSTEM,
                     PREDIM_PREVIOUS_LEAF, SLA, PLANT_PHASE, TEST_IC,
                     PLANT_STATE};

    PhytomerModel(int index, bool is_on_mainstem, double plasto, double phyllo, double ligulo, double LL_BL, bool is_last_phytomer) :
        _index(index),
        _is_first_phytomer(index == 1),
        _plasto(plasto),
        _phyllo(phyllo),
        _ligulo(ligulo),
        _LL_BL(LL_BL),
        _is_last_phytomer(is_last_phytomer),
        _is_on_mainstem(is_on_mainstem),
        _internode_model(new InternodeModel(_index, _is_on_mainstem, _is_last_phytomer)),
        _leaf_model(new LeafModel(_index, _is_on_mainstem, _plasto, _phyllo, _ligulo, _LL_BL))
    {
        // submodels
        setsubmodel(LEAF, _leaf_model.get());
        setsubmodel(INTERNODE, _internode_model.get());

        // internals
        Internal(KILL_LEAF, &PhytomerModel::_kill_leaf);
        Internal(LEAF_PREDIM, &PhytomerModel::_leaf_predim);
        Internal(LEAF_BIOMASS, &PhytomerModel::_leaf_biomass);
        Internal(LEAF_BLADE_AREA, &PhytomerModel::_leaf_blade_area);
        Internal(LEAF_VISIBLE_BLADE_AREA, &PhytomerModel::_leaf_visible_blade_area);
        Internal(LEAF_DEMAND, &PhytomerModel::_leaf_demand);
        Internal(LEAF_LAST_DEMAND, &PhytomerModel::_leaf_last_demand);
        Internal(REALLOC_BIOMASS, &PhytomerModel::_realloc_biomass);
        Internal(SENESC_DW, &PhytomerModel::_senesc_dw);
        Internal(SENESC_DW_SUM, &PhytomerModel::_senesc_dw_sum);
        Internal(LEAF_LEN, &PhytomerModel::_leaf_len);
        Internal(INTERNODE_LAST_DEMAND, &PhytomerModel::_internode_last_demand);
        Internal(INTERNODE_DEMAND, &PhytomerModel::_internode_demand);
        Internal(INTERNODE_BIOMASS, &PhytomerModel::_internode_biomass);
        Internal(INTERNODE_LEN, &PhytomerModel::_internode_len);

        // externals
        External(PLANT_STATE, &PhytomerModel::_plant_state);
        External(PLANT_PHASE, &PhytomerModel::_plant_phase);
        External(TEST_IC, &PhytomerModel::_test_ic);
        External(FCSTR, &PhytomerModel::_fcstr);
        External(PREDIM_LEAF_ON_MAINSTEM, &PhytomerModel::_predim_leaf_on_mainstem);
        External(PREDIM_PREVIOUS_LEAF, &PhytomerModel::_predim_previous_leaf);
        External(FTSW, &PhytomerModel::_ftsw);
        External(DD, &PhytomerModel::_dd);
        External(DELTA_T, &PhytomerModel::_delta_t);
        External(SLA, &PhytomerModel::_sla);
    }

    virtual ~PhytomerModel()
    {
        _internode_model.reset(nullptr);
        _leaf_model.reset(nullptr);
    }

    void init(double t, const ecomeristem::ModelParameters& parameters)
    {
        // submodels
        _internode_model->init(t, parameters);
        _leaf_model->init(t, parameters);

        _kill_leaf = false;
        _leaf_predim = 0;
        _leaf_biomass = 0;
        _leaf_blade_area = 0;
        _leaf_visible_blade_area = 0;
        _leaf_demand = 0;
        _leaf_last_demand = 0;
        _realloc_biomass = 0;
        _senesc_dw = 0;
        _senesc_dw_sum = 0;
        _leaf_len = 0;
        _internode_last_demand = 0;
        _internode_demand = 0;
        _internode_biomass = 0;
        _internode_len = 0;

    }

    void compute(double t, bool /* update */)
    {
        if (_leaf_model) {
            _leaf_model->put(t, LeafModel::KILL_LEAF, _kill_leaf);
            _leaf_model->put(t, LeafModel::DELTA_T, _delta_t);
            _leaf_model->put(t, LeafModel::FTSW, _ftsw);
            _leaf_model->put(t, LeafModel::FCSTR, _fcstr);
            _leaf_model->put(t, LeafModel::LEAF_PREDIM_ON_MAINSTEM, _predim_leaf_on_mainstem);
            _leaf_model->put(t, LeafModel::PREVIOUS_LEAF_PREDIM, _predim_previous_leaf);
            _leaf_model->put(t, LeafModel::SLA, _sla);
            _leaf_model->put < plant::plant_state >(t, LeafModel::PLANT_STATE, _plant_state);
            _leaf_model->put(t, LeafModel::TEST_IC, _test_ic);
            (*_leaf_model)(t);
            _leaf_predim = _leaf_model->get< double >(t, LeafModel::LEAF_PREDIM);
            _leaf_biomass = _leaf_model->get< double >(t, LeafModel::BIOMASS);
            _leaf_blade_area = _leaf_model->get< double >(t, LeafModel::BLADE_AREA);
            _leaf_visible_blade_area = _leaf_model->get< double >(t, LeafModel::VISIBLE_BLADE_AREA);
            _leaf_demand = _leaf_model->get< double >(t, LeafModel::DEMAND);
            _leaf_last_demand = _leaf_model->get< double >(t, LeafModel::LAST_DEMAND);
            _realloc_biomass = _leaf_model->get< double >(t, LeafModel::REALLOC_BIOMASS);
            _senesc_dw = _leaf_model->get< double >(t, LeafModel::SENESC_DW);
            _senesc_dw_sum = _leaf_model->get< double >(t, LeafModel::SENESC_DW_SUM);
            _leaf_len = _leaf_model->get< double >(t, LeafModel::LEAF_LEN);
        }

        if(_internode_model) {
            _internode_model->put(t, InternodeModel::DELTA_T, _delta_t);
            _internode_model->put(t, InternodeModel::FTSW, _ftsw);
            _internode_model->put(t, InternodeModel::FCSTR, _fcstr);
            _internode_model->put < plant::plant_state >(t, InternodeModel::PLANT_STATE, _plant_state);
            _internode_model->put < plant::plant_phase >(t, InternodeModel::PLANT_PHASE, _plant_phase);
            _internode_model->put(t, InternodeModel::LIG, _leaf_model->get < double > (t, LeafModel::LIG_T));
            _internode_model->put(t, InternodeModel::LEAF_PREDIM, _leaf_model->get < double > (t, LeafModel::LEAF_PREDIM));
            _internode_model->put(t, InternodeModel::IS_LIG, _leaf_model->get < bool > (t, LeafModel::IS_LIG));
            _internode_model->put(t, InternodeModel::TEST_IC, _test_ic);
            (*_internode_model)(t);
            _internode_last_demand = _internode_model->get< double >(t, InternodeModel::LAST_DEMAND);
            _internode_demand = _internode_model->get< double >(t, InternodeModel::DEMAND);
            _internode_biomass = _internode_model->get< double >(t, InternodeModel::BIOMASS);
            _internode_len = _internode_model->get< double >(t, InternodeModel::INTERNODE_LEN);
        }
    }

    void kill_leaf(double t)
    { _kill_leaf = true; }

    LeafModel * leaf() const
    { return _leaf_model.get(); }

    InternodeModel * internode() const
    { return _internode_model.get(); }

    int get_index() const
    { return _index; }

    bool is_leaf_dead(double t) const
    { return _kill_leaf ||
                _leaf_model->get < bool >(t, LeafModel::IS_DEAD) ; }

    bool is_leaf_lig(double t) const {
        return !is_leaf_dead(t) &&
                _leaf_model->get < bool > (t, LeafModel::IS_LIG);
    }

    bool is_leaf_app(double t) const {
        return !is_leaf_dead(t) &&
                _leaf_model->get < bool > (t, LeafModel::IS_APP);
    }

    bool is_leaf_ligged(double t) const {
        return _leaf_model->get < bool >(t, LeafModel::IS_LIG);
    }

    bool is_leaf_apped(double t) const {
        return _leaf_model->get < bool >(t, LeafModel::IS_APP);
    }


private:
    //  attribute
    int _index;
    bool _is_first_phytomer;
    bool _is_on_mainstem;
    double _plasto;
    double _phyllo;
    double _ligulo;
    double _LL_BL;
    bool _is_last_phytomer;

    // submodels
    std::unique_ptr < InternodeModel > _internode_model;
    std::unique_ptr < LeafModel > _leaf_model;

    // internal
    bool _kill_leaf;
    double _leaf_predim;
    double _leaf_biomass;
    double _leaf_blade_area;
    double _leaf_visible_blade_area;
    double _leaf_demand;
    double _leaf_last_demand;
    double _realloc_biomass;
    double _senesc_dw;
    double _senesc_dw_sum;
    double _leaf_len;
    double _internode_last_demand;
    double _internode_demand;
    double _internode_biomass;
    double _internode_len;

    // external variables
    double _ftsw;
    plant::plant_state _plant_state;
    plant::plant_phase _plant_phase;
    double _fcstr;
    double _predim_leaf_on_mainstem;
    double _predim_previous_leaf;
    double _test_ic;
    double _dd;
    double _delta_t;
    double _sla;
};

} // namespace model
