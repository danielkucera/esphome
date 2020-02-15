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
  traits.set_supports_current_temperature(true);
  traits.set_supports_auto_mode(false);
  traits.set_supports_cool_mode(false);
  traits.set_supports_heat_mode(true);
  traits.set_supports_two_point_target_temperature(false);
  traits.set_supports_away(false);
  traits.set_supports_action(true);
  traits.set_visual_min_temperature(COMETBLUE_TEMP_MIN);
  traits.set_visual_max_temperature(COMETBLUE_TEMP_MAX);
  traits.set_visual_temperature_step(0.5);
  return traits;
}

int CometblueClimate::connect() {
  if (!dev->isConnected() && !dev->connect(mac_, 0)){ //TODO:pin from config
    ESP_LOGCONFIG(TAG, "Connect to '%s' FAILED!", this->mac_.c_str());
    return 0;
  }
  ESP_LOGCONFIG(TAG, "Connected to '%s' '%s'", this->mac_.c_str(), dev->getModelNumber().c_str());

  return 1;
}

void CometblueClimate::setup() {
  ESP_LOGCONFIG(TAG, "Setting up CometBlue client for '%s'...", this->mac_.c_str());

  this->mode = climate::CLIMATE_MODE_HEAT; // we can only heat
  this->current_temperature = NAN;
  this->target_temperature = NAN;

  //safe to do here?
  esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);

  dev = new CometBlue(mac_);

  //TODO: dynamic interval
  this->set_interval(300*1000, [this] { this->update(); });

  update();
}

static void CometblueClimate::update_task(CometblueClimate* parent) {
  parent->update_ext();
}

void CometblueClimate::update_ext() {
  ESP_LOGCONFIG(TAG, "Updating state of '%s'...", this->mac_.c_str());
  if (!this->connect()){
    return;
  }

  Temperatures temp = dev->getTemperatures();

  this->current_temperature = temp.current_temp;
  this->target_temperature = temp.manual_temp;

  if (temp.current_temp < temp.manual_temp){
    this->action = climate::CLIMATE_ACTION_HEATING;
  } else {
    this->action = climate::CLIMATE_ACTION_IDLE;
  }

  this->publish_state();
  dev->disconnect();
}

void CometblueClimate::update() {
//  xTaskCreate(
  //                  update_task,       /* Task function. */
    //                "CometblueUpdate", /* String with name of task. */
      //              10000,             /* Stack size in bytes. */
        //            (void *)&this,      /* Parameter passed as input of the task */
          //          1,                 /* Priority of the task. */
            //        NULL);             /* Task handle. */
//}

void CometblueClimate::control(const climate::ClimateCall &call) {
  if (call.get_target_temperature().has_value()){
    if (connect()){
      Temperatures temp = dev->getTemperatures();
      temp.manual_temp = *call.get_target_temperature(); // target temperature in degree Celsius
      ESP_LOGCONFIG(TAG, "About to set target temperature for '%s' to %f", this->mac_.c_str(), temp.manual_temp);
      //TODO: fix 64deg currrent temp
      if (!dev->setTemperatures(temp)){
        ESP_LOGCONFIG(TAG, "Seting temperature failed!");
      }
    }
  }
  dev->disconnect();

  //TODO: do asynchronously?
  update();
}

}  // namespace cometblue
}  // namespace esphome
