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

#include <limits>
#include <string>

#include <boost/format.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>

#include "Tudat/Basics/testMacros.h"

#include "Tudat/Astrodynamics/GroundStations/pointingAnglesCalculator.h"
#include "Tudat/SimulationSetup/tudatEstimationHeader.h"

namespace tudat
{
namespace unit_tests
{

using namespace tudat::observation_models;
using namespace tudat::spice_interface;
using namespace tudat::ephemerides;
using namespace tudat::simulation_setup;


BOOST_AUTO_TEST_SUITE( test_observation_viability_calculators )

std::vector< double > getBodyLinkElevationAngles(
        const LinkEnds linkEnds,
        const ObservableType observableType,
        const std::string referenceBody,
        const std::vector< Eigen::Vector6d > linkEndStates,
        const std::vector< double > linkEndTimes,
        const NamedBodyMap& bodyMap )
{
    boost::shared_ptr< ground_stations::PointingAnglesCalculator > currentPointingAnglesCalculator;
    std::vector< double > elevationAngles;
    switch( observableType )
    {
    case one_way_range:
    {
        if( linkEnds.at( transmitter ).first == referenceBody )
        {
            currentPointingAnglesCalculator = bodyMap.at( referenceBody )->getGroundStation(
                        linkEnds.at( transmitter ).second )->getPointingAnglesCalculator( );
            elevationAngles.push_back( currentPointingAnglesCalculator->calculateElevationAngle(
                                           ( linkEndStates.at( 1 ) - linkEndStates.at( 0 ) ).segment( 0, 3 ), linkEndTimes.at( 0 ) ) );

        }
        else if( linkEnds.at( receiver ).first == referenceBody )
        {
            currentPointingAnglesCalculator = bodyMap.at( referenceBody )->getGroundStation(
                        linkEnds.at( receiver ).second )->getPointingAnglesCalculator( );
            elevationAngles.push_back( currentPointingAnglesCalculator->calculateElevationAngle(
                                           ( linkEndStates.at( 0 ) - linkEndStates.at( 1 ) ).segment( 0, 3 ), linkEndTimes.at( 1 ) ) );
        }
        break;
    }
    case one_way_doppler:
    {
        if( linkEnds.at( transmitter ).first == referenceBody )
        {
            currentPointingAnglesCalculator = bodyMap.at( referenceBody )->getGroundStation(
                        linkEnds.at( transmitter ).second )->getPointingAnglesCalculator( );
            elevationAngles.push_back( currentPointingAnglesCalculator->calculateElevationAngle(
                                           ( linkEndStates.at( 1 ) - linkEndStates.at( 0 ) ).segment( 0, 3 ), linkEndTimes.at( 0 ) ) );

        }
        else if( linkEnds.at( receiver ).first == referenceBody )
        {
            currentPointingAnglesCalculator = bodyMap.at( referenceBody )->getGroundStation(
                        linkEnds.at( receiver ).second )->getPointingAnglesCalculator( );
            elevationAngles.push_back( currentPointingAnglesCalculator->calculateElevationAngle(
                                           ( linkEndStates.at( 0 ) - linkEndStates.at( 1 ) ).segment( 0, 3 ), linkEndTimes.at( 1 ) ) );
        }
        break;
    }
    case angular_position:
    {
        if( linkEnds.at( transmitter ).first == referenceBody )
        {
            currentPointingAnglesCalculator = bodyMap.at( referenceBody )->getGroundStation(
                        linkEnds.at( transmitter ).second )->getPointingAnglesCalculator( );
            elevationAngles.push_back( currentPointingAnglesCalculator->calculateElevationAngle(
                                           ( linkEndStates.at( 1 ) - linkEndStates.at( 0 ) ).segment( 0, 3 ), linkEndTimes.at( 0 ) ) );

        }
        else if( linkEnds.at( receiver ).first == referenceBody )
        {
            currentPointingAnglesCalculator = bodyMap.at( referenceBody )->getGroundStation(
                        linkEnds.at( receiver ).second )->getPointingAnglesCalculator( );
            elevationAngles.push_back( currentPointingAnglesCalculator->calculateElevationAngle(
                                           ( linkEndStates.at( 0 ) - linkEndStates.at( 1 ) ).segment( 0, 3 ), linkEndTimes.at( 1 ) ) );
        }
        break;
    }
    case one_way_differenced_range:
    {
        if( linkEnds.at( transmitter ).first == referenceBody )
        {
            currentPointingAnglesCalculator = bodyMap.at( referenceBody )->getGroundStation(
                        linkEnds.at( transmitter ).second )->getPointingAnglesCalculator( );
            elevationAngles.push_back( currentPointingAnglesCalculator->calculateElevationAngle(
                                           ( linkEndStates.at( 1 ) - linkEndStates.at( 0 ) ).segment( 0, 3 ), linkEndTimes.at( 0 ) ) );
            elevationAngles.push_back( currentPointingAnglesCalculator->calculateElevationAngle(
                                           ( linkEndStates.at( 3 ) - linkEndStates.at( 2 ) ).segment( 0, 3 ), linkEndTimes.at( 2 ) ) );

        }
        else if( linkEnds.at( receiver ).first == referenceBody )
        {
            currentPointingAnglesCalculator = bodyMap.at( referenceBody )->getGroundStation(
                        linkEnds.at( receiver ).second )->getPointingAnglesCalculator( );
            elevationAngles.push_back( currentPointingAnglesCalculator->calculateElevationAngle(
                                           ( linkEndStates.at( 0 ) - linkEndStates.at( 1 ) ).segment( 0, 3 ), linkEndTimes.at( 1 ) ) );
            elevationAngles.push_back( currentPointingAnglesCalculator->calculateElevationAngle(
                                           ( linkEndStates.at( 2 ) - linkEndStates.at( 3 ) ).segment( 0, 3 ), linkEndTimes.at( 3 ) ) );
        }
        break;
    }
    case n_way_range:
    {
        int linkEndIndex = 0;
        for( LinkEnds::const_iterator linkEndIterator = linkEnds.begin( ); linkEndIterator != linkEnds.end( );
             linkEndIterator++ )
        {
            if( linkEndIterator->second.first == referenceBody )
            {
                currentPointingAnglesCalculator = bodyMap.at( referenceBody )->getGroundStation(
                            linkEndIterator->second.second )->getPointingAnglesCalculator( );
                if( linkEndIndex != 0 )
                {
                    elevationAngles.push_back(
                                currentPointingAnglesCalculator->calculateElevationAngle(
                                    ( linkEndStates.at( 2 * ( linkEndIndex - 1 ) ) -
                                      linkEndStates.at( 2 * ( linkEndIndex - 1 ) + 1 ) ).segment( 0, 3 ),
                                    linkEndTimes.at( 2 * ( linkEndIndex - 1 ) + 1 ) ) );
                }

                if( linkEndIndex != static_cast< int >( linkEnds.size( ) ) - 1 )
                {
                    elevationAngles.push_back(
                                currentPointingAnglesCalculator->calculateElevationAngle(
                                    ( linkEndStates.at( 2 * linkEndIndex + 1) -
                                      linkEndStates.at( 2 * linkEndIndex ) ).segment( 0, 3 ),
                                    linkEndTimes.at( 2 * linkEndIndex ) ) );
                }

            }
            linkEndIndex++;
        }
        break;
    }
    default:
        throw std::runtime_error( "Error when testing elevation angle viability, observable not recognized" );

    }

    return elevationAngles;
}

std::vector< double > getBodyCosineAvoidanceAngles(
        const LinkEnds linkEnds,
        const ObservableType observableType,
        const std::string referenceBody,
        const std::string bodyToAvoid,
        const std::vector< Eigen::Vector6d > linkEndStates,
        const std::vector< double > linkEndTimes,
        const NamedBodyMap& bodyMap )
{
    std::vector< double > cosineAvoidanceAngles;
    switch( observableType )
    {
    case one_way_range:
    {
        if( linkEnds.at( transmitter ).first == referenceBody )
        {
            cosineAvoidanceAngles.push_back( linear_algebra::computeCosineOfAngleBetweenVectors(
                                                 ( ( linkEndStates.at( 1 ) - linkEndStates.at( 0 ) ).segment( 0, 3 ) ), (
                                                     bodyMap.at( bodyToAvoid )->getStateInBaseFrameFromEphemeris< double, double >(
                                                         ( linkEndTimes.at( 1 ) + linkEndTimes.at( 0 ) ) / 2.0 ).segment( 0, 3 ) -
                                                     linkEndStates.at( 0 ).segment( 0, 3 ) ) ) );
        }
        else if( linkEnds.at( receiver ).first == referenceBody )
        {
            cosineAvoidanceAngles.push_back( linear_algebra::computeCosineOfAngleBetweenVectors(
                                                 ( ( linkEndStates.at( 0 ) - linkEndStates.at( 1 ) ).segment( 0, 3 ) ), (
                                                     bodyMap.at( bodyToAvoid )->getStateInBaseFrameFromEphemeris< double, double >(
                                                         ( linkEndTimes.at( 1 ) + linkEndTimes.at( 0 ) ) / 2.0 ).segment( 0, 3 ) -
                                                     linkEndStates.at( 1 ).segment( 0, 3 ) ) ) );
        }
        break;
    }
    case one_way_doppler:
    {
        if( linkEnds.at( transmitter ).first == referenceBody )
        {
            cosineAvoidanceAngles.push_back( linear_algebra::computeCosineOfAngleBetweenVectors(
                                                 ( ( linkEndStates.at( 1 ) - linkEndStates.at( 0 ) ).segment( 0, 3 ) ), (
                                                     bodyMap.at( bodyToAvoid )->getStateInBaseFrameFromEphemeris< double, double >(
                                                         ( linkEndTimes.at( 1 ) + linkEndTimes.at( 0 ) ) / 2.0 ).segment( 0, 3 ) -
                                                     linkEndStates.at( 0 ).segment( 0, 3 ) ) ) );
        }
        else if( linkEnds.at( receiver ).first == referenceBody )
        {
            cosineAvoidanceAngles.push_back( linear_algebra::computeCosineOfAngleBetweenVectors(
                                                 ( ( linkEndStates.at( 0 ) - linkEndStates.at( 1 ) ).segment( 0, 3 ) ), (
                                                     bodyMap.at( bodyToAvoid )->getStateInBaseFrameFromEphemeris< double, double >(
                                                         ( linkEndTimes.at( 1 ) + linkEndTimes.at( 0 ) ) / 2.0 ).segment( 0, 3 ) -
                                                     linkEndStates.at( 1 ).segment( 0, 3 ) ) ) );
        }
        break;
    }
    case angular_position:
    {
        if( linkEnds.at( transmitter ).first == referenceBody )
        {
            cosineAvoidanceAngles.push_back( linear_algebra::computeCosineOfAngleBetweenVectors(
                                                 ( ( linkEndStates.at( 1 ) - linkEndStates.at( 0 ) ).segment( 0, 3 ) ), (
                                                     bodyMap.at( bodyToAvoid )->getStateInBaseFrameFromEphemeris< double, double >(
                                                         ( linkEndTimes.at( 1 ) + linkEndTimes.at( 0 ) ) / 2.0 ).segment( 0, 3 ) -
                                                     linkEndStates.at( 0 ).segment( 0, 3 ) ) ) );
        }
        else if( linkEnds.at( receiver ).first == referenceBody )
        {
            cosineAvoidanceAngles.push_back( linear_algebra::computeCosineOfAngleBetweenVectors(
                                                 ( ( linkEndStates.at( 0 ) - linkEndStates.at( 1 ) ).segment( 0, 3 ) ), (
                                                     bodyMap.at( bodyToAvoid )->getStateInBaseFrameFromEphemeris< double, double >(
                                                         ( linkEndTimes.at( 1 ) + linkEndTimes.at( 0 ) ) / 2.0 ).segment( 0, 3 ) -
                                                     linkEndStates.at( 1 ).segment( 0, 3 ) ) ) );
        }
        break;
    }
    case one_way_differenced_range:
    {
        if( linkEnds.at( transmitter ).first == referenceBody )
        {
            cosineAvoidanceAngles.push_back( linear_algebra::computeCosineOfAngleBetweenVectors(
                                                 ( ( linkEndStates.at( 1 ) - linkEndStates.at( 0 ) ).segment( 0, 3 ) ), (
                                                     bodyMap.at( bodyToAvoid )->getStateInBaseFrameFromEphemeris< double, double >(
                                                         ( linkEndTimes.at( 1 ) + linkEndTimes.at( 0 ) ) / 2.0 ).segment( 0, 3 ) -
                                                     linkEndStates.at( 0 ).segment( 0, 3 ) ) ) );
            cosineAvoidanceAngles.push_back(  linear_algebra::computeCosineOfAngleBetweenVectors(
                                                  ( ( linkEndStates.at( 3 ) - linkEndStates.at( 2 ) ).segment( 0, 3 ) ), (
                                                      bodyMap.at( bodyToAvoid )->getStateInBaseFrameFromEphemeris< double, double >(
                                                          ( linkEndTimes.at( 3 ) + linkEndTimes.at( 2 ) ) / 2.0 ).segment( 0, 3 ) -
                                                      linkEndStates.at( 2 ).segment( 0, 3 ) ) ) );
        }
        else if( linkEnds.at( receiver ).first == referenceBody )
        {
            cosineAvoidanceAngles.push_back(  linear_algebra::computeCosineOfAngleBetweenVectors(
                                                  ( ( linkEndStates.at( 0 ) - linkEndStates.at( 1 ) ).segment( 0, 3 ) ), (
                                                      bodyMap.at( bodyToAvoid )->getStateInBaseFrameFromEphemeris< double, double >(
                                                          ( linkEndTimes.at( 1 ) + linkEndTimes.at( 0 ) ) / 2.0 ).segment( 0, 3 ) -
                                                      linkEndStates.at( 1 ).segment( 0, 3 ) ) ) );
            cosineAvoidanceAngles.push_back(  linear_algebra::computeCosineOfAngleBetweenVectors(
                                                  ( ( linkEndStates.at( 2 ) - linkEndStates.at( 3 ) ).segment( 0, 3 ) ), (
                                                      bodyMap.at( bodyToAvoid )->getStateInBaseFrameFromEphemeris< double, double >(
                                                          ( linkEndTimes.at( 3 ) + linkEndTimes.at( 2 ) ) / 2.0 ).segment( 0, 3 ) -
                                                      linkEndStates.at( 3 ).segment( 0, 3 ) ) ) );
        }
        break;
    }
    case n_way_range:
    {
        int linkEndIndex = 0;
        for( LinkEnds::const_iterator linkEndIterator = linkEnds.begin( ); linkEndIterator != linkEnds.end( );
             linkEndIterator++ )
        {
            if( linkEndIterator->second.first == referenceBody )
            {
                if( linkEndIndex != 0 )
                {
                    cosineAvoidanceAngles.push_back(  linear_algebra::computeCosineOfAngleBetweenVectors(
                                                          ( ( linkEndStates.at( 2 * ( linkEndIndex - 1 ) ) -
                                                              linkEndStates.at( 2 * ( linkEndIndex - 1 ) + 1 ) ).segment( 0, 3 ) ), (
                                                              bodyMap.at( bodyToAvoid )->getStateInBaseFrameFromEphemeris< double, double >(
                                                                  ( linkEndTimes.at( 2 * ( linkEndIndex - 1 ) + 1  ) +
                                                                    linkEndTimes.at( 2 * ( linkEndIndex - 1 ) ) ) / 2.0 ).segment( 0, 3 ) -
                                                              linkEndStates.at( 2 * ( linkEndIndex - 1 ) + 1 ).segment( 0, 3 ) ) ) );
                }

                if( linkEndIndex != static_cast< int >( linkEnds.size( ) ) - 1 )
                {
                    cosineAvoidanceAngles.push_back( linear_algebra::computeCosineOfAngleBetweenVectors(
                                                         ( linkEndStates.at( 2 * linkEndIndex + 1 ) -
                                                           linkEndStates.at( 2 * linkEndIndex ) ).segment( 0, 3 ), (
                                                             bodyMap.at( bodyToAvoid )->getStateInBaseFrameFromEphemeris< double, double >(
                                                                 ( linkEndTimes.at( 2 * linkEndIndex + 1  ) +
                                                                   linkEndTimes.at( 2 * linkEndIndex  ) ) / 2.0 ).segment( 0, 3 ) -
                                                             linkEndStates.at( 2 * linkEndIndex ).segment( 0, 3 ) ) ) );
                }

            }
            linkEndIndex++;
        }
        break;
    }
    default:
        throw std::runtime_error( "Error when testing avoidance angle viability, observable not recognized" );

    }

    return cosineAvoidanceAngles;
}

std::vector< double > getDistanceBetweenLineOfSightVectorAndPoint(
        const std::string bodyToAnalyze,
        const std::vector< Eigen::Vector6d > linkEndStates,
        const std::vector< double > linkEndTimes,
        const NamedBodyMap& bodyMap )
{
    std::vector< double > pointDistances;
    for( unsigned int i = 0; i < linkEndStates.size( ); i += 2 )
    {
        Eigen::Vector3d linkEnd1Position = linkEndStates.at( i ).segment( 0, 3 );
        Eigen::Vector3d linkEnd2Position = linkEndStates.at( i + 1 ).segment( 0, 3 );

        Eigen::Vector3d pointPosition = bodyMap.at( bodyToAnalyze )->getStateInBaseFrameFromEphemeris< double, double >(
                    ( linkEndTimes.at( i ) + linkEndTimes.at( i ) ) / 2.0 ).segment( 0, 3 );

        pointDistances.push_back( ( ( linkEnd2Position - pointPosition ).cross( linkEnd1Position - pointPosition ) ).norm( ) /
                                  ( linkEnd2Position - linkEnd1Position ).norm( ) );

    }
    return pointDistances;
}

BOOST_AUTO_TEST_CASE( testObservationViabilityCalculators )
{
    //Load spice kernels.
    std::string kernelsPath = input_output::getSpiceKernelPath( );
    spice_interface::loadSpiceKernelInTudat( kernelsPath + "de-403-masses.tpc");
    spice_interface::loadSpiceKernelInTudat( kernelsPath + "naif0009.tls");
    spice_interface::loadSpiceKernelInTudat( kernelsPath + "pck00009.tpc");
    spice_interface::loadSpiceKernelInTudat( kernelsPath + "de421.bsp");

    // Define environment settings
    std::vector< std::string > bodyNames;
    bodyNames.push_back( "Earth" );
    bodyNames.push_back( "Mars" );
    bodyNames.push_back( "Sun" );
    bodyNames.push_back( "Moon" );

    std::map< std::string, boost::shared_ptr< BodySettings > > bodySettings =
            getDefaultBodySettings( bodyNames );

    // Set simplified rotation models for Earth/Mars (spice rotation retrieval is slow)
    bodySettings[ "Earth" ]->rotationModelSettings = boost::make_shared< SimpleRotationModelSettings >(
                "ECLIPJ2000", "IAU_Earth",
                spice_interface::computeRotationQuaternionBetweenFrames(
                    "ECLIPJ2000", "IAU_Earth", 0.0 ),
                0.0, 2.0 * mathematical_constants::PI /
                ( physical_constants::JULIAN_DAY ) );
    bodySettings[ "Mars" ]->rotationModelSettings = boost::make_shared< SimpleRotationModelSettings >(
                "ECLIPJ2000", "IAU_Mars",
                spice_interface::computeRotationQuaternionBetweenFrames(
                    "ECLIPJ2000", "IAU_Mars", 0.0 ),
                0.0, 2.0 * mathematical_constants::PI /
                ( physical_constants::JULIAN_DAY + 40.0 * 60.0 ) );
    double moonRadius = 1.0E9;
    bodySettings[ "Moon" ]->shapeModelSettings = boost::make_shared< SphericalBodyShapeSettings >( moonRadius );
    NamedBodyMap bodyMap = createBodies( bodySettings );
    setGlobalFrameBodyEphemerides( bodyMap, "SSB", "ECLIPJ2000" );

    // Create ground stations
    std::pair< std::string, std::string > earthStation1 = std::pair< std::string, std::string >( "Earth", "EarthStation1" );
    std::pair< std::string, std::string > earthStation2 = std::pair< std::string, std::string >( "Earth", "EarthStation2" );
    std::pair< std::string, std::string > mslStation1 = std::pair< std::string, std::string >( "Mars", "MarsStation1" );
    std::pair< std::string, std::string > mslStation2 = std::pair< std::string, std::string >( "Mars", "MarsStation2" );

    createGroundStation( bodyMap.at( "Mars" ), "MarsStation1", ( Eigen::Vector3d( )<< 100.0, 0.2, 2.1 ).finished( ),
                         coordinate_conversions::geodetic_position );
    createGroundStation( bodyMap.at( "Mars" ), "MarsStation2", ( Eigen::Vector3d( )<< -2000.0, -0.4, 0.1 ).finished( ),
                         coordinate_conversions::geodetic_position );
    createGroundStation( bodyMap.at( "Earth" ), "EarthStation1", ( Eigen::Vector3d( )<< 800.0, 0.12, 5.3 ).finished( ),
                         coordinate_conversions::geodetic_position );
    createGroundStation( bodyMap.at( "Earth" ), "EarthStation2", ( Eigen::Vector3d( )<< 100.0, 0.15, 0.0 ).finished( ),
                         coordinate_conversions::geodetic_position );

    LinkEnds oneWayLinkEnds1;
    oneWayLinkEnds1[ transmitter ] = std::make_pair( "Mars", "MarsStation1" );
    oneWayLinkEnds1[ receiver ] = std::make_pair( "Earth", "EarthStation1" );

    LinkEnds oneWayLinkEnds2;
    oneWayLinkEnds2[ transmitter ] = std::make_pair( "Earth", "EarthStation2" );
    oneWayLinkEnds2[ receiver ] = std::make_pair( "Mars", "MarsStation2" );

    LinkEnds oneWayLinkEnds3;
    oneWayLinkEnds3[ transmitter ] = std::make_pair( "Mars", "MarsStation1" );
    oneWayLinkEnds3[ receiver ] = std::make_pair( "Earth", "EarthStation2" );
    std::vector< LinkEnds > oneWayRangeLinkEnds;
    oneWayRangeLinkEnds.push_back( oneWayLinkEnds1 );
    oneWayRangeLinkEnds.push_back( oneWayLinkEnds2 );
    oneWayRangeLinkEnds.push_back( oneWayLinkEnds3 );

    LinkEnds twoWayLinkEnds1;
    twoWayLinkEnds1[ transmitter ] = std::make_pair( "Mars", "MarsStation1" );
    twoWayLinkEnds1[ reflector1 ] = std::make_pair( "Earth", "EarthStation1" );
    twoWayLinkEnds1[ receiver ] = std::make_pair( "Mars", "MarsStation1" );

    LinkEnds twoWayLinkEnds2;
    twoWayLinkEnds2[ transmitter ] = std::make_pair( "Earth", "EarthStation2" );
    twoWayLinkEnds2[ reflector1 ] = std::make_pair( "Mars", "MarsStation2" );
    twoWayLinkEnds2[ receiver ] = std::make_pair( "Earth", "EarthStation1" );

    LinkEnds twoWayLinkEnds3;
    twoWayLinkEnds3[ transmitter ] = std::make_pair( "Mars", "MarsStation2" );
    twoWayLinkEnds3[ reflector1 ] = std::make_pair( "Earth", "EarthStation2" );
    twoWayLinkEnds3[ receiver ] = std::make_pair( "Mars", "MarsStation1" );
    std::vector< LinkEnds > twoWayRangeLinkEnds;
    twoWayRangeLinkEnds.push_back( twoWayLinkEnds1 );
    twoWayRangeLinkEnds.push_back( twoWayLinkEnds2 );
    twoWayRangeLinkEnds.push_back( twoWayLinkEnds3 );

    std::map< ObservableType, std::vector< LinkEnds > > testLinkEndsList;
    testLinkEndsList[ one_way_range ] = oneWayRangeLinkEnds;
    testLinkEndsList[ one_way_differenced_range ] = oneWayRangeLinkEnds;
    testLinkEndsList[ angular_position ] = oneWayRangeLinkEnds;
    testLinkEndsList[ n_way_range ] = twoWayRangeLinkEnds;

    std::vector< double > unconstrainedObservationTimes;
    LinkEndType referenceLinkEnd = transmitter;
    double initialTime = 0.0, finalTime = 10.0 * physical_constants::JULIAN_YEAR, timeStep = physical_constants::JULIAN_DAY * 2.5;
    double currentTime = initialTime;
    while( currentTime <= finalTime )
    {
        unconstrainedObservationTimes.push_back( currentTime );
        currentTime += timeStep;
    }
    int unconstrainedNumberOfObservations = unconstrainedObservationTimes.size( );

    std::map< ObservableType, std::map< LinkEnds, boost::shared_ptr< ObservationSimulationTimeSettings< double > > > >
            observationTimeSettings;


    typedef std::map< ObservableType, std::map< LinkEnds, boost::shared_ptr< ObservationSettings > > > SortedObservationSettingsMap;
    SortedObservationSettingsMap observationSettingsMap;
    for( std::map< ObservableType, std::vector< LinkEnds > >::const_iterator observableIterator = testLinkEndsList.begin( );
         observableIterator != testLinkEndsList.end( ); observableIterator++ )
    {
        for( unsigned int i = 0; i < observableIterator->second.size( ); i++ )
        {
            if( observableIterator->first == one_way_differenced_range )
            {
                observationSettingsMap[ observableIterator->first ][ observableIterator->second.at( i ) ] =
                        boost::make_shared< OneWayDifferencedRangeRateObservationSettings >(
                            boost::lambda::constant( 60.0 ), boost::shared_ptr< LightTimeCorrectionSettings > ( ) );
            }
            else if( observableIterator->first == n_way_range )
            {
                observationSettingsMap[ observableIterator->first ][ observableIterator->second.at( i ) ] =
                        boost::make_shared< NWayRangeObservationSettings >(
                            boost::shared_ptr< LightTimeCorrectionSettings >( ), observableIterator->second.at( i ).size( ) );
            }
            else
            {
                observationSettingsMap[ observableIterator->first ][ observableIterator->second.at( i ) ] =
                        boost::make_shared< ObservationSettings >(
                            observableIterator->first, boost::shared_ptr< LightTimeCorrectionSettings >( ) );

            }
            observationTimeSettings[ observableIterator->first ][ observableIterator->second.at( i ) ] =
                    boost::make_shared< TabulatedObservationSimulationTimeSettings< double > >(
                        referenceLinkEnd, unconstrainedObservationTimes );
        }
    }

    // Create observation viability settings
    double earthLimitAngle = 4.0 * M_PI / 180.0;
    double marsLimitAngle = 10.0 * M_PI / 180.0;

    double earthSunAvoidanceAngle = 30.0 * M_PI / 180.0;
    double marsSunAvoidanceAngle = 21.0 * M_PI / 180.0;


    std::vector< boost::shared_ptr< ObservationViabilitySettings > > observationViabilitySettings;
    observationViabilitySettings.push_back( boost::make_shared< ObservationViabilitySettings >(
                                                minimum_elevation_angle, std::make_pair( "Earth", "" ), "",
                                                earthLimitAngle ) );
    observationViabilitySettings.push_back( boost::make_shared< ObservationViabilitySettings >(
                                                minimum_elevation_angle, std::make_pair( "Mars", "" ), "",
                                                marsLimitAngle ) );
    observationViabilitySettings.push_back( boost::make_shared< ObservationViabilitySettings >(
                                                body_avoidance_angle, std::make_pair( "Earth", "" ), "Sun",
                                                earthSunAvoidanceAngle ) );
    observationViabilitySettings.push_back( boost::make_shared< ObservationViabilitySettings >(
                                                body_avoidance_angle, std::make_pair( "Mars", "" ), "Sun",
                                                marsSunAvoidanceAngle ) );
    observationViabilitySettings.push_back( boost::make_shared< ObservationViabilitySettings >(
                                                body_occultation, std::make_pair( "Earth", "" ), "Moon" ) );
    PerObservableObservationViabilityCalculatorList viabilityCalculators = createObservationViabilityCalculators(
                bodyMap, testLinkEndsList, observationViabilitySettings );

    std::map< ObservableType,  boost::shared_ptr< ObservationSimulatorBase< double, double > > > observationSimulators =
            createObservationSimulators( observationSettingsMap , bodyMap );



    std::map< ObservableType, std::map< LinkEnds, std::pair< Eigen::VectorXd, std::vector< double > > > >
            unconstrainedSimulatedObservables = removeLinkIdFromSimulatedObservations(
                simulateObservations( observationTimeSettings,observationSimulators ) );
    std::map< ObservableType, std::map< LinkEnds, std::pair< Eigen::VectorXd, std::vector< double > > > >
            unconstrainedSimulatedObservablesFromObjects;

    std::map< ObservableType, std::map< LinkEnds, std::pair< Eigen::VectorXd, std::vector< double > > > >
            constrainedSimulatedObservables = removeLinkIdFromSimulatedObservations(
                simulateObservations( observationTimeSettings, observationSimulators, viabilityCalculators ) );
    std::map< ObservableType, std::map< LinkEnds,  std::pair< Eigen::VectorXd, std::vector< double > > > >
            constrainedSimulatedObservablesFromObjects;

    for( std::map< ObservableType,  boost::shared_ptr< ObservationSimulatorBase< double, double > > >::iterator
         simulatorIterator = observationSimulators.begin( ); simulatorIterator != observationSimulators.end( );
         simulatorIterator++ )
    {
        for( unsigned int i = 0; i < testLinkEndsList.at( simulatorIterator->first ).size( ); i++ )
        {
            unconstrainedSimulatedObservablesFromObjects[ simulatorIterator->first ][
                    testLinkEndsList.at( simulatorIterator->first ).at ( i ) ] = simulatorIterator->second->simulateObservations(
                        unconstrainedObservationTimes,  testLinkEndsList.at( simulatorIterator->first ).at ( i ),
                        referenceLinkEnd, false );
        }

        if( viabilityCalculators.count( simulatorIterator->first ) > 0 )
        {
            simulatorIterator->second->setViabilityCalculators( viabilityCalculators.at( simulatorIterator->first ) );
        }

        for( unsigned int i = 0; i < testLinkEndsList.at( simulatorIterator->first ).size( ); i++ )
        {
            constrainedSimulatedObservablesFromObjects[ simulatorIterator->first ][
                    testLinkEndsList.at( simulatorIterator->first ).at ( i ) ] = simulatorIterator->second->simulateObservations(
                        unconstrainedObservationTimes,  testLinkEndsList.at( simulatorIterator->first ).at ( i ),
                        referenceLinkEnd, true );
        }
    }

    int numberOfObsevables = testLinkEndsList.size( );

    BOOST_CHECK_EQUAL( numberOfObsevables, unconstrainedSimulatedObservablesFromObjects.size( ) );
    BOOST_CHECK_EQUAL( numberOfObsevables, constrainedSimulatedObservablesFromObjects.size( ) );

    BOOST_CHECK_EQUAL( numberOfObsevables, constrainedSimulatedObservables.size( ) );
    BOOST_CHECK_EQUAL( numberOfObsevables, constrainedSimulatedObservablesFromObjects.size( ) );

    std::map< ObservableType, std::map< LinkEnds, std::pair< Eigen::VectorXd, std::vector< double > > > >::iterator unconstrainedIterator =
            unconstrainedSimulatedObservables.begin( );
    std::map< ObservableType, std::map< LinkEnds, std::pair< Eigen::VectorXd, std::vector< double > > > >::iterator unconstrainedIteratorFromObjects =
            unconstrainedSimulatedObservablesFromObjects.begin( );

    std::map< ObservableType, std::map< LinkEnds, std::pair< Eigen::VectorXd, std::vector< double > > > >::iterator constrainedIterator =
            constrainedSimulatedObservables.begin( );
    std::map< ObservableType, std::map< LinkEnds,  std::pair< Eigen::VectorXd, std::vector< double > > > >::iterator constrainedIteratorFromObjects =
            constrainedSimulatedObservablesFromObjects.begin( );

    std::vector< double > linkEndTimes;
    std::vector< Eigen::Vector6d > linkEndStates;


    for( int i = 0; i < numberOfObsevables; i++ )
    {
        int numberOfLinkEnds = testLinkEndsList.at( unconstrainedIterator->first ).size( );
        int currentObservableSize = getObservableSize( unconstrainedIterator->first );

        std::cout<<"Obs: "<<unconstrainedIterator->first<<std::endl;

        BOOST_CHECK_EQUAL( numberOfLinkEnds, unconstrainedIterator->second.size( ) );
        BOOST_CHECK_EQUAL( numberOfLinkEnds, unconstrainedIteratorFromObjects->second.size( ) );

        BOOST_CHECK_EQUAL( numberOfLinkEnds, constrainedIterator->second.size( ) );
        BOOST_CHECK_EQUAL( numberOfLinkEnds, constrainedIteratorFromObjects->second.size( ) );

        std::map< LinkEnds, std::pair< Eigen::VectorXd, std::vector< double > > >::iterator unconstrainedLinkIterator =
                unconstrainedIterator->second.begin( );
        std::map< LinkEnds, std::pair< Eigen::VectorXd, std::vector< double > > >::iterator unconstrainedLinkIteratorFromObjects =
                unconstrainedIteratorFromObjects->second.begin( );
        std::map< LinkEnds, std::pair< Eigen::VectorXd, std::vector< double > > >::iterator constrainedLinkIterator =
                constrainedIterator->second.begin( );
        std::map< LinkEnds, std::pair< Eigen::VectorXd, std::vector< double > > >::iterator constrainedLinkIteratorFromObjects =
                constrainedIteratorFromObjects->second.begin( );

        ObservableType currentObservable = unconstrainedIterator->first;

        boost::shared_ptr< ObservationSimulatorBase< double, double > > currentObservationSimulator =
                observationSimulators.at( currentObservable );

        for( int j = 0; j < numberOfLinkEnds; j++ )
        {
            LinkEnds currentLinkEnds = unconstrainedLinkIterator->first;

            std::vector< boost::shared_ptr< ObservationViabilityCalculator > > currentViabilityCalculators =
                    viabilityCalculators.at( currentObservable ).at( currentLinkEnds );
            BOOST_CHECK_EQUAL( unconstrainedNumberOfObservations, unconstrainedLinkIterator->second.second.size( ) );
            BOOST_CHECK_EQUAL( unconstrainedNumberOfObservations, unconstrainedLinkIteratorFromObjects->second.second.size( ) );

            BOOST_CHECK_EQUAL( unconstrainedNumberOfObservations * currentObservableSize, unconstrainedLinkIterator->second.first.rows( ) );
            BOOST_CHECK_EQUAL( unconstrainedNumberOfObservations * currentObservableSize, unconstrainedLinkIteratorFromObjects->second.first.rows( ) );

            BOOST_CHECK_EQUAL( constrainedLinkIteratorFromObjects->second.second.size( ), constrainedLinkIterator->second.second.size( ) );
            BOOST_CHECK_EQUAL( constrainedLinkIteratorFromObjects->second.first.rows( ), constrainedLinkIterator->second.first.rows( ) );

            BOOST_CHECK_EQUAL( constrainedLinkIteratorFromObjects->second.first.rows( ),
                               constrainedLinkIteratorFromObjects->second.second.size( ) * currentObservableSize );


            int unconstrainedIndex = 0;
            int constrainedIndex = 0;

            bool currentObservationWasViable;
            bool currentObservationIsViable;

            bool isSingleViabilityConditionMet;

            Eigen::VectorXd currentObservation;

            while( unconstrainedIndex < unconstrainedNumberOfObservations )
            {
                std::pair< Eigen::VectorXd, std::vector< double > > unconstrainedSingleLinkObservations
                        = unconstrainedLinkIterator->second;
                std::pair< Eigen::VectorXd, std::vector< double > > constrainedSingleLinkObservations
                        = constrainedLinkIterator->second;

                if( constrainedIndex >= static_cast< int >( constrainedSingleLinkObservations.second.size( ) ) )
                {
                    currentObservationWasViable = false;
                }
                else if( unconstrainedSingleLinkObservations.second.at( unconstrainedIndex ) ==
                         constrainedSingleLinkObservations.second.at( constrainedIndex ) )
                {
                    currentObservationWasViable = true;
                }
                else
                {
                    currentObservationWasViable = false;
                }

                currentObservation = currentObservationSimulator->simulateObservation(
                            unconstrainedObservationTimes.at( unconstrainedIndex ), currentLinkEnds, referenceLinkEnd,
                            linkEndTimes, linkEndStates );

                currentObservationIsViable = true;
                for( unsigned int k = 0; k < currentViabilityCalculators.size( ); k++ )
                {
                    isSingleViabilityConditionMet = currentViabilityCalculators.at( k )->isObservationViable(
                                linkEndStates, linkEndTimes );
                    if( !isSingleViabilityConditionMet )
                    {
                        currentObservationIsViable = false;
                    }
                }

                std::vector< double > marsElevationAngles = getBodyLinkElevationAngles(
                            currentLinkEnds, currentObservable, "Mars", linkEndStates, linkEndTimes, bodyMap );
                bool computedViability = true;
                for( unsigned int l = 0; l < marsElevationAngles.size( ); l++ )
                {
                    if( marsElevationAngles.at( l ) < marsLimitAngle )
                    {
                        computedViability =  false;
                    }
                }

                std::vector< double > earthElevationAngles = getBodyLinkElevationAngles(
                            currentLinkEnds, currentObservable, "Earth", linkEndStates, linkEndTimes, bodyMap );
                for( unsigned int l = 0; l < earthElevationAngles.size( ); l++ )
                {
                    if( earthElevationAngles.at( l ) < earthLimitAngle )
                    {
                        computedViability =  false;
                    }
                }

                std::vector< double > earthSunCosineAvoidanceAngles = getBodyCosineAvoidanceAngles(
                            currentLinkEnds, currentObservable, "Earth", "Sun", linkEndStates, linkEndTimes, bodyMap );
                for( unsigned int l = 0; l < earthSunCosineAvoidanceAngles.size( ); l++ )
                {
                    if( earthSunCosineAvoidanceAngles.at( l ) > std::cos( earthSunAvoidanceAngle ) )
                    {
                        computedViability =  false;
                    }
                }


                std::vector< double > marsSunCosineAvoidanceAngles = getBodyCosineAvoidanceAngles(
                            currentLinkEnds, currentObservable, "Mars", "Sun", linkEndStates, linkEndTimes, bodyMap );
                for( unsigned int l = 0; l < marsSunCosineAvoidanceAngles.size( ); l++ )
                {
                    if( marsSunCosineAvoidanceAngles.at( l ) > std::cos( marsSunAvoidanceAngle ) )
                    {
                        computedViability =  false;
                    }
                }

                std::vector< double > moonLineOfSightDistances = getDistanceBetweenLineOfSightVectorAndPoint(
                            "Moon", linkEndStates, linkEndTimes, bodyMap );
                for( unsigned int l = 0; l < moonLineOfSightDistances.size( ); l++ )
                {
                    if( moonLineOfSightDistances.at( l ) < moonRadius )
                    {
                        computedViability =  false;
                    }
                }



                BOOST_CHECK_EQUAL( currentObservationIsViable, currentObservationWasViable );
                BOOST_CHECK_EQUAL( computedViability, currentObservationWasViable );


                if( currentObservationWasViable )
                {
                    constrainedIndex++;
                }
                unconstrainedIndex++;
            }
            BOOST_CHECK_EQUAL( constrainedIndex, constrainedLinkIteratorFromObjects->second.second.size( ) );


            unconstrainedLinkIterator++;
            unconstrainedLinkIteratorFromObjects++;
            constrainedLinkIterator++;
            constrainedLinkIteratorFromObjects++;
        }

        unconstrainedIterator++;
        unconstrainedIteratorFromObjects++;
        constrainedIterator++;
        constrainedIteratorFromObjects++;
    }
}

BOOST_AUTO_TEST_SUITE_END( )

}

}
