#ifndef _COLOR_MATH_h
#define _COLOR_MATH_h

typedef struct {
    uint8_t r, g, b;
} RGB_t;

typedef struct {
    uint8_t h, s, v;
} HSV_t;

const uint8_t max_whiteness = 15;
const uint8_t max_value = 17;

const uint8_t sixth_hue = 16; // you can adjust it
const uint8_t third_hue = sixth_hue * 2;
const uint8_t half_hue = sixth_hue * 3;
const uint8_t two_thirds_hue = sixth_hue * 4;
const uint8_t five_sixths_hue = sixth_hue * 5;
const uint8_t full_hue = sixth_hue * 6;

inline RGB_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (RGB_t) {r, g, b};
}

inline HSV_t hsv(uint8_t h, uint8_t s, uint8_t v) {
    return (HSV_t) {h, s, v};
}

const RGB_t black = {0, 0, 0};

RGB_t hsv2rgb(HSV_t hsv) {
    if (hsv.v > max_value) {
        hsv.v = max_value * 2 - hsv.v;
    }

    if (hsv.v == 0) return black;
    
    uint8_t high = hsv.v * max_whiteness; //channel with max value    
    if (hsv.s == 0) return rgb(high, high, high); //grayscale
    
    uint8_t W = max_whiteness - hsv.s;
    uint8_t low = hsv.v * W; //channel with min value
    uint8_t rising = low;
    uint8_t falling = high;
    
    uint8_t h_after_sixth = hsv.h % sixth_hue;
    if (h_after_sixth > 0) {//not at primary color? ok, h_after_sixth = 1..sixth_hue - 1
        uint8_t z = hsv.s * uint8_t(hsv.v * h_after_sixth) / sixth_hue;
        rising += z;
        falling -= z + 1; //it's never 255, so ok
    }
    
    uint8_t H = hsv.h;
    while (H >= full_hue) H -= full_hue;
    
    if (H < sixth_hue) return rgb(high, rising, low);
    if (H < third_hue) return rgb(falling, high, low);
    if (H < half_hue) return rgb(low, high, rising);
    if (H < two_thirds_hue) return rgb(low, falling, high);
    if (H < five_sixths_hue) return rgb(rising, low, high);
    return rgb(high, low, falling);
}

RGB_t hsv2rgb(uint8_t H, uint8_t S, uint8_t V) {
    return hsv2rgb(hsv(H, S, V));
}

//////////////////

inline void set_led(RGB_t* leds, uint8_t led, RGB_t color) {
    leds[led] = color;
}

inline void hsv_gradient(RGB_t* leds,
                     uint8_t led_from, uint8_t H_from, uint8_t S_from, uint8_t V_from,
                     uint8_t led_to,   uint8_t H_to,   uint8_t S_to,   uint8_t V_to) {
    int8_t led_gap = led_to - led_from;
    int8_t H_gap = H_to - H_from,
           S_gap = S_to - S_from,
           V_gap = V_to - V_from;

    for (uint8_t led = 0; led <= led_gap; led++) {
        set_led(leds, led_from + led, hsv2rgb(
            H_from + H_gap * led / led_gap,
            S_from + S_gap * led / led_gap,
            V_from + V_gap * led / led_gap
        ));
    }
}

inline void rgb_gradient(RGB_t* leds,
                     uint8_t led_from, uint8_t R_from, uint8_t G_from, uint8_t B_from,
                     uint8_t led_to,   uint8_t R_to,   uint8_t G_to,   uint8_t B_to) {
    int8_t led_gap = led_to - led_from;
    int8_t R_gap = R_to - R_from,
           G_gap = G_to - G_from,
           B_gap = B_to - B_from;

    for (uint8_t led = 0; led <= led_gap; led++) {
        set_led(leds, led_from + led, rgb(
            R_from + R_gap * led / led_gap,
            G_from + G_gap * led / led_gap,
            B_from + B_gap * led / led_gap
        ));
    }
}

#endif // _COLOR_MATH_h
