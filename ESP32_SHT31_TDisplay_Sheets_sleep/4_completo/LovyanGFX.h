#include <LovyanGFX.hpp>


class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7789  _panel_instance;
  lgfx::Bus_SPI       _bus_instance;
  lgfx::Light_PWM     _light_instance;

public:
  LGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = VSPI_HOST;
      cfg.spi_mode = 3;
      cfg.freq_write = 40000000;
      cfg.pin_sclk = 18;
      cfg.pin_mosi = 19;
      cfg.pin_miso = -1;
      cfg.pin_dc   = 16;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs           = 5;
      cfg.pin_rst          = 23;
      cfg.pin_busy         = -1;
      cfg.panel_width      = 135;
      cfg.panel_height     = 240;
      cfg.offset_x         = 52;
      cfg.offset_y         = 40;
      cfg.invert           = true;
      cfg.rgb_order        = false;
      _panel_instance.config(cfg);
    }
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = 4;
      cfg.invert = false;
      cfg.freq   = 44100;
      cfg.pwm_channel = 7;
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }
    setPanel(&_panel_instance);
  }
};

static LGFX lcd;
