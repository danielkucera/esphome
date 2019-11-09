import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, sensor
from esphome.const import CONF_ID, CONF_SENSOR, CONF_SUPPORTS_HEAT, CONF_MAC_ADDRESS

AUTO_LOAD = ['climate']

cometblue_ns = cg.esphome_ns.namespace('cometblue')
CometblueClimate = cometblue_ns.class_('CometblueClimate', climate.Climate, cg.Component)

CONFIG_SCHEMA = cv.All(climate.CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(CometblueClimate),
    cv.Required(CONF_MAC_ADDRESS): cv.mac_address,
    cv.Optional(CONF_SENSOR): cv.use_id(sensor.Sensor),
}).extend(cv.COMPONENT_SCHEMA))


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)

    cg.add(var.set_mac(config[CONF_MAC_ADDRESS].__str__()))

    cg.add_library('https://github.com/MrKrabat/Comet-Blue-Thermostat', '')

