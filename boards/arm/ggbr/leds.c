#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/led.h>
#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zmk/hid_indicators.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/hid_indicators_changed.h>

#define LED_GPIO_NODE_ID DT_COMPAT_GET_ANY_STATUS_OKAY(gpio_leds)

// GPIO-based LED device
static const struct device *led_dev = DEVICE_DT_GET(LED_GPIO_NODE_ID);

static int led_keylock_listener_cb(const zmk_event_t *eh) {
    zmk_hid_indicators_t flags = zmk_hid_indicators_get_current_profile();
    unsigned int capsBit = 1 << (HID_USAGE_LED_CAPS_LOCK - 1);

    if (flags & capsBit) {
        led_on(led_dev, DT_NODE_CHILD_IDX(DT_ALIAS(led_caps)));
    } else {
        led_off(led_dev, DT_NODE_CHILD_IDX(DT_ALIAS(led_caps)));
    }

    return 0;
}

ZMK_LISTENER(led_indicators_listener, led_keylock_listener_cb);
ZMK_SUBSCRIPTION(led_indicators_listener, zmk_hid_indicators_changed);

// Layer state listener for layer LED
// static int led_layer_listener_cb(const zmk_event_t *eh) {
//   const struct zmk_layer_state_changed *ev = as_zmk_layer_state_changed(eh);

//   // layer NUM -> led blink NUM
//   if (ev->layer >= 2) {
//     if (ev->state) {
//       for (int i = 0; i < ev->layer; i++) {
//         led_on(led_dev, DT_NODE_CHILD_IDX(DT_ALIAS(led_caps)));
//         k_sleep(K_MSEC(300));
//         led_off(led_dev, DT_NODE_CHILD_IDX(DT_ALIAS(led_caps)));
//         k_sleep(K_MSEC(300));
//       }
//     }
//   }
//   return 0;
// }

// ZMK_LISTENER(layer_led_listener, led_layer_listener_cb);
// ZMK_SUBSCRIPTION(layer_led_listener, zmk_layer_state_changed);
