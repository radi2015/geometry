#ifndef BOOST_GEOMETRY_PROJECTIONS_NATEARTH_HPP
#define BOOST_GEOMETRY_PROJECTIONS_NATEARTH_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 4.9.1

// Original copyright notice:

// The Natural Earth projection was designed by Tom Patterson, US National Park
// Service, in 2007, using Flex Projector. The shape of the original projection
// was defined at every 5 degrees and piece-wise cubic spline interpolation was
// used to compute the complete graticule.
// The code here uses polynomial functions instead of cubic splines and
// is therefore much simpler to program. The polynomial approximation was
// developed by Bojan Savric, in collaboration with Tom Patterson and Bernhard
// Jenny, Institute of Cartography, ETH Zurich. It slightly deviates from
// Patterson's original projection by adding additional curvature to meridians
// where they meet the horizontal pole line. This improvement is by intention
// and designed in collaboration with Tom Patterson.
// Port to PROJ.4 by Bernhard Jenny, 6 June 2011

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>

namespace boost { namespace geometry { namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace natearth
    {

            static const double A0 = 0.8707;
            static const double A1 = -0.131979;
            static const double A2 = -0.013791;
            static const double A3 = 0.003971;
            static const double A4 = -0.001529;
            static const double B0 = 1.007226;
            static const double B1 = 0.015085;
            static const double B2 = -0.044475;
            static const double B3 = 0.028874;
            static const double B4 = -0.005916;
            static const double C0 = B0;
            static const double C1 = (3 * B1);
            static const double C2 = (7 * B2);
            static const double C3 = (9 * B3);
            static const double C4 = (11 * B4);
            static const double EPS = 1e-11;
            static const double MAX_Y = (0.8707 * 0.52 * geometry::math::pi<double>());

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_natearth_spheroid : public base_t_fi<base_natearth_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;


                inline base_natearth_spheroid(const Parameters& par)
                    : base_t_fi<base_natearth_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                // FORWARD(s_forward)  spheroid
                // Project coordinates from geographic (lon, lat) to cartesian (x, y)
                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double phi2, phi4;

                    phi2 = lp_lat * lp_lat;
                    phi4 = phi2 * phi2;
                    xy_x = lp_lon * (A0 + phi2 * (A1 + phi2 * (A2 + phi4 * phi2 * (A3 + phi2 * A4))));
                    xy_y = lp_lat * (B0 + phi2 * (B1 + phi4 * (B2 + B3 * phi2 + B4 * phi4)));
                }

                // INVERSE(s_inverse)  spheroid
                // Project coordinates from cartesian (x, y) to geographic (lon, lat)
                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double yc, tol, y2, y4, f, fder;

                        /* make sure y is inside valid range */
                    if (xy_y > MAX_Y) {
                        xy_y = MAX_Y;
                    } else if (xy_y < -MAX_Y) {
                        xy_y = -MAX_Y;
                    }

                        /* latitude */
                    yc = xy_y;
                        for (;;) { /* Newton-Raphson */
                        y2 = yc * yc;
                        y4 = y2 * y2;
                        f = (yc * (B0 + y2 * (B1 + y4 * (B2 + B3 * y2 + B4 * y4)))) - xy_y;
                        fder = C0 + y2 * (C1 + y4 * (C2 + C3 * y2 + C4 * y4));
                        yc -= tol = f / fder;
                        if (fabs(tol) < EPS) {
                            break;
                        }
                    }
                    lp_lat = yc;

                        /* longitude */
                    y2 = yc * yc;
                    lp_lon = xy_x / (A0 + y2 * (A1 + y2 * (A2 + y2 * y2 * y2 * (A3 + y2 * A4))));
                }
            };

            // Natural Earth
            template <typename Parameters>
            void setup_natearth(Parameters& par)
            {
                par.es = 0;
            }

        }} // namespace detail::natearth
    #endif // doxygen

    /*!
        \brief Natural Earth projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
        \par Example
        \image html ex_natearth.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct natearth_spheroid : public detail::natearth::base_natearth_spheroid<Geographic, Cartesian, Parameters>
    {
        inline natearth_spheroid(const Parameters& par) : detail::natearth::base_natearth_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::natearth::setup_natearth(this->m_par);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class natearth_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<natearth_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void natearth_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("natearth", new natearth_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_NATEARTH_HPP

