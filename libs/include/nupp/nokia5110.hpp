#ifndef NUPP_NOKIA5110_HPP
#define NUPP_NOKIA5110_HPP 1

#include "nupp/spi.hpp"

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
	struct Nokia5110: protected SPI {
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

		DigitalOut reset, dc;
		static const uint16_t lcd_x = 84;
		static const uint16_t lcd_y = 48;

		Nokia5110(Pin _cs, SpiChannel _chn, Pin _reset, Pin _dc);
		void cmd_func_set(cmd_func_set_options opts);
		void cmd_set_vop(uint8_t vop);
		void ALWAYSINLINE cmd_set_contrast(uint8_t vop) {cmd_set_vop(vop);}
		void cmd_set_temp_coeff(cmd_func_set_options coeff);
		void cmd_set_bias(uint8_t bias);
		void cmd_set_disp_mode(cmd_func_set_options mode);


		void put_c(unsigned char c);
		void puts(unsigned char *str);
		DEPRECATED void PRINTF(2,3) printf(const char *fmt, ...);

		/**
		 * Goto column x, line y. 84 columns, 6 lines.
		 */
		ALWAYSINLINE void goto_xy(uint8_t x, uint8_t y) {
			cmd_set_ram_x_addr(x);
			cmd_set_ram_y_addr(y);
		}

		/**
		 * Set one pixel. UNUSED
		 * The LCD has 6 rows, with 8 pixels per row.
		 * 'y_row' is the row that the pixel is in.
		 * 'y_pix' is the pixel in that row we want to enable/disable
		 */
		UNUSED void set_pixel(uint8_t x, uint8_t y) {
			if (x >= lcd_x || y >= lcd_y) return;

			uint8_t y_row = (uint8_t)(y >> 3);          // >>3 divides by 8
			uint8_t y_pix = (uint8_t)(y-(y_row << 3));  // <<3 multiplies by 8
			uint8_t val   = (uint8_t)(1 << y_pix);

			goto_xy(x, y_row); // Write pixel out to LCD
			write_data(val);
		}

		ALWAYSINLINE void lcd_clear(){
			goto_xy(0,0);
			for (uint32_t ui = 0; ui < (lcd_x * 6); ui++) // WARNING
				write_data(0x00);
		}

	private:
		ALWAYSINLINE void write_cmd(uint8_t cmd) {
			dc.low();
			tx(&cmd, 1);
		}
		ALWAYSINLINE void write_data(uint8_t data) {
			dc.high();
			tx(&data, 1);
		}

		void cmd_set_ram_x_addr(uint8_t x);
		void cmd_set_ram_y_addr(uint8_t y);
	};
}


#endif /* defined(__nusolar_lib__nokia5110__) */
