#ifndef TEMOTO_RESOURCE_REGISTRAR__ROS_QUERY_H
#define TEMOTO_RESOURCE_REGISTRAR__ROS_QUERY_H

#include "temoto_resource_registrar/rr_query_base.h"
#include "temoto_resource_registrar/rr_serializer.h"
#include "rr_interfaces/msg/temoto_request_metadata.hpp"
#include "rr_interfaces/msg/temoto_response_metadata.hpp"

/**
 * @brief A wrapper class for temoto_resource_registrar::RrQueryBase. Used as a intermediery container to hande data by the ResourceRegistrar.
 * 
 * @tparam ServiceClass - Type of the server, a ROS2 srv
 */
template <class ServiceClass>
class Ros2Query : public temoto_resource_registrar::RrQueryBase
{
public:
  Ros2Query() {}

  /**
   * @brief Construct a new Ros2 Query object. Does not store existing field valus of the request.
   * 
   * @param metadata 
   */
  Ros2Query(const rr_interfaces::msg::TemotoRequestMetadata &request_data,
            const rr_interfaces::msg::TemotoResponseMetadata &response_data)
  {
    setRr(request_data.serving_rr);
    setOrigin(request_data.origin_rr);

    setId(response_data.request_id);
    setStatus(response_data.status);

    /*
    if (request_data.metadata.size() > 0)
      setRequestMetadata(Serializer::deserialize<temoto_resource_registrar::RequestMetadata>(request_data.metadata));
*/
    if (response_data.metadata.size() > 0)
      setResponseMetadata(Serializer::deserialize<temoto_resource_registrar::ResponseMetadata>(response_data.metadata));
  }

  Ros2Query(const std::shared_ptr<typename ServiceClass::Request> &request)
  {
    setRr(request->temoto_metadata.serving_rr);
    setOrigin(request->temoto_metadata.origin_rr);

    if (request->temoto_metadata.metadata.size() > 0)
      setRequestMetadata(Serializer::deserialize<temoto_resource_registrar::RequestMetadata>(request->temoto_metadata.metadata));

    typed_query_req_ = request;
  }

  void response(const std::shared_ptr<typename ServiceClass::Response> &response)
  {
    typed_query_res_ = response;

    setId(response->temoto_metadata.request_id);
    setStatus(response->temoto_metadata.status);

    if (response->temoto_metadata.metadata.size() > 0)
      setResponseMetadata(Serializer::deserialize<temoto_resource_registrar::ResponseMetadata>(response->temoto_metadata.metadata));
  }

  /**
   * @brief Returs the reference of the stored srv request.
   * 
   * @return ServiceClass::Request 
   */
  std::shared_ptr<typename ServiceClass::Request> request()
  {
    typed_query_req_->temoto_metadata.serving_rr = rr();
    typed_query_req_->temoto_metadata.origin_rr = origin();

    typed_query_req_->temoto_metadata.metadata = Serializer::serialize<temoto_resource_registrar::RequestMetadata>(requestMetadata());

    return typed_query_req_;
  }

  /**
 * @brief Returs the reference of the stored srv response.
 * 
 * @return ServiceClass::Response 
 */
  std::shared_ptr<typename ServiceClass::Response> response()
  {
    typed_query_res_->temoto_metadata.request_id = id();
    typed_query_res_->temoto_metadata.status = status();
    typed_query_res_->temoto_metadata.metadata = Serializer::serialize<temoto_resource_registrar::ResponseMetadata>(responseMetadata());

    return typed_query_res_;
  }

  /**
   * @brief Modifies an existing request and response with metadata from the resourecRegistrar.
   * 
   * @param req - ServiceClass::Request that will be modified
   * @param res - ServiceClass::Response that will be modified
   */
  void rosQuery(typename std::shared_ptr<typename ServiceClass::Request> &req, std::shared_ptr<typename ServiceClass::Response> &res)
  {
    std::shared_ptr<ServiceClass> sq = rosQuery();

    req->temoto_metadata = sq->request.temoto_metadata;
    res->temoto_metadata = sq->response.temoto_metadata;
  }

  /**
   * @brief Returns a new ServiceClass type object that has its metadata filled.
   * 
   * @return ServiceClass 
   */
  /*
  std::shared_ptr<ServiceClass> rosQuery()
  {
    //ROS_INFO_STREAM("Returning ROS query");

    typed_query_req_->request.temoto_metadata.serving_rr = rr();
    typed_query_req_->request.temoto_metadata.origin_rr = origin();
    typed_query_req_->request.temoto_metadata.metadata = Serializer::serialize<temoto_resource_registrar::RequestMetadata>(requestMetadata());

    typed_query_res_->response.temoto_metadata.request_id = id();
    typed_query_res_->response.temoto_metadata.status = status();
    typed_query_res_->response.temoto_metadata.metadata = Serializer::serialize<temoto_resource_registrar::ResponseMetadata>(responseMetadata());

    return typed_query_;
  }
  */

protected:
  std::shared_ptr<typename ServiceClass::Request> typed_query_req_;
  std::shared_ptr<typename ServiceClass::Response> typed_query_res_;

private:
};
#endif