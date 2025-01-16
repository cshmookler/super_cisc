// Local includes
#include "../build/version.hpp"

namespace arch {

const char* get_runtime_version() {
    return ::arch::compiletime_version;
}

} // namespace arch