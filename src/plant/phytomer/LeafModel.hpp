/**
 * @file ecomeristem/leaf/Model.hpp
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

class LeafModel : public AtomicModel < LeafModel >
{
public:
    enum internals { LEAF_PHASE, LIFE_SPAN, REDUCTION_LER, LEAF_LEN, LER,
                     EXP_TIME, LEAF_PREDIM, WIDTH,
                     TT_LIG, BLADE_AREA, VISIBLE_BLADE_AREA, BIOMASS, DEMAND, LAST_DEMAND,
                     REALLOC_BIOMASS, SENESC_DW, SENESC_DW_SUM,
                     TIME_FROM_APP, LIG_T, IS_LIG, IS_LIG_T, OLD_BIOMASS,
                     LAST_LEAF_BIOMASS, SLA_CSTE, LL_BL_, PLASTO, PHYLLO, LIGULO, FIRST_DAY,
                     SHEATH_LEN, LAST_BLADE_AREA, LAST_VISIBLE_BLADE_AREA, SHEATH_LLL_CST,
                     IS_APP, IS_DEAD, IS_FIRST, GROWTH_DELAY, POT_LER, RED_LENGTH, POT_PREDIM,
                     WIDTH_LER, POT_LEN, BLADE_LEN };

    enum externals { DELTA_T, FTSW, FCSTR, FCSTRL, FCSTRLLEN,
                     LEAF_PREDIM_ON_MAINSTEM, PREVIOUS_LEAF_PREDIM,
                     SLA, PLANT_STATE, TEST_IC, MGR, KILL_LEAF, CULM_DEFICIT, CULM_STOCK, SHEATH_LLL,
                     CULM_NBLEAF_PARAM2, PLASTO_NBLEAF_PARAM2, PREDIM_APP_LEAF_MS };


    virtual ~LeafModel()
    { }

    LeafModel(int index, bool is_on_mainstem, double plasto, double phyllo, double ligulo, double LL_BL) :
        _index(index),
        _is_first_leaf(_index == 1),
        _is_on_mainstem(is_on_mainstem),
        _plasto(plasto),
        _phyllo(phyllo),
        _ligulo(ligulo),
        _LL_BL(LL_BL)
    {
        //internals
        Internal(LEAF_PHASE, &LeafModel::_leaf_phase);
        Internal(LEAF_PREDIM, &LeafModel::_predim);
        Internal(LEAF_LEN, &LeafModel::_len);
        Internal(LIFE_SPAN, &LeafModel::_life_span);
        Internal(REDUCTION_LER, &LeafModel::_reduction_ler);
        Internal(LER, &LeafModel::_ler);
        Internal(EXP_TIME, &LeafModel::_exp_time);
        Internal(WIDTH, &LeafModel::_width);
        Internal(TT_LIG, &LeafModel::_TT_Lig);
        Internal(BLADE_AREA, &LeafModel::_blade_area);
        Internal(VISIBLE_BLADE_AREA, &LeafModel::_visible_blade_area);
        Internal(BIOMASS, &LeafModel::_biomass);
        Internal(REALLOC_BIOMASS, &LeafModel::_realloc_biomass);
        Internal(SENESC_DW, &LeafModel::_senesc_dw);
        Internal(SENESC_DW_SUM, &LeafModel::_senesc_dw_sum);
        Internal(DEMAND, &LeafModel::_demand);
        Internal(LAST_DEMAND, &LeafModel::_last_demand);
        Internal(TIME_FROM_APP, &LeafModel::_time_from_app);
        Internal(LIG_T, &LeafModel::_lig_t);
        Internal(IS_LIG, &LeafModel::_is_lig);
        Internal(IS_LIG_T, &LeafModel::_is_lig_t);
        Internal(OLD_BIOMASS, &LeafModel::_old_biomass);
        Internal(LAST_LEAF_BIOMASS, &LeafModel::_last_leaf_biomass);
        Internal(SLA_CSTE, &LeafModel::_sla_cste);
        Internal(LL_BL_, &LeafModel::_LL_BL);
        Internal(PLASTO, &LeafModel::_plasto);
        Internal(PHYLLO, &LeafModel::_phyllo);
        Internal(LIGULO, &LeafModel::_ligulo);
        Internal(FIRST_DAY, &LeafModel::_first_day);
        Internal(SHEATH_LEN, &LeafModel::_sheath_len);
        Internal(LAST_BLADE_AREA, &LeafModel::_last_blade_area);
        Internal(LAST_VISIBLE_BLADE_AREA, &LeafModel::_last_visible_blade_area);
        Internal(SHEATH_LLL_CST, &LeafModel::_sheath_LLL_cst);
        Internal(IS_APP, &LeafModel::_is_app);
        Internal(IS_DEAD, &LeafModel::_is_dead);
        Internal(IS_FIRST, &LeafModel::_firstl);
        Internal(GROWTH_DELAY, &LeafModel::_growth_delay);
        Internal(POT_LER, &LeafModel::_pot_ler);
        Internal(RED_LENGTH, &LeafModel::_red_length);
        Internal(POT_PREDIM, &LeafModel::_pot_predim);
        Internal(WIDTH_LER, &LeafModel::_width_ler);
        Internal(POT_LEN, &LeafModel::_pot_len);
        Internal(BLADE_LEN, &LeafModel::_blade_len);

        //externals
        External(PLANT_STATE, &LeafModel::_plant_state);
        External(TEST_IC, &LeafModel::_test_ic);
        External(FCSTR, &LeafModel::_fcstr);
        External(FCSTRL, &LeafModel::_fcstrL);
        External(FCSTRLLEN, &LeafModel::_fcstrLlen);
        External(LEAF_PREDIM_ON_MAINSTEM, &LeafModel::_predim_leaf_on_mainstem);
        External(PREVIOUS_LEAF_PREDIM, &LeafModel::_predim_previous_leaf);
        External(PREDIM_APP_LEAF_MS, &LeafModel::_predim_app_leaf_on_mainstem);
        External(FTSW, &LeafModel::_ftsw);
        External(DELTA_T, &LeafModel::_delta_t);
        External(SLA, &LeafModel::_sla);
        External(MGR, &LeafModel::_MGR);
        External(KILL_LEAF, &LeafModel::_kill_leaf);
        External(CULM_DEFICIT, &LeafModel::_culm_deficit);
        External(CULM_STOCK, &LeafModel::_culm_stock);
        External(SHEATH_LLL, &LeafModel::_sheath_LLL);
        External(CULM_NBLEAF_PARAM2, &LeafModel::_culm_nbleaf_param2);
        External(PLASTO_NBLEAF_PARAM2, &LeafModel::_plasto_nbleaf_param2);
    }

    void compute(double t, bool /* update */)
    {
        if(_kill_leaf or _leaf_phase == leaf::DEAD) {
            _leaf_phase = leaf::DEAD;
            _is_dead = true;
            _realloc_biomass = 0;
            _life_span = 0;
            _reduction_ler = 1.;
            _ler = 0;
            _exp_time = 0;
            _len = 0;
            _width = 0;
            _TT_Lig = 0;
            _blade_area = 0;
            _biomass = 0;
            _old_biomass = 0;
            _demand = 0;
            _last_demand = 0;
            _time_from_app = 0;
            _last_blade_area = 0;
            _last_leaf_biomass = 0;
            _visible_blade_area = 0;
            _senesc_dw = 0;
            _blade_len = 0;
            _is_lig = _is_lig;
            _is_app = _is_app;
            return;
        }
        _p = _parameters.get(t).P;

        if(t == _first_day) {
            if(_is_first_leaf) {
                _firstl = true;
            }
            //life span
            _life_span = _coeffLifespan * std::exp(_mu * _index);
            if(_is_first_leaf && _is_on_mainstem) {
                _sheath_LLL_cst = 0;
            } else {
                _sheath_LLL_cst = _sheath_LLL;
            }

            //Leaf predim
            if (_is_first_leaf and _is_on_mainstem) {
                _predim = _Lef1;
            } else if (not _is_first_leaf and _is_on_mainstem) {
                _predim =  _predim_leaf_on_mainstem + _MGR * _test_ic * _fcstr;
            } else if (_is_first_leaf and not _is_on_mainstem) {
                _predim = 0.5 * (_predim_app_leaf_on_mainstem + _Lef1) *
                        _test_ic * _fcstr;
            } else {
                _predim = 0.5 * (_predim_leaf_on_mainstem +
                                 _predim_previous_leaf) +
                        _MGR * _test_ic * _fcstr;
            }
            _pot_predim = _predim;
        }

        //growth deficit
        _predim = std::max(0.,std::max(_len,_predim + _red_length));

        //ReductionLER
        if(t == _first_day && _is_first_leaf && _is_on_mainstem) {
            _reduction_ler = 1.;
        } else {
            if(_wbmodel == 2) {
                _reduction_ler = std::max(1e-4, (std::min(1.,_fcstrL * (1. + (_p * _respLER))))* _test_ic);
            } else {
                if (_ftsw < _thresLER) {
                    _reduction_ler = std::max(1e-4, ((1. / _thresLER) * _ftsw) * (1. + (_p * _respLER))* _test_ic);
                } else {
                    _reduction_ler = 1. + _p * _respLER * _test_ic;
                }
            }
        }

        //LER & exp time
        //leaves already grown with a specific plasto/phyllo/ligulo or the initial plasto/phyllo/ligulo
        double tmp1 = std::max(0., _index-(_plasto_nbleaf_param2-1));
        double tmp2 = std::max(0., _index-(_culm_nbleaf_param2-1));
        if (_leaf_phase == leaf::INITIAL) {
            if(_is_first_leaf) {
                if(_index <= _culm_nbleaf_param2) {
                    _ler = (_sheath_LLL_cst/_phyllo_init)*_reduction_ler;
                } else {
                    _ler = (_sheath_LLL_cst/_phyllo)*_reduction_ler;
                }
                _exp_time = (_sheath_LLL_cst-_len)/_ler;
            } else {
                double time = (((_index-tmp2-1)*_phyllo_init)+(tmp2*_phyllo))-(((std::max(0.,_index-tmp1-_nbinitleaves))*_plasto_init)+(tmp1*_plasto));
                _ler = (_sheath_LLL_cst/time)*_reduction_ler;
                _exp_time = (_sheath_LLL_cst-_len)/_ler;
            }
            _width_ler = _ler;
        } else {
            double time = (((_index-tmp2)*_ligulo_init)+(tmp2*_ligulo))-(((_index-tmp2-1)*_phyllo_init)+(tmp2*_phyllo));
            _ler = ((_predim-_sheath_LLL_cst)/time)*_reduction_ler;
            _width_ler = ((_pot_predim-_sheath_LLL_cst)/time)*_reduction_ler;
            _exp_time = (_predim-_len)/_ler;
        }

        //growth deficit
        _pot_ler = _ler / _reduction_ler;
        _growth_delay = std::min(_delta_t, _exp_time * _reduction_ler) * (-1. + _reduction_ler);
        if((_fcstrL < 1 or _fcstr < 1)) {
            _red_length = (_growth_delay * _pot_ler) * (1-_fcstrLlen);
        } else {
            _red_length = 0;
        }

        //LeafLen
        if (!(_plant_state & plant::NOGROWTH) and (_culm_deficit + _culm_stock >= 0)) {
            if(_leaf_phase == leaf::INITIAL) {
                _len = std::min(_sheath_LLL_cst, _len+_ler*std::min(_delta_t, _exp_time));
                _pot_len = _len;
            } else {
                _len = std::min(_predim, _len+_ler*std::min(_delta_t, _exp_time));
                _pot_len = std::min(_pot_predim, _pot_len+_width_ler*std::min(_delta_t, _exp_time));
            }
        }

        //LeafManager
        step_state();

        //Width
        _width = _pot_len * _WLR / _LL_BL;

        //ThermalTimeSinceLigulation
        _is_lig_t = false;
        if (not _is_lig) {
            if(_leaf_phase == leaf::LIG) {
                _is_lig = true;
                _is_lig_t = true;
                if(_lig_t == 0) {
                    _lig_t = t;
                }
            }
        } else {
            _TT_Lig += _delta_t;
        }

        //Sheath and blade length
        _sheath_len = (1 - (1 / _LL_BL)) * _len;
        _blade_len = _len - _sheath_len;

        //BladeArea and VisibleBladeArea : consider only leaves already appeared for PAI
        _blade_area = _len * _width * _allo_area / _LL_BL;
        if (not _is_lig || _is_lig_t) {
            if(_leaf_phase == leaf::INITIAL) {
                _visible_blade_area = 0;
            } else {
                if(_sheath_len < _sheath_LLL_cst) {
                    _visible_blade_area = std::max(0.,_len - _sheath_LLL_cst) * _width * _allo_area;
                } else {
                    _visible_blade_area = _blade_area;
                }
            }
            if(_is_lig_t) {
                _last_blade_area = _blade_area;
                _last_visible_blade_area = _visible_blade_area;
            }
        } else {
            _blade_area = std::max(0.,_last_blade_area * (1 - _TT_Lig / _life_span));
            _visible_blade_area = _blade_area;
        }


        //Biomass
        _old_biomass = _biomass;
        if (_first_day == t) {
            _biomass = (1. / _G_L) * _blade_area / _sla;
            _realloc_biomass = 0;
            _sla_cste = _sla;
        } else {
            if ((!(_plant_state & plant::NOGROWTH) and (_culm_deficit + _culm_stock >= 0)) or (_is_lig and !(_is_lig_t))) {
                if (not _is_lig || _is_lig_t) {
                    _biomass = (1. / _G_L) * _blade_area / _sla_cste;
                    _realloc_biomass = 0;
                    if (_is_lig_t) {
                        _last_leaf_biomass = _biomass;
                    }
                } else {
                    _biomass = std::max(0.,_last_leaf_biomass * (1. - _TT_Lig / _life_span));
                    double delta_biomass = _old_biomass - _biomass;
                    _realloc_biomass = delta_biomass * _realocationCoeff;
                    _senesc_dw = delta_biomass * (1 - _realocationCoeff);
                    _senesc_dw_sum = _senesc_dw_sum + _senesc_dw;
                }
            }
        }

        //LeafDemand
        if (_first_day == t) {
            _demand = _biomass;
        } else {
            if (not _is_lig) {
                _demand = _biomass - _old_biomass;
            } else {
                _demand = 0;
            }
        }

        // LeafLastDemand
        if (_is_lig_t) {
            _last_demand = _biomass - _old_biomass;
        } else {
            _last_demand = 0;
        }

        //LeafTimeFromApp
        if (_first_day == t) {
            _time_from_app = _delta_t;
        } else {
            if (!(_plant_state & plant::NOGROWTH) and (_culm_deficit + _culm_stock >= 0)) {
                _time_from_app = _time_from_app + _delta_t;
            }
        }

        if(_biomass == 0) {
            _is_dead = true;
            _leaf_phase = leaf::DEAD;
        }

        if(_demand < 0) {
            _demand = _demand;
        }
    }

    void step_state() {
        switch (_leaf_phase) {
        case leaf::INITIAL:
            if(_len >= _sheath_LLL_cst) {
                _leaf_phase = leaf::VEGETATIVE;
                _is_app = true;
            }
            break;
        case leaf::VEGETATIVE:
            if(_len >= _predim && !(_plant_state & plant::NOGROWTH)) {
                _leaf_phase = leaf::LIG;
                _is_app = false;
            }
            break;
        }
    }

    void init(double t,
              const ecomeristem::ModelParameters& parameters)
    {
        last_time = t-1;
        _parameters = parameters;

        //parameters
        _coeffLifespan = parameters.get("coeff_lifespan");
        _mu = parameters.get("mu");
        _Lef1 = parameters.get("Lef1");
        _thresLER = parameters.get("thresLER");
        _respLER = parameters.get("resp_LER");
        _WLR = parameters.get("WLR");
        _allo_area = parameters.get("allo_area");
        _G_L = parameters.get("G_L");
        _realocationCoeff = parameters.get("realocationCoeff");
        _nbinitleaves = parameters.get("nbinitleaves");
        _wbmodel = parameters.get("wbmodel");
        _phyllo_init = parameters.get("phyllo_init");
        _plasto_init = parameters.get("plasto_init");
        _ligulo_init = parameters.get("ligulo_init");

        //internals
        _realloc_biomass = 0;
        _first_day = t;
        _life_span = 0;
        if(_is_first_leaf && _is_on_mainstem) {
            _leaf_phase = leaf::VEGETATIVE;
            _is_app = true;
        } else {
            _leaf_phase = leaf::INITIAL;
            _is_app = false;
        }
        _predim = 0;
        _reduction_ler = 1.;
        _ler = 0;
        _exp_time = 0;
        _len = 0;
        _width = 0;
        _TT_Lig = 0;
        _is_lig = false;
        _is_lig_t = false;
        _blade_area = 0;
        _visible_blade_area = 0;
        _biomass = 0;
        _old_biomass = 0;
        _senesc_dw = 0;
        _senesc_dw_sum = 0;
        _demand = 0;
        _last_demand = 0;
        _time_from_app = 0;
        _lig_t = 0;
        _last_blade_area = 0;
        _last_visible_blade_area = 0;
        _last_leaf_biomass = 0;
        _sla_cste = 0;
        _sheath_len = 0;
        _sheath_LLL_cst = 0;
        _is_dead = false;
        _firstl = (_index == 1);
        _pot_ler = 0;
        _growth_delay = 0;
        _red_length = 0;
        _pot_predim = 0;
        _width_ler = 0;
        _pot_len = 0;
        _blade_len = 0;
    }

private:
    ecomeristem::ModelParameters _parameters;
    // parameters
    double _coeffLifespan;
    double _mu;
    double _Lef1;
    double _thresLER;
    double _respLER;
    double _WLR;
    double _allo_area;
    double _G_L;
    double _realocationCoeff;
    double _nbinitleaves;
    double _wbmodel;
    double _phyllo_init;
    double _plasto_init;
    double _ligulo_init;
    double _p;

    // attributes
    int _index;
    bool _is_first_leaf;
    bool _is_on_mainstem;
    double _plasto;
    double _phyllo;
    double _ligulo;
    double _LL_BL;

    // internal variable
    double _width;
    leaf::leaf_phase _leaf_phase;
    double _predim;
    double _first_day;
    double _life_span;
    double _reduction_ler;
    double _ler;
    double _exp_time;
    double _len;
    double _TT_Lig;
    bool _is_lig;
    bool _is_lig_t;
    bool _is_app;
    double _blade_area;
    double _visible_blade_area;
    double _biomass;
    double _realloc_biomass;
    double _old_biomass;
    double _senesc_dw;
    double _senesc_dw_sum;
    double _demand;
    double _last_demand;
    double _time_from_app;
    double _sla_cste;
    double _lig_t;
    double _last_blade_area;
    double _last_visible_blade_area;
    double _last_leaf_biomass;
    double _sheath_len;
    double _sheath_LLL_cst;
    bool _is_dead;
    bool _firstl;
    double _pot_ler;
    double _growth_delay;
    double _red_length;
    double _pot_predim;
    double _width_ler;
    double _pot_len;
    double _blade_len;


    // external variables
    double _MGR;
    double _ftsw;
    plant::plant_state _plant_state;
    double _fcstr;
    double _fcstrL;
    double _predim_leaf_on_mainstem;
    double _predim_previous_leaf;
    double _predim_app_leaf_on_mainstem;
    double _test_ic;
    double _delta_t;
    double _sla;
    bool _kill_leaf;
    double _culm_deficit;
    double _culm_stock;
    double _sheath_LLL;
    double _culm_nbleaf_param2;
    double _plasto_nbleaf_param2;
    double _fcstrLlen;
};

} // namespace model
