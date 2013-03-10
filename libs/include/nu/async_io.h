#ifndef NU_ASYNC_IO_H
#define NU_ASYNC_IO_H

#include "circ_buf.h"
#include "nu_types.h"

struct async_io {
    const char *rx_delims;  /* should be null-terminated */
    struct circ_buf tx_buf;
    size_t tx_buf_size;
    struct circ_buf rx_buf;
    size_t rx_buf_size;
    const struct vtbl_async_io *op;
};
#define ASYNC_IO_INIT(_tx_buf, _tx_buf_siz, _rx_buf, _rx_buf_siz, _op)	\
	{                                                               \
	CIRC_BUF_INIT(_tx_buf, 0, 0),                                   \
	(_tx_buf_siz),                                                  \
	CIRC_BUF_INIT(_rx_buf, 0, 0),                                   \
	(_rx_buf_siz),                                                  \
	(_op)                                                           \
	}

struct vtbl_async_io {
    s32	(*setup)        (struct async_io *a);
    s32 (*tx_callback)  (struct async_io *a, const void *src, size_t n);
    s32 (*rx_callback)  (struct async_io *a, void *dst, size_t n);
};

extern const char *default_rx_delims;

s32
async_io_setup(struct async_io *a);

s32
async_io_tx(struct async_io *a, const void *src, size_t n, bool overrun, bool *overrun_out);

#endif
