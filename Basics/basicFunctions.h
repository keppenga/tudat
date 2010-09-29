/*! \file basicFunctions.h
 *    Header file that defines the basicFunctions namespace, containing all
 *    basic functions contained in Tudat.
 *
 *    Path              : /Basics/
 *    Version           : 4
 *    Check status      : Checked
 *
 *    Author            : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Checker           : D. Dirkx
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : D.Dirkx@student.tudelft.nl
 *
 *    Date created      : 10 august, 2010
 *    Last modified     : 29 september, 2010
 *
 *    References
 *      Press W.H., et al. Numerical Recipes in C++: The Art of
 *          Scientific Computing. Cambridge University Press, February 2002.
 *
 *    Notes
 *
 *    Copyright (c) 2010 Delft University of Technology.
 *
 *    This software is protected by national and international copyright.
 *    Any unauthorized use, reproduction or modificaton is unlawful and
 *    will be prosecuted. Commercial and non-private application of the
 *    software in any form is strictly prohibited unless otherwise granted
 *    by the authors.
 *
 *    The code is provided without any warranty; without even the implied
 *    warranty of merchantibility or fitness for a particular purpose.
 *
 *    Changelog
 *      YYMMDD    author              comment
 *      100902    K. Kumar            File header and footer added.
 *      100916    D. Dirkx            Added minor comments and placeholder
 *                                    tag during checking.
 *      100928    K. Kumar            Added reference and adjusted include
 *                                    statements.
 *      100929    K. Kumar            Changed EigenRoutines.h include statement
 *                                    to linearAlgebra.h and removed
 *                                    placeholder comment. Added small comment
 *                                    modifications.
 */

#ifndef BASICOPERATIONS_H
#define BASICOPERATIONS_H

// Include statements.
#include <map>
#include <iterator>
#include "linearAlgebra.h"

//! basicFunctions namespace.
/*!
 * Defintion of basicFunctions namespace.
 */
namespace basic_functions
{

//! Nearest left neighbor binary search.
/*!
 * This function searches for the nearest left neighbor in a vector of sorted
 * data using a binary algorithm (Press W.H., et al., 2002).
 *  \param vectorOfSortedData Vector of data sorted in ascending/descending
 *          order.
 *  \param targetValueInVectorOfSortedData Target value in vector of sorted
 *          data.
 *  \return Index of nearest left neighbor to target value.
 */
int computeNearestLeftNeighborUsingBinarySearch(
        Vector& vectorOfSortedData,
        double& targetValueInVectorOfSortedData );

//! Nearest left neighbor binary search.
/*!
 * This function searches for the nearest left neighbor in a map of sorted
 * data using a binary algorithm (Press W.H., et al., 2002).
 * \param sortedIndepedentAndDependentVariables Map of independent and
 *           dependent data sorted in ascending/descending order.
 * \param targetValueInMapOfData Target value in map of sorted data.
 * \return Index of nearest left neighbor to target value.
 */
int computeNearestLeftNeighborUsingBinarySearch(
        std::map < double, Vector >& sortedIndepedentAndDependentVariables,
        double& targetValueInMapOfData );

}

#endif // BASICOPERATIONS_H

// End of file.