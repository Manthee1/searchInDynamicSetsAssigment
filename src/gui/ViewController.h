#include <sstream>

struct ViewData {
	std::string name;
	void (*draw)();
};

namespace ViewController {
	extern ViewData views[];
	extern int viewsSize;
	extern int currentView;
	void run();
	void draw();

}
