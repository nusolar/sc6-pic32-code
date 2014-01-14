#ifndef NU_SPI_H
#define NU_SPI_H 1

/*
 * NU_SPI_PLATFORM_INIT
 * NU_INIT_SPI_PLATFORM
 * nu_init_spi_platform_args_t
 * (optional) nu_init_spi_platform_defaults
 * nu_spi_platform_setup
 * nu_spi_platform_setup_args_t
 * (optional) nu_spi_platform_setup_defaults
 */
#include "nu/platform/spi.h"
#include "nu/peripheral/pinctl.h"
#include "nu/compiler.h"
#include "nu/types.h"

#ifdef __cplusplus
extern "C" {
#endif

	struct nu_spi {
		struct nu_pin cs;
		struct nu_spi_platform platform;
	};

	static ALWAYSINLINE void
	NU_INIT_SPI(struct nu_spi *s,
			const nu_init_pin_platform_args_t *pa,
			const nu_init_spi_platform_args_t *a)
	{
		NU_INIT_PIN_PLATFORM(&(s->cs.platform), pa);
		NU_INIT_SPI_PLATFORM(&(s->platform), a);
	}

	static ALWAYSINLINE void
	nu_spi_cs_high(const struct nu_spi *s)
	{
		nu_pin_set(&(s->cs));
	}

	static ALWAYSINLINE void
	nu_spi_cs_low(const struct nu_spi *s)
	{
		nu_pin_clear(&(s->cs));
	}

	void
	nu_spi_setup(struct nu_spi *s, u32 bitrate,
			const struct nu_spi_platform_setup_args *args);

	size_t
	nu_spi_tx(const struct nu_spi *s, const void *src, size_t n);

	size_t
	nu_spi_rx(const struct nu_spi *s, void *dst, size_t n);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
