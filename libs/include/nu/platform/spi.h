#ifndef NU_PLATFORM_SPI_H
#define NU_PLATFORM_SPI_H 1

#include "nu/compiler.h"
#include "nu/types.h"
#include "nu/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

	struct nu__Spi__Platform;
	struct nu__Spi__PlatformSetupArgs;

	struct nu__Spi__PlatformOps {
		void (*setup)   (struct nu__Spi__Platform *p, u32 bitrate,
				const struct nu__Spi__PlatformSetupArgs *args);
		s32 (*putchar)  (const struct nu__Spi__Platform *p, s32 c);
		s32 (*getchar)  (const struct nu__Spi__Platform *p);
	};


	#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
	#error "Unknown NU_PLATFORM in nu/platform/spi.h"

	#elif NU_PLATFORM==NU_PLATFORM_GENERIC
	#error "No generic SPI code!"

	#elif NU_PLATFORM==NU_PLATFORM_PIC32MX
	#include <peripheral/spi.h>

	extern const struct nu__Spi__PlatformOps nu__Spi__platform_ops;
	static const struct nu__Spi__PlatformSetupArgs {
		SpiOpenFlags oflags;
	} nu__Spi__platform_setup_defaults = {
		(SpiOpenFlags)SPI_OPEN_MSTEN|SPI_OPEN_MODE8,
	};


	typedef struct nu__Spi__Platform {
		SpiChannel chn;
	} nu__Spi__PlatformInitArgs_t;
	#define NU_SPI_PLATFORM_INIT(chn) { chn }

	#define NU_SPI_CHANNEL1 { SPI_CHANNEL1 }
	#define NU_SPI_CHANNEL2 { SPI_CHANNEL2 }
	#define NU_SPI_CHANNEL3 { SPI_CHANNEL3 }
	#define NU_SPI_CHANNEL4 { SPI_CHANNEL4 }

	static ALWAYSINLINE void
	NU_INIT_SPI_PLATFORM(struct nu__Spi__Platform *p,
			const nu__Spi__PlatformInitArgs_t *args)
	{
		p->chn = args->chn;
	}

	#endif /* NU_PLATFORM switch */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NU_PLATFORM_SPI_H */
