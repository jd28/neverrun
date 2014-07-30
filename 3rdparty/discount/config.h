#ifndef CONFIG_H
#define CONFIG_H

#if defined(_WIN32) || defined(_WIN64)

#include <Windows.h>

#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define strcasecmp _stricmp
#define strncasecmp _strnicmp

#define INITRNG(x) srand((unsigned int)x)
#define COINTOSS() (rand()&1)

// Set tabstops to N characters: --with-tabstops=4
#define TABSTOP 4

// Allow fenced code blocks: --with-fenced-code
#define WITH_FENCED_CODE 1

// Use id= anchors for table-of-contents links: --with-id-anchor
#define WITH_ID_ANCHOR 1

// Allow `_` and `-` in <> tags: --with-github-tags
#define WITH_GITHUB_TAGS 1

// Use Discount, Extra, or Both types of definition list: --with-dl=both
#define USE_EXTRA_DL 1
#define USE_DISCOUNT_DL 1

#define VERSION "2.1.6"

#endif

#endif // CONFIG_H
