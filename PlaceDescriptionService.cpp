#include "PlaceDescriptionService.h"

#include <string>

#include "Address.h"
#include "AddressExtractor.h"
#include "Http.h"

PlaceDescriptionService::PlaceDescriptionService(Http* http) : http_(http) {}

std::string PlaceDescriptionService::summaryDescription(
      const std::string& latitude, const std::string& longitude) const {
   auto request = createGetRequestUrl(latitude, longitude);
   auto response = get(request);
   return summaryDescription(response);
}

std::string PlaceDescriptionService::summaryDescription(
      const std::string& response) const {
   AddressExtractor extractor;
   auto address = extractor.addressFrom(response);
   return address.summaryDescription();
}

std::string PlaceDescriptionService::get(const std::string& url) const {
   http_->initialize();
   auto response = http_->get(url);

   return response;
}

std::string PlaceDescriptionService::createGetRequestUrl(
      const std::string& latitude, const std::string& longitude) const {
   std::string server{"http://open.mapquestapi.com/"};
   std::string document{"nominatim/v1/reverse"};
   return server + document + "?" +
          keyValue("format", "json") + "&" +
          keyValue("lat", latitude) + "&" +
          keyValue("lon", longitude);
}

std::string PlaceDescriptionService::keyValue(
      const std::string& key, const std::string& value) const {
   return key + "=" + value;
}
