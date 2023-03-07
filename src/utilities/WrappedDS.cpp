#include "wrappers/AVLTreeWrapper.h"
#include "wrappers/RedBlackTreeWrapper.h"
#include "wrappers/CHashTableWrapper.h"
#include "wrappers/OAHashTableWrapper.h"

#include "WrappedDS.h"

// Array of pointers to the wrapper class
DSStandardWrapper *WrappedDS::DSentries[] = {
	new AVLTreeWrapper(),
	new RedBlackTreeWrapper(),
	new CHashTableWrapper(),
	new OAHashTableWrapper()};