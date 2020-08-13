#ifndef __DEBOUNCED_BUTTON_HPP__
#define __DEBOUNCED_BUTTON_HPP__

#include "gpio.h"
#include "ApplicationSettings.h"

// Durations should be up to bit length of click_state - 1
template<uint8_t debounce_duration = 7, uint8_t long_press_duration = 31>
class DebouncedButton {
public:
    DebouncedButton(GPIO_TypeDef *port_click, uint32_t pin_click)
        : port_click(port_click), pin_click(pin_click) {};
    ~DebouncedButton() = default;

    void update() {
        click_state <<= 1;
        click_state |= getPin(port_click, pin_click);
    }        

    inline bool isPressed() const {
        return (click_state & short_click_mask) == short_click_mask;
    }

    inline bool isLongCLick() const {
        return (click_state & long_click_mask) == long_click_mask;
    }

    inline bool isRisingEdge() const {
        return (click_state & short_click_mask) == click_mask_rising; 
    }

    inline bool isFallingEdge() const {
        return (click_state & short_click_mask) == click_mask_rising; 
    }

private:
    GPIO_TypeDef *port_click;
    uint32_t pin_click;
    uint32_t click_state;

    static constexpr uint32_t click_mask_rising = ~(1 << debounce_duration);
    static constexpr uint32_t click_mask_falling = 1 << debounce_duration;
    static constexpr uint32_t short_click_mask = ~(1 << debounce_duration + 1);
    static constexpr uint32_t long_click_mask = ~(1 << long_press_duration);
};

#endif