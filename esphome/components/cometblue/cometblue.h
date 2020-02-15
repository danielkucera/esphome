#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/climate/climate.h"

namespace esphome {
namespace cometblue {

class CometblueClimate : public climate::Climate, public Component {
 public:
  void setup() override;
  void update_ext();

  void set_mac(std::string mac) { mac_ = mac; }
  void set_supports_cool(bool supports_cool) { this->supports_cool_ = supports_cool; }
  void set_supports_heat(bool supports_heat) { this->supports_heat_ = supports_heat; }

 protected:
  /// Override control to change settings of the climate device.
  void control(const climate::ClimateCall &call) override;
  /// Return the traits of this controller.
  climate::ClimateTraits traits() override;

  int connect();
  void update();
  static void update_task(CometblueClimate* parent);

  bool supports_cool_{true};
  bool supports_heat_{true};
  std::string mac_;

};

}  // namespace cometblue
}  // namespace esphome
