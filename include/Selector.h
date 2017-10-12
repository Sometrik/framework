#ifndef _SELECTOR_H_
#define _SELECTOR_H_

enum class Selector {
  NORMAL = 0,	// State during normal operation
  ACTIVE,	// Element is active or pressed
  HOVER,	// Mouse cursor is over element
  SELECTED,	// Element has been selected
  DISABLED,	// Element is disabled
  LINK		// Element or a part of element is a link
};

#endif
