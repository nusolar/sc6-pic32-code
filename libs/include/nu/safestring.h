/**
 * @file SafeString.h
 * @brief Provides strlcpy and strlcat, safer alternativesto strncpy and
 *          strncat, respectively.
 */

#ifndef __NU_SAFESTRING_H
#define __NU_SAFESTRING_H

#include <sys/types.h>
#include <string.h>

/**
 * @brief   Copies a string from src to dst, guaranteeing null-termination.
 *
 * @param[in,out]   dst A pointer to the destination string.
 * @param[in]       src A pointer to the source string. String must be
 *                      null-terminated.
 * @param[in]       siz The full size of the destination buffer.
 * @return              The length of src.
 */
size_t
strlcpy(char *dst, const char *src, size_t siz);

/**
 * @brief   Concatenates the string dst with the string src, guaranteeing
 *          null-termination.
 *
 * @param[in,out]   dst A pointer to the destination string.
 * @param[in]       src A pointer to the source string. String must be
 *                      null-terminated.
 * @param[in]       siz The full size of the destination buffer.
 * @return              The initial length of dst plus the length of src.
 */
size_t
strlcat(char *dst, const char *src, size_t siz);

#endif
