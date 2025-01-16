// Local includes
#include "assembler.hpp"

int main() {
    std::vector<instruction_t> program{
        { inst, op_random },
        { inst, op_random },
        { inst, op_input },
        { inst, op_write_to_screen, 0, 123, 0b01101010 },
    };

    return assemble(program);
}
