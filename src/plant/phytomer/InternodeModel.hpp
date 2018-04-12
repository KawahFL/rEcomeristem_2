/**
 * @file ecomeristem/internode/Model.hpp
 * @author The Ecomeristem Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * Copyright (C) 2005-2017 Cirad http://www.cirad.fr
 * Copyright (C) 2012-2017 ULCO http://www.univ-littoral.fr
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

namespace model {

class InternodeModel : public AtomicModel < InternodeModel >
{
public:
    enum internals { INTERNODE_PHASE, INTERNODE_PHASE_1, INTERNODE_PREDIM, INTERNODE_LEN,
                     REDUCTION_INER, INER, EXP_TIME, INTER_DIAMETER,
                     VOLUME, BIOMASS, DEMAND, LAST_DEMAND, TIME_FROM_APP, /*CSTE_PLASTO,*/
                     CSTE_LIGULO, DENSITY_IN, POT_INER, GROWTH_DELAY, RED_LENGTH, POT_PREDIM, INDEX_};

    enum externals { PLANT_PHASE, PLANT_STATE, CULM_PHASE, LIG, IS_LIG, LEAF_PREDIM, FTSW,
                     DELTA_T, PLASTO, LIGULO, NB_LIG, CULM_DEFICIT, CULM_STOCK,
                     BOOL_CROSSED_PLASTO, LAST_LEAF_INDEX, PREVIOUS_IN_PREDIM, PHENOSTAGE, LIGSTAGE,
                     TEST_IC, FCSTR, FCSTRI, FCSTRL, CULM_NBLEAF_PARAM2 };

    InternodeModel(int index, bool is_on_mainstem, bool is_last_internode):
        _index(index),
        _is_on_mainstem(is_on_mainstem),
        _is_last_internode(is_last_internode)
    {
        Internal(INTERNODE_PHASE, &InternodeModel::_inter_phase);
        Internal(INTERNODE_PHASE_1, &InternodeModel::_inter_phase_1);
        Internal(INTERNODE_PREDIM, &InternodeModel::_inter_predim);
        Internal(INTERNODE_LEN, &InternodeModel::_inter_len);
        Internal(REDUCTION_INER, &InternodeModel::_reduction_iner);
        Internal(INER, &InternodeModel::_iner);
        Internal(EXP_TIME, &InternodeModel::_exp_time);
        Internal(INTER_DIAMETER, &InternodeModel::_inter_diameter);
        Internal(VOLUME, &InternodeModel::_inter_volume);
        Internal(BIOMASS, &InternodeModel::_biomass);
        Internal(DEMAND, &InternodeModel::_demand);
        Internal(LAST_DEMAND, &InternodeModel::_last_demand);
        Internal(TIME_FROM_APP, &InternodeModel::_time_from_app);
        Internal(CSTE_LIGULO, &InternodeModel::_cste_ligulo);
        Internal(DENSITY_IN, &InternodeModel::_density);
        Internal(POT_INER, &InternodeModel::_pot_iner);
        Internal(GROWTH_DELAY, &InternodeModel::_growth_delay);
        Internal(RED_LENGTH, &InternodeModel::_red_length);
        Internal(POT_PREDIM, &InternodeModel::_pot_predim);
        Internal(INDEX_, &InternodeModel::_index);

        External(PLANT_STATE, &InternodeModel::_plant_state);
        External(PLANT_PHASE, &InternodeModel::_plant_phase);
        External(CULM_PHASE, &InternodeModel::_culm_phase);
        External(LIG, &InternodeModel::_lig);
        External(IS_LIG, &InternodeModel::_is_lig);
        External(LEAF_PREDIM, &InternodeModel::_leaf_predim);
        External(FTSW, &InternodeModel::_ftsw);
        External(DELTA_T, &InternodeModel::_delta_t);
        External(PLASTO, &InternodeModel::_plasto);
        External(LIGULO, &InternodeModel::_ligulo);
        External(NB_LIG, &InternodeModel::_nb_lig);
        External(CULM_STOCK, &InternodeModel::_culm_stock);
        External(CULM_DEFICIT, &InternodeModel::_culm_deficit);
        External(BOOL_CROSSED_PLASTO, &InternodeModel::_bool_crossed_plasto);
        External(LAST_LEAF_INDEX, &InternodeModel::_last_leaf_index);
        External(PREVIOUS_IN_PREDIM, &InternodeModel::_previous_inter_predim);
        External(PHENOSTAGE, &InternodeModel::_phenostage);
        External(LIGSTAGE, &InternodeModel::_ligstage);
        External(TEST_IC, &InternodeModel::_test_ic);
        External(FCSTR, &InternodeModel::_fcstr);
        External(FCSTRI, &InternodeModel::_fcstrI);
        External(FCSTRL, &InternodeModel::_fcstrL);
        External(CULM_NBLEAF_PARAM2, &InternodeModel::_culm_nb_leaf_param2);
    }

    virtual ~InternodeModel()
    { }

    void compute(double t, bool /* update */){
        _p = _parameters.get(t).P;

        if(t == _first_day) {
            _cste_ligulo = _ligulo;
        }

        //InternodePredim
        if (_inter_phase == internode::VEGETATIVE and (_culm_phase == culm::ELONG or _culm_phase == culm::PI or _culm_phase == culm::PRE_FLO) and (_index >= _last_leaf_index) and _plant_phase != plant::FLO and _nb_lig > 0 and _is_lig) {
            if(_index <= _culm_nb_leaf_param2) {
                _inter_predim = std::max(1e-4, _leaf_length_to_IN_length * _leaf_predim );
            } else {
                if(_previous_inter_predim == 0) {
                    _inter_predim = std::max(1e-4, _leaf_length_to_IN_length * _leaf_predim );
                } else {
                    _inter_predim = std::max(1e-4, _previous_inter_predim * _slope_length_IN);
                }
            }
            _pot_predim = _inter_predim;
        }

        //growth deficit
        _inter_predim = std::max(0.,std::max(_inter_len,_inter_predim + _red_length));

        //ReductionINER
        if(_wbmodel == 2) {
            _reduction_iner = std::max(1e-4, (std::min(1.,_fcstrL * (1. + (_p * _respINER)))) * _test_ic);
        } else {
            if (_ftsw < _thresINER) {
                _reduction_iner = std::max(1e-4, ((1./_thresINER) * _ftsw) * (1. + (_p * _respINER)) * _test_ic);
            } else {
                _reduction_iner = 1. + _p * _respINER * _test_ic;
            }
        }

        //INER
        if(_is_last_internode) {
            _iner = _inter_predim * _reduction_iner / (_cste_ligulo);

        } else {
            _iner = _inter_predim * _reduction_iner / (3*_cste_ligulo);
        }

        //growth deficit
        _pot_iner = _iner / _reduction_iner;
        _growth_delay = std::min(_delta_t, _exp_time * _reduction_iner) * (-1. + _reduction_iner);
        if((_fcstrL < 1 or _fcstr < 1)) {
            _red_length = (_growth_delay * _pot_iner) * (1-_fcstrI);
        } else {
            _red_length = 0;
        }


        //InternodeManager
        step_state(t);

        //InternodeLen & InternodeExpTime
        if (_inter_phase == internode::VEGETATIVE) {
            _inter_len = 0;
            _exp_time = 0;
        } else {
            if (_inter_phase_1 == internode::VEGETATIVE and _inter_phase == internode::REALIZATION) {
                _inter_len = _iner * _delta_t;
                _exp_time = (_inter_predim - _inter_len) / _iner;
            } else {
                if (!(_plant_state & plant::NOGROWTH) and (_culm_deficit + _culm_stock >= 0) and (_plant_phase == plant::ELONG or _plant_phase == plant::PI or _plant_phase == plant::PRE_FLO or _plant_phase == plant::FLO)) {
                    _exp_time = (_inter_predim - _inter_len) / _iner;
                    _inter_len = std::min(_inter_predim, _inter_len + _iner * std::min(_delta_t, _exp_time));
                }
            }
        }

        //DiameterPredim
        _inter_diameter = _IN_length_to_IN_diam * std::max(0.,(_index - _nb_leaf_stem_elong + 1)) + _coef_lin_IN_diam;

        //Volume
        double radius = _inter_diameter / 2;
        _inter_volume = _inter_len * 3.141592653589793238462643383280 * radius * radius;

        //Density per IN
        _density = std::min(_density_IN2, _density + ((_density_IN2-_density_IN1)/(3*_cste_ligulo) * _delta_t));
        //whole plant
        //_density = std::min(_density_IN2, _density_IN1 + std::max(0., (_ligstage - _nb_leaf_stem_elong)) * ((_density_IN2 - _density_IN1)/((_maxleaves + 4) - _nb_leaf_stem_elong)));


        //Biomass
        double biomass_1 = _biomass;
        if(_culm_deficit + _culm_stock >= 0) {
            _biomass = _inter_volume * _density;
        }

        //InternodeDemand & InternodeLastDemand
        _last_demand = 0;
        if (_inter_len >= _inter_predim) {
            _demand = 0;
            if (! _is_mature) {
                _last_demand = _biomass - biomass_1;
                _is_mature = true;
            }
        } else {
            _demand = _biomass - biomass_1;
        }

        //InternodeTimeFromApp
        if(t == _first_day) {
            _time_from_app = _delta_t;
        } else {
            if (!(_plant_state & plant::NOGROWTH) and (_culm_deficit + _culm_stock >= 0)) {
                _time_from_app = _time_from_app + _delta_t;
            }
        }
    }

    void culm_dead(double t) {
        _inter_phase_1 = _inter_phase;
        _inter_phase = internode::DEAD;
        _inter_len = 0;
        _reduction_iner = 0;
        _iner = 0;
        _exp_time = 0;
        _inter_diameter = 0;
        _inter_volume = 0;
        _biomass = 0;
        _demand = 0;
        _last_demand = 0;
        _first_day = t;
        _time_from_app = 0;
        _is_mature = false;
    }

    void step_state(double t) {
        _inter_phase_1 = _inter_phase;

        switch (_inter_phase) {
        case internode::INITIAL:
            _inter_phase = internode::VEGETATIVE;
            break;
        case internode::VEGETATIVE:
            if((_culm_phase == culm::ELONG or _culm_phase == culm::PI or _culm_phase == culm::PRE_FLO) and (_index >= _last_leaf_index) and _plant_phase != plant::FLO and _nb_lig > 0 and _is_lig) {
                _inter_phase = internode::REALIZATION;
            }
            break;
        case internode::REALIZATION:
            if(_inter_len >= _inter_predim) {
                _inter_phase = internode::MATURITY;
            }
            break;
        }
    }

    void init(double t,
              const ecomeristem::ModelParameters& parameters) {
        _parameters = parameters;
        //parameters
        _slope_length_IN = parameters.get("slope_length_IN");
        _leaf_length_to_IN_length = parameters.get("leaf_length_to_IN_length");
        _nb_leaf_param2 = parameters.get("nb_leaf_param2");
        _thresINER = parameters.get("thresINER");
        _respINER = parameters.get("resp_LER");
        _slopeINER = parameters.get("slopeINER");
        _IN_length_to_IN_diam =
                parameters.get("IN_length_to_IN_diam");
        _coef_lin_IN_diam = parameters.get("coef_lin_IN_diam");
        _density_IN1 = parameters.get("density_IN1");
        _density_IN2 = parameters.get("density_IN2");
        _coeff_species = parameters.get("coeff_species");
        _nb_leaf_stem_elong = parameters.get("nb_leaf_stem_elong");
        _phenostage_pre_flo_to_flo = parameters.get("phenostage_PRE_FLO_to_FLO");
        _wbmodel = parameters.get("wbmodel");
        _maxleaves = parameters.get("maxleaves");

        //internals
        _inter_phase = internode::INITIAL;
        _inter_phase_1 = internode::INITIAL;
        _inter_len = 0;
        _reduction_iner = 0;
        _iner = 0;
        _exp_time = 0;
        _inter_diameter = 0;
        _inter_volume = 0;
        _biomass = 0;
        _demand = 0;
        _last_demand = 0;
        _first_day = t;
        _time_from_app = 0;
        _inter_predim = 0;
        _is_mature = false;
        _cste_ligulo = 0;
        _density = 0;
        _pot_iner = 0;
        _growth_delay = 0;
        _red_length = 0;
        _pot_predim = 0;
    }

private:
    ecomeristem::ModelParameters _parameters;
    // attributes
    int _index;
    bool _is_on_mainstem;
    bool _is_last_internode;

    // parameters
    double _phenostage_pre_flo_to_flo;
    double _nb_leaf_param2;
    double _slope_length_IN;
    double _leaf_length_to_IN_length;
    double _thresINER;
    double _respINER;
    double _slopeINER;
    double _IN_length_to_IN_diam;
    double _coef_lin_IN_diam;
    double _coeff_species;
    double _nb_leaf_stem_elong;
    double _density_IN1;
    double _density_IN2;
    double _wbmodel;
    double _maxleaves;
    double _p;

    // internals
    double _density;
    internode::internode_phase _inter_phase;
    internode::internode_phase _inter_phase_1;
    double _inter_len;
    double _inter_predim;
    double _reduction_iner;
    double _iner;
    double _exp_time;
    double _inter_diameter;
    double _inter_volume;
    double _biomass;
    double _demand;
    double _last_demand;
    double _time_from_app;
    double _first_day;
    bool _is_mature;
    double _cste_ligulo;
    double _pot_iner;
    double _growth_delay;
    double _red_length;
    double _pot_predim;

    // externals
    plant::plant_state _plant_state;
    plant::plant_phase _plant_phase;
    culm::culm_phase _culm_phase;
    double _leaf_predim;
    double _lig;
    bool _is_lig;
    double _ftsw;
    double _delta_t;
    double _ligulo;
    double _plasto;
    double _nb_lig;
    double _culm_deficit;
    double _culm_stock;
    double _bool_crossed_plasto;
    double _last_leaf_index;
    double _previous_inter_predim;
    int _phenostage;
    int _ligstage;
    double _test_ic;
    double _fcstr;
    double _fcstrI;
    double _fcstrL;
    double _culm_nb_leaf_param2;
};

} // namespace model
