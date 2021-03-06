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

#ifndef TEMOTO_RESOURCE_REGISTRAR__RR_CLIENT_BASE_H
#define TEMOTO_RESOURCE_REGISTRAR__RR_CLIENT_BASE_H

#include "rr_catalog.h"
#include "rr_identifiable.h"
#include "rr_query_base.h"
#include "rr_status.h"

#include <iostream>
#include <string>
#include <vector>

namespace temoto_resource_registrar
{
  class RrClientBase : public Identifiable<std::string>
  {
  public:
    RrClientBase(const std::string &rr, const std::string &name);

    virtual void wrappedCallback();

    virtual std::string id() const;

    void setCatalog(const RrCatalogPtr &reg);

    virtual void invoke(const RrQueryBase &query) const;

    virtual void internalStatusCallback(const std::string &request_id, const Status &status);

    template <class UserStatusCb>
    void registerUserStatusCb(const std::string &request_id, const UserStatusCb &user_status_cb);

    void registerUserStatusCb(const std::string &request_id, void *const &t);

    virtual bool hasRegisteredCb(const std::string &request_id) const;

    virtual std::vector<std::string> registeredCallbackQueries() const;

    std::string rr() const;

  protected:
    std::string rr_;
    std::string name_;
    RrCatalogPtr rr_catalog_;
    std::vector<std::string> status_query_ids_;

  private:
    std::string id_;
  };

} // namespace temoto_resource_registrar

#endif