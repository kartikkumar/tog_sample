/*
 * Copyright (c) 2014-2015 Kartik Kumar (me@kartikkumar.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef TOG_TYPEDEFS_HPP
#define TOG_TYPEDEFS_HPP

#include <map>
#include <vector>

#include <rapidjson/document.h>

namespace tog
{

//! 3-Vector.
typedef std::vector< double > Vector3;

//! 6-Vector.
typedef std::vector< double > Vector6;

//! State history.
typedef std::map< double, Vector6 > StateHistory;

//! JSON config iterator.
typedef rapidjson::Value::ConstMemberIterator ConfigIterator;

} // namespace d2d

#endif // TOG_CATALOG_PRUNER_HPP
