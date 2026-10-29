#line 1 "G:\\My Drive\\Trabajo\\IIE\\Actividades_Externas\\Digital\\Arduino\\03 Data logger Arduino\\sketches_datalogger\\PLACAS\\LiliGo_TDisplay\\ESP32_SHT31_TDisplay_Sheets_sleep\\lgfx_user.h"
#define LGFX_USE_V1

#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7789 panel;
  lgfx::Bus_SPI bus;
  lgfx::Light_PWM light;

public:
  LGFX(void) {
    { // Configuración del bus SPI
      auto cfg = bus.config();
      cfg.spi_host = VSPI_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read  = 16000000;
      cfg.pin_sclk = 18;
      cfg.pin_mosi = 19;
      cfg.pin_miso = -1;
      cfg.pin_dc   = 16;
      bus.config(cfg);
      panel.setBus(&bus);
    }
    { // Configuración del panel ST7789
      auto cfg = panel.config();
      cfg.pin_cs   = 5;
      cfg.pin_rst  = 23;
      cfg.pin_busy = -1;
      cfg.panel_width  = 135;
      cfg.panel_height = 240;
      cfg.offset_x = 52;
      cfg.offset_y = 40;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1;
      cfg.readable = false;
      cfg.invert   = true;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      panel.config(cfg);
    }
    { // Configuración de la retroiluminación (backlight, GPIO4)
      auto cfg = light.config();
      cfg.pin_bl = 4;
      cfg.invert = false;
      cfg.freq   = 44100;
      cfg.pwm_channel = 7;
      light.config(cfg);
      panel.setLight(&light);
    }
    setPanel(&panel);
  }
};
