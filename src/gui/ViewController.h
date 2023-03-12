#include <sstream>

struct ViewData {
	std::string name;
	void (*draw)();
};

enum ViewType {
	AVL_TREE_VIEW,
	RED_BLACK_TREE_VIEW,
	HASH_TABLE_OPEN_ADDRESSING_VIEW,
	HASH_TABLE_CHAINING_VIEW,
};

namespace ViewController {
	extern ViewData views[];
	extern int viewsSize;
	extern int currentView;
	void run();
	void run(ViewType viewType);
	void draw();
}
