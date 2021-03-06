/**
 * @file ecomeristem/plant/thermal-time/Model.hpp
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


#ifndef ICT_MODEL_HPP
#define ICT_MODE_HPP

#include <defines.hpp>

namespace model {

class IctModel : public AtomicModel < IctModel >
{
public:    
    enum internals { IS_COMPUTED, SURVIVED, IC_1, IC };
    enum externals { BOOL_CROSSED_PHYLLO, DAY_DEMAND, SEEDRES, SUPPLY };


    IctModel() {
        //    computed variables
        Internal(IS_COMPUTED, &IctModel::_is_computed);
        Internal(SURVIVED, &IctModel::_survived);
        Internal(IC_1, &IctModel::_ic_1);
        Internal(IC, &IctModel::_ic);

        //    external variables
        External(BOOL_CROSSED_PHYLLO, &IctModel::_bool_crossed_phyllo);
        External(DAY_DEMAND, &IctModel::_day_demand);
        External(SEEDRES, &IctModel::_seed_res);
        External(SUPPLY, &IctModel::_supply);
    }

    virtual ~IctModel()
    {}

    void compute(double t, bool /* update */) {
        if (t != _parameters.beginDate) {
            if(_is_computed) {
                _seed_res_ = _seed_res_;
                _day_demand_ = _day_demand_;
                _supply_ = _supply_;
                _ic_ = _ic_;
                _ic = _ic;
                _survived = _survived;
                _is_computed = _is_computed;
                return;
            } else {
                if(_bool_crossed_phyllo >= 0) {
                    //test si on va survivre
                    if(_ic > Ict) {
                        _survived = true;
                    } else {
                        _survived = false;
                    }
                    _is_computed = true;
                } else {
                    //declaration variables
                    double total = 0;
                    double n = 0;
                    double mean = 0;
                    _ic_1 = _ic;

                    //ajout des valeurs du jour
                    _seed_res_.push_back(_seed_res);
                    _day_demand_.push_back(_day_demand);
                    _supply_.push_back(_supply);

                    //calcul ic
                    for(int i = 0; i < _seed_res_.size(); i++) {
                        if(_day_demand != 0) {
                            _ic_.push_back((std::max(0., _seed_res_[i]) + _supply_[i]) /_day_demand_[i]);
                            total += _ic_[i];
                            ++n;
                        }
                    }
                    if(n != 0) {
                        mean = total/n;
                    } else {
                        mean = _ic_1;
                    }

                    double tmp = std::min(5., mean);
                    if(tmp == 0) {
                        _ic = 0.0001;
                    } else {
                        _ic = tmp;
                    }
                }
            }
        }
    }

    void init(double t, const ecomeristem::ModelParameters& parameters) {
        //parameters
        _parameters = parameters;
        Ict = _parameters.get("Ict");

        //internals
        _is_computed = false;
        _survived = true;
        _ic_1 = 0.0001;
        _ic = 0.0001;
    }

private:
    ecomeristem::ModelParameters _parameters;
    //parameters
    double Ict;

    //internals
    bool _is_computed;
    bool _survived;

    double _ic_1;
    double _ic;

    std::vector<double> _seed_res_;
    std::vector<double> _day_demand_;
    std::vector<double> _supply_;
    std::vector<double> _ic_;


    //externals
    double _bool_crossed_phyllo;
    double _day_demand;
    double _seed_res;
    double _supply;


};

} // namespace model
#endif
