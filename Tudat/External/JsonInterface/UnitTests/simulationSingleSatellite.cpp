/*    Copyright (c) 2010-2017, Delft University of Technology
 *    All rigths reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */

#define BOOST_TEST_MAIN

#include <Tudat/SimulationSetup/tudatSimulationHeader.h>

#include "unitTestSupport.h"
#include <Tudat/External/JsonInterface/simulation.h>

namespace tudat
{

namespace unit_tests
{

BOOST_AUTO_TEST_SUITE( test_json_simulationSingleSatellite )

BOOST_AUTO_TEST_CASE( test_json_simulationSingleSatellite )
{

    using namespace simulation_setup;
    using namespace propagators;
    using namespace numerical_integrators;
    using namespace orbital_element_conversions;
    using namespace basic_mathematics;
    using namespace unit_conversions;
    using namespace json_interface;

    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////             JSON SIMULATION              ///////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    Simulation< > jsonSimulation( ( inputDirectory( ) / "simulationSingleSatellite.json" ).string( ) );
    jsonSimulation.run( );
    std::map< double, Eigen::VectorXd > jsonResults =
            jsonSimulation.getDynamicsSimulator( )->getEquationsOfMotionNumericalSolution( );



    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////            MANUAL SIMULATION             ///////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////     CREATE ENVIRONMENT AND VEHICLE       ///////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    // Load Spice kernels.
    spice_interface::loadSpiceKernelInTudat( input_output::getSpiceKernelPath( ) + "pck00009.tpc" );
    spice_interface::loadSpiceKernelInTudat( input_output::getSpiceKernelPath( ) + "de-403-masses.tpc" );
    spice_interface::loadSpiceKernelInTudat( input_output::getSpiceKernelPath( ) + "de421.bsp" );

    // Set simulation end epoch.
    const double simulationEndEpoch = tudat::physical_constants::JULIAN_DAY;


    // Create body objects.
    std::vector< std::string > bodiesToCreate;
    bodiesToCreate.push_back( "Earth" );
    std::map< std::string, boost::shared_ptr< BodySettings > > bodySettings =
            getDefaultBodySettings( bodiesToCreate );
    bodySettings[ "Earth" ]->ephemerisSettings = boost::make_shared< ConstantEphemerisSettings >(
                Eigen::Vector6d::Zero( ) );

    // Create Earth object
    NamedBodyMap bodyMap = createBodies( bodySettings );

    // Create spacecraft object.
    bodyMap[ "Asterix" ] = boost::make_shared< simulation_setup::Body >( );

    // Finalize body creation.
    setGlobalFrameBodyEphemerides( bodyMap, "SSB", "ECLIPJ2000" );

    ////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////            CREATE ACCELERATIONS          ///////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    // Define propagator settings variables.
    SelectedAccelerationMap accelerationMap;
    std::vector< std::string > bodiesToPropagate;
    std::vector< std::string > centralBodies;

    bodiesToPropagate.push_back( "Asterix" );
    centralBodies.push_back( "Earth" );

    // Define propagation settings.
    SingleSelectedAccelerationMap accelerationsOfAsterix;
    accelerationsOfAsterix[ "Earth" ].push_back( boost::make_shared< AccelerationSettings >(
                                                     basic_astrodynamics::central_gravity ) );
    accelerationMap[  "Asterix" ] = accelerationsOfAsterix;

    // Create acceleration models and propagation settings.
    basic_astrodynamics::AccelerationMap accelerationModelMap = createAccelerationModelsMap(
                bodyMap, accelerationMap, bodiesToPropagate, centralBodies );



    ////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////             CREATE PROPAGATION SETTINGS            /////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    // Set initial conditions for the Asterix satellite that will be propagated in this simulation.
    const Eigen::VectorXd systemInitialState =
            ( Eigen::Vector6d( ) << 7.037484e6, 3.238059e6, 2.150724e6, -1.465658e3, -0.040958e3, 6.622798e3 ).finished( );

    boost::shared_ptr< TranslationalStatePropagatorSettings< double > > propagatorSettings =
            boost::make_shared< TranslationalStatePropagatorSettings< double > >
            ( centralBodies, accelerationModelMap, bodiesToPropagate, systemInitialState, simulationEndEpoch );


    // Create numerical integrator.
    const double simulationStartEpoch = 0.0;
    const double fixedStepSize = 10.0;
    boost::shared_ptr< IntegratorSettings< > > integratorSettings =
            boost::make_shared< IntegratorSettings< > >
            ( rungeKutta4, simulationStartEpoch, fixedStepSize );

    ////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////             PROPAGATE ORBIT            /////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    // Create simulation object and propagate dynamics.
    const boost::shared_ptr< SingleArcDynamicsSimulator< > > dynamicsSimulator =
            boost::make_shared< SingleArcDynamicsSimulator< > >( bodyMap, integratorSettings, propagatorSettings );
    const std::map< double, Eigen::VectorXd > results = dynamicsSimulator->getEquationsOfMotionNumericalSolution( );


    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////        COMPARE RESULTS           ///////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    BOOST_CHECK_CLOSE_INTEGRATION_RESULTS( jsonResults, results, 1.0E-15 );



    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    ///////////      CHECK CONSISTENCY OF from_json and to_json FUNCTIONS          /////////
    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    // Convert jsonSimulation to JSON (using to_json functions) and use that to reset the simulation
    // (using from_json functions)
    jsonSimulation.reset( jsonSimulation.getAsJSON( ) );

    // Get results
    jsonSimulation.run( );
    jsonResults = jsonSimulation.getDynamicsSimulator( )->getEquationsOfMotionNumericalSolution( );

    BOOST_CHECK_CLOSE_INTEGRATION_RESULTS( jsonResults, results, 1.0E-15 );

}


BOOST_AUTO_TEST_SUITE_END( )

} // namespace unit_tests

} // namespace tudat
