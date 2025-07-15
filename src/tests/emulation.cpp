#include "tests/emulation.hpp"

#include <zephyr/devicetree.h>
#include <zephyr/drivers/fuel_gauge.h>

#include <zephyr/drivers/emul_fuel_gauge.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(emulation);

void test_emulated_fuel_gauge() {
    /*
     Fuel gauge is a device on the emulated "zephyr,i2c-emul-controller" I2C bus,
     which in turn does not communicate with a physical bus, but instead forwards the transfers to
     emulated devices (defined with the EMUL_DT_INST_DEFINE macro).

     In our case:
     The "driver" is defined at projects/zephyr/drivers/fuel_gauge/sbs_gauge/sbs_gauge.c
        * Enabled by the CONFIG_FUEL_GAUGE=y option (in combination with the compatible devicetree node);
     The "hw device" is defined at projects/zephyr/drivers/fuel_gauge/sbs_gauge/emul_sbs_gauge.c
        * Enabled by the CONFIG_EMUL_SBS_GAUGE=y option (implied by CONFIG_EMUL=y);
     The frontend is glued to the backend by the I2C emulator at projects/zephyr/drivers/i2c/i2c_emul.c
        * Uses a sys_list to track all emulators;
        * Forwards I2C transfers directly to the emulator based on the device address;
        * Enabled by the CONFIG_EMUL=y option (in combination with the compatible devicetree node);
     Finally, the "backend API" for injecting data into the emulator is defined at
     projects/zephyr/include/zephyr/drivers/emul_fuel_gauge.h
    */
    static const device* dev = DEVICE_DT_GET(DT_NODELABEL(fuel_gauge));
    static const emul* emul = EMUL_DT_GET(DT_NODELABEL(fuel_gauge));

    LOG_INF("Using fuel gauge %s", dev->name);

    // Read manufacturer name:
    struct sbs_gauge_manufacturer_name mfg_name;
    int ret = fuel_gauge_get_buffer_prop(dev, FUEL_GAUGE_MANUFACTURER_NAME, &mfg_name, sizeof(mfg_name));
    LOG_INF("Fuel gauge manufacturer name: %s", mfg_name.manufacturer_name);
    __ASSERT(strcmp(mfg_name.manufacturer_name, "ACME") == 0, "Unexpected manufacturer name");

    // Read voltage property before any artificial injection:
    fuel_gauge_prop_t voltage_prop = FUEL_GAUGE_VOLTAGE;
    fuel_gauge_prop_val voltage_val;
    ret = fuel_gauge_get_props(dev, &voltage_prop, &voltage_val, 1);
    LOG_INF("Fuel gauge voltage before backend injection: %d", voltage_val.voltage);

    // Inject new voltage value:
    const uint32_t uV = 3300;
    const int uA = 1000;
    if (emul_fuel_gauge_set_battery_charging(emul, uV, uA) != 0) {
        LOG_ERR("Failed to access emulator backend!");
    }

    // Read voltage property after artificial injection:
    ret = fuel_gauge_get_props(dev, &voltage_prop, &voltage_val, 1);
    LOG_INF("Fuel gauge voltage after backend injection: %d", voltage_val.voltage);
}

void test_emulation() { test_emulated_fuel_gauge(); }
