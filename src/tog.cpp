/*
 * Copyright (c) 2014-2015 Kartik Kumar (me@kartikkumar.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <rapidjson/document.h>

#include <keplerian_toolbox.h>

int main( const int numberOfInputs, const char* inputArguments[ ] )
{
    ///////////////////////////////////////////////////////////////////////////

    std::cout << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "                Theory of Games -- Sample Application             " << std::endl;
    std::cout << "                              0.1.0                               " << std::endl;
    std::cout << std::endl;
    std::cout << "          Copyright (c) 2015, K. Kumar (me@kartikkumar.com)       " << std::endl;
    std::cout << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////

    std::cout << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << "                          Input parameters                        " << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << std::endl;

    // Check that only one input has been provided (a JSON file).
    if ( numberOfInputs - 1 != 1 )
    {
        std::cerr << "ERROR: Number of inputs is wrong. Please only provide a JSON input file!"
                  << std::endl;
        throw;
    }

    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////

    // Read and store JSON input document (filter out comment lines).
    // TODO: Need to make comment-line filtering more robust.
    std::ifstream inputFile( inputArguments[ 1 ] );
    std::stringstream jsonDocumentBuffer;
    std::string inputLine;
    while ( std::getline( inputFile, inputLine ) )
    {
        size_t startPosition = inputLine.find_first_not_of( " \t" );
        if ( std::string::npos != startPosition )
        {
            inputLine = inputLine.substr( startPosition );
        }

        if ( inputLine.substr( 0, 2 ) != "//" )
        {
            jsonDocumentBuffer << inputLine << "\n";
        }
    }

    rapidjson::Document config;
    config.Parse( jsonDocumentBuffer.str( ).c_str( ) );
    rapidjson::Value::MemberIterator configIterator;

    // Extract initial Keplerian state vector provided by user
    // [a=[km], e=[-], i=[rad], omega=[rad], Omega=[rad], theta=[rad]].
    configIterator = config.FindMember( "initial_keplerian_state" );
    std::vector< double > initialKeplerianState( 6 );
    for ( unsigned int i = 0; i < 6; i++ )
    {
        initialKeplerianState.at( i ) = configIterator->value[ i ].GetDouble( );
    }
    std::cout << "Initial Keplerian state [km, km/s]  :  " << initialKeplerianState << std::endl;

    // Extract gravitational parameter [km^3 s^-2].
    configIterator = config.FindMember( "gravitational_parameter" );
    const double gravitationalParameter = configIterator->value.GetDouble( );
    std::cout << "Gravitational parameter [km^3 s^-2] :  " << gravitationalParameter << std::endl;

    // Extract start epoch [s].
    configIterator = config.FindMember( "start_epoch" );
    const double startEpoch = configIterator->value.GetDouble( );
    std::cout << "Start epoch [s]                     :  " << startEpoch << std::endl;

    // Extract step size [s].
    configIterator = config.FindMember( "step_size" );
    const double stepSize = configIterator->value.GetDouble( );
    std::cout << "Step size [s]                       :  " << stepSize << std::endl;

    // Extract end epoch [s].
    configIterator = config.FindMember( "number_of_steps" );
    const double numberOfSteps = configIterator->value.GetDouble( );
    std::cout << "Number of steps                     :  " << numberOfSteps << std::endl;

    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////

    std::cout << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << "                             Simulation                           " << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << std::endl;

    // Set epoch [s].
    double epoch = startEpoch;

    // Convert initial Keplerian state to Cartesian position and velocity [km; km/s].
    std::vector< double > position( 3 );
    std::vector< double > velocity( 3 );
    kep_toolbox::par2ic( initialKeplerianState, gravitationalParameter, position, velocity );

    // std::cout << "t           position [km]                        velocity [km/s]" << std::endl;
    std::cout << epoch << ",";
    for ( unsigned int j = 0; j < 3; j++ )
    {
        std::cout << position.at( j ) << ",";
    }
    for ( unsigned int k = 0; k < 2; k++ )
    {
        std::cout << velocity.at( k ) << ",";
    }
    std::cout << velocity.at( 2 ) << std::endl;

    for ( unsigned int i = 0; i < numberOfSteps; i++ )
    {
        kep_toolbox::propagate_lagrangian( position, velocity, stepSize, gravitationalParameter );
        epoch = epoch + stepSize;
        std::cout << epoch << ",";
        for ( unsigned int j = 0; j < 3; j++ )
        {
            std::cout << position.at( j ) << ",";
        }
        for ( unsigned int k = 0; k < 2; k++ )
        {
            std::cout << velocity.at( k ) << ",";
        }
        std::cout << velocity.at( 2 ) << std::endl;
    }

    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////

    std::cout << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "                         Exited successfully!                     " << std::endl;
    std::cout << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    return EXIT_SUCCESS;

    ///////////////////////////////////////////////////////////////////////////
}
