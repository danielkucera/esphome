#include "cometblue.h"
#include "esphome/core/log.h"
#include <CometBlue.h>

namespace esphome {
namespace cometblue {

static const char *TAG = "cometblue";

const uint8_t COMETBLUE_TEMP_MAX = 28;  // Celsius
const uint8_t COMETBLUE_TEMP_MIN = 8;  // Celsius

CometBlue* dev;

climate::ClimateTraits CometblueClimate::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(this->sensor_ != nullptr);
  traits.set_supports_auto_mode(true);
  traits.set_supports_cool_mode(false);
  traits.set_supports_heat_mode(true);
  traits.set_supports_two_point_target_temperature(false);
  traits.set_supports_away(false);
  traits.set_visual_min_temperature(COMETBLUE_TEMP_MIN);
  traits.set_visual_max_temperature(COMETBLUE_TEMP_MAX);
  traits.set_visual_temperature_step(0.5);
  return traits;
}

void CometblueClimate::setup() {
  ESP_LOGCONFIG(TAG, "Setting up CometBlue client for '%s'...", this->mac_.c_str());

  dev = new CometBlue(mac_);

  if (!dev->connect(mac_, 0)){
  	ESP_LOGCONFIG(TAG, "Connect to '%s' FAILED!", this->mac_.c_str());
	return; //why are we void?
  }
  ESP_LOGCONFIG(TAG, "Connect to '%s' SUCCESS!", this->mac_.c_str());

  Temperatures temp = dev->getTemperatures();

  this->current_temperature = temp.current_temp;
  this->publish_state();
/*
  if (this->sensor_) {
    this->sensor_->add_on_state_callback([this](float state) {
      this->current_temperature = state;
      // current temperature changed, publish state
      this->publish_state();
    });
    this->current_temperature = this->sensor_->state;
  } else
    this->current_temperature = NAN;
  // restore set points
  auto restore = this->restore_state_();
  if (restore.has_value()) {
    restore->apply(this);
  } else {
    // restore from defaults
    this->mode = climate::CLIMATE_MODE_OFF;
    this->target_temperature = 24;
  }
  */
}

void CometblueClimate::control(const climate::ClimateCall &call) {
/*
  if (call.get_mode().has_value())
    this->mode = *call.get_mode();
  if (call.get_target_temperature().has_value())
    this->target_temperature = *call.get_target_temperature();

  this->transmit_state_();
  this->publish_state();
*/
}

}  // namespace cometblue
}  // namespace esphome
