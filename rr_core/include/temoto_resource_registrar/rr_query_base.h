/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2020 TeMoto Telerobotics
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef TEMOTO_RESOURCE_REGISTRAR__RR_QUERY_BASE_H
#define TEMOTO_RESOURCE_REGISTRAR__RR_QUERY_BASE_H

#include "rr_query_request.h"
#include "rr_query_response.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace temoto_resource_registrar
{
  class RrQueryBase
  {
  public:
    virtual ~RrQueryBase() = default;

    template <class MT>
    MT request() const;

    template <class MT>
    void storeResponse(MT &resp);

    RrQueryResponse response();

    void setId(const std::string &id)
    {
      requestId_ = id;
    };

    std::string id() const
    {
      return requestId_;
    }

    void includeDependency(const std::string &id)
    {
      dependentQueryIds_.insert(id);
    }

    std::set<std::string> dependencies()
    {
      return dependentQueryIds_;
    }

  protected:
    std::string requestId_;
    std::set<std::string> dependentQueryIds_;

  private:
  };

} // namespace temoto_resource_registrar

#endif