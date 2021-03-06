#ifndef NUXX_NOKIA5110_HPP
#define NUXX_NOKIA5110_HPP 1

#include "nuxx/peripheral/spi.hpp"

namespace nu {
	/***********************************
	 *  Schematic:
	 *  LCD 1-Vcc		----->	NU32v2 3.3V
	 *  LCD 2-GND		----->	NU32v2 GND
	 *  LCD 3-SCE/CS     ----->	NU32v2 E2
	 *  LCD 4-RST		----->	NU32v2 E1
	 *  LCD 5-D/C		----->	NU32v2 E0
	 *  LCD 6-DNK(MOSI)/SDO	----->	NU32v2 F8
	 *  LCD 7-SCLK		----->	NU32v2 D15
	 *  LCD 8-LED		----->	330 ohm	----->	5V
	 *  (8-LED for backlight only if desired)
	 ************************************/
	struct Nokia5110: public OStream {
		enum cmd_func_set_options {
			DC_DATA                = 0,
			DC_COMMAND             = 1<<0,

			ADDRESSING_HORIZONTAL  = 0,
			ADDRESSING_VERTICAL    = 1<<1,

			ACTIVE                 = 0,
			POWERDOWN              = 1<<2,

			DISP_MODE_BLANK        = 0,
			DISP_MODE_NORMAL       = 1<<3,
			DISP_MODE_ALL_ON       = 1<<4,
			DISP_MODE_INVERSE      = 1<<4 | 1<<3,

			INSTRUCTIONS_BASIC     = 0,
			INSTRUCTIONS_EXTENDED  = 1<<5,

			TEMP_COEFF_0           = 0,
			TEMP_COEFF_1           = 1<<6,
			TEMP_COEFF_2           = 1<<7,
			TEMP_COEFF_3           = 1<<7 | 1<<6
		};

		Spi base;
		DigitalOut reset, dc;
		static const uint16_t lcd_x = 84;
		static const uint16_t lcd_y = 48;

		Nokia5110(PlatformPin _cs, uint8_t _channel, PlatformPin _reset, PlatformPin _dc);
		void setup() {this->base.setup(); this->reconfigure();}

		void reconfigure();

		void put_c(const uint8_t c);
		void puts(const char *str);
//		DEPRECATED void PRINTF(2,3) printf(const char *fmt, ...);

		/**
		 * Goto column x, line y. 84 columns, 6 lines.
		 */
		void goto_xy(const uint8_t x, const uint8_t y) {
			cmd_set_ram_x_addr(x);
			cmd_set_ram_y_addr(y);
		}

		/**
		 * Set one pixel.
		 * The LCD has 6 rows, with 8 pixels per row.
		 * 'y_row' is the row that the pixel is in.
		 * 'y_pix' is the pixel in that row we want to enable/disable
		 */
		UNUSED void set_pixel(const uint8_t x, const uint8_t y) {
			if (x >= lcd_x || y >= lcd_y) return;

			uint8_t y_row = (uint8_t)(y >> 3);          // >>3 divides by 8
			uint8_t y_pix = (uint8_t)(y-(y_row << 3));  // <<3 multiplies by 8
			uint8_t val   = (uint8_t)(1 << y_pix);

			goto_xy(x, y_row); // Write pixel out to LCD
			write_data(val);
		}

		void lcd_clear(){
			goto_xy(0, 0);
			for (uint32_t ui = 0; ui < (lcd_x * 6); ui++) // WARNING
				write_data(0x88); // Clear with 2 dots to prevent fading
			goto_xy(0, 0);
		}

	private:
		void write_cmd(const uint8_t cmd) {
			dc.low();
			this->base.tx(&cmd, 1);
		}
		void write_data(const uint8_t data) {
			dc.high();
			this->base.tx(&data, 1);
		}

		void cmd_func_set(const cmd_func_set_options opts);
		void cmd_set_vop(const uint8_t vop);
		void cmd_set_contrast(const uint8_t vop) {cmd_set_vop(vop);}
		void cmd_set_temp_coeff(const cmd_func_set_options coeff);
		void cmd_set_bias(const uint8_t bias);
		void cmd_set_disp_mode(const cmd_func_set_options mode);
		void cmd_set_ram_x_addr(const uint8_t x);
		void cmd_set_ram_y_addr(const uint8_t y);
	};
}


#endif /* defined(__nusolar_lib__nokia5110__) */
