#include "Http.h"
#include "PlaceDescriptionService.h"
#include "gmock/gmock.h"

using ::testing::_;
using ::testing::EndsWith;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::InSequence;

class APlaceDescriptionService : public testing::Test {
 public:
  static const std::string ValidLatitude;
  static const std::string ValidLongitude;
};

const std::string APlaceDescriptionService::ValidLatitude("38.005");
const std::string APlaceDescriptionService::ValidLongitude("-104.44");

class HttpStub : public Http {
 public:
  MOCK_METHOD(void, initialize, (), (override));
  MOCK_METHOD(std::string, get, (const std::string&), (const, override));
};

TEST_F(APlaceDescriptionService, MakesHttpRequestToObtainAddress) {
  InSequence forceExpectationOrder;
  HttpStub httpStub;
  std::string urlStart{
      "http://open.mapquestapi.com/nominatim/v1/reverse?format=json&"};

  auto expectedURL = urlStart +
                     "lat=" + APlaceDescriptionService::ValidLatitude + "&" +
                     "lon=" + APlaceDescriptionService::ValidLongitude;

  EXPECT_CALL(httpStub, initialize());
  EXPECT_CALL(httpStub, get(expectedURL));

  PlaceDescriptionService service{&httpStub};

  service.summaryDescription(ValidLatitude, ValidLongitude);
}

TEST_F(APlaceDescriptionService, FormatRetrievedAddressIntoSummaryDescription) {
  NiceMock<HttpStub> httpStub;

  EXPECT_CALL(httpStub, get(_))
      .WillOnce(Return(
          R"({ "address": {
              "road":"Drury Ln",
              "city":"Fountain",
              "state":"CO",
              "country":"US" }})"));

  PlaceDescriptionService service{&httpStub};
  auto description = service.summaryDescription(ValidLatitude, ValidLongitude);

  ASSERT_THAT(description, Eq("Drury Ln, Fountain, CO, US"));
}
