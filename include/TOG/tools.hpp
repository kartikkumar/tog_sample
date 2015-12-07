/*
 * Copyright (c) 2014-2015 Kartik Kumar (me@kartikkumar.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef TOG_TOOLS_HPP
#define TOG_TOOLS_HPP

#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <string>

#include <catch.hpp>

#include <rapidjson/document.h>

#include "TOG/typedefs.hpp"

namespace tog
{

//! Create custom CATCH Approx object with tolerance for comparing doubles.
/*!
 * Creates a custom CATCH Approx object that can be used for comparing doubles in unit tests. The
 * tolerance is set based on machine precision (epsilon) for doubles.
 *
 * Using this comparison object ensures that large numbers are compared in a relative sense and
 * small numbers in an absolute sense.
 *
 * @see <a href="https://github.com/philsquared/Catch/include/internal/catch_approx.hpp">CATCH</a>
 * @see <a href="http://www.thusspakeak.com/ak/2013/06/01-YoureGoingToHaveToThink01.html">
 *      Harris, R. (2013)</a>
 */
static Approx approx
  = Approx::custom( ).epsilon( std::numeric_limits< double >::epsilon( ) * 1000.0 );

//! Get root-path for TOG.
/*!
 * Returns path to root-directory of the TOG application (trailing slash included).
 *
 * @todo    Find a way to test the root-path function.
 *
 * @return  TOG root-path.
 */
static inline std::string getRootPath( )
{
    std::string filePath( __FILE__ );
    return filePath.substr(
      0, filePath.length( ) - std::string( "include/TOG/tools.hpp" ).length( ) );
}

//! Print value to stream.
/*!
 * Prints a specified value to stream provided, given a specified width and a filler character.
 *
 * @tparam     DataType  Type for specified value
 * @param[out] stream    Output stream
 * @param[in]  value     Specified value to print
 * @param[in]  width     Width of value printed to stream, in terms of number of characters
 *                       (default = 25)
 * @param[in]  filler    Character used to fill fixed-width (default = ' ')
 */
template< typename DataType >
inline void print( std::ostream& stream,
                   const DataType value,
                   const int width = 25,
                   const char filler = ' ' )
{
    stream << std::left << std::setw( width ) << std::setfill( filler ) << value;
}

//! Print metadata parameter to stream.
/*!
 * Prints metadata parameter to stream provided, given a specified name, value, units, and
 * delimiter.
 *
 * @tparam     DataType      Type for specified value
 * @param[out] stream        Output stream
 * @param[in]  parameterName Name for metadata parameter
 * @param[in]  value         Specified value to print
 * @param[in]  units         Units for value
 * @param[in]  delimiter     Character used to delimiter entries in stream (default = ' ')
 * @param[in]  width         Width of value printed to stream, in terms of number of characters
 *                           (default = 25)
 * @param[in]  filler        Character used to fill fixed-width (default = ' ')
 */
template< typename DataType >
inline void print( std::ostream& stream,
                   const std::string& parameterName,
                   const DataType value,
                   const std::string& units,
                   const char delimiter = ',',
                   const int width = 25,
                   const char filler = ' ' )
{
    print( stream, parameterName, width, filler );
    stream << delimiter;
    print( stream, value, width, filler );
    stream << delimiter;
    print( stream, units, width, filler );
}

//! Find parameter.
/*!
 * Finds parameter in config stored in JSON document. An error is thrown if the parameter cannot
 * be found. If the parameter is found, an iterator to the member in the JSON document is returned.
 *
 * @param[in] config        JSON document containing config parameters
 * @param[in] parameterName Name of parameter to find
 * @return                  Iterator to parameter retreived from JSON document
 */
ConfigIterator find( const rapidjson::Document& config, const std::string& parameterName );

//! Remove newline characters from string.
/*!
 * Removes newline characters from a string by making use of the STL erase() and remove()
 * functions. Removes '\\n' and '\\r' characters.
 *
 * @param[in,out] string String from which newline characters should be removed
 */
void removeNewline( std::string& string );

} // namespace tog

#endif // TOG_TOOLS_HPP
