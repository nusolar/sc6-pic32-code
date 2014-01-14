#ifndef NU_SPI_H
#define NU_SPI_H 1

/*
 * NU_SPI_PLATFORM_INIT
 * NU_INIT_SPI_PLATFORM
 * nu__Spi__PlatformInitArgs_t
 * (optional) nu_init_spi_platform_defaults
 * nu__Spi__platform_setup
 * nu__Spi__platform_setup_args_t
 * (optional) nu__Spi__platform_setup_defaults
 */
#include "nu/platform/spi.h"
#include "nu/peripheral/pinctl.h"
#include "nu/compiler.h"
#include "nu/types.h"

#ifdef __cplusplus
extern "C" {
#endif

	struct nu__Spi {
		struct nu__Pin cs;
		struct nu__Spi__Platform platform;
	};

	static ALWAYSINLINE void
	NU_INIT_SPI(struct nu__Spi *s,
			const nu__Pin__PlatformInitArgs_t *pa,
			const nu__Spi__PlatformInitArgs_t *a)
	{
		NU_INIT_PIN_PLATFORM(&(s->cs.platform), pa);
		NU_INIT_SPI_PLATFORM(&(s->platform), a);
	}

	static ALWAYSINLINE void
	nu__Spi__cs_high(const struct nu__Spi *s)
	{
		nu__Pin__set(&(s->cs));
	}

	static ALWAYSINLINE void
	nu__Spi__cs_low(const struct nu__Spi *s)
	{
		nu__Pin__clear(&(s->cs));
	}

	void
	nu__Spi__setup(struct nu__Spi *s, u32 bitrate,
			const struct nu__Spi__PlatformSetupArgs *args);

	size_t
	nu__Spi__tx(const struct nu__Spi *s, const void *src, size_t n);

	size_t
	nu__Spi__rx(const struct nu__Spi *s, void *dst, size_t n);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
