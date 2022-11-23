#include "vm.h"

#include <iostream>
#include <limits>

#include "util.h"


namespace vm {


vm_state create_vm(bool debug) {
    vm_state state;

    // enable vm debugging
    state.debug = debug;


    register_instruction(state, "PRINT", [](vm_state& vmstate, const item_t /*arg*/) {
        std::cout << vmstate.stack.top() << std::endl;
        return true;});

    register_instruction(state, "LOAD_CONST", [](vm_state& state, const item_t number){
        state.stack.push(number);
        return true;});

    register_instruction(state, "EXIT", [](vm_state& state, const item_t){
        if(state.stack.size() < 1){throw vm_stackfail{std::string{"optional message"}};};
        return false;});

    register_instruction(state, "POP", [](vm_state& state, const item_t){
        if(state.stack.size() < 1){throw vm_stackfail{std::string{"optional message"}};};
        state.stack.pop();
        return true;});

    register_instruction(state, "ADD", [](vm_state& state, const item_t){
        if(state.stack.size() < 2){throw vm_stackfail{std::string{"optional message"}};};
        auto first = state.stack.top();
        state.stack.pop();
        auto second = state.stack.top();
        state.stack.pop();
        state.stack.push(first+second);
        return true;});

    register_instruction(state, "DIV", [](vm_state& state, const item_t){
        if(state.stack.size() < 2){throw vm_stackfail{std::string{"optional message"}};};
            auto den = state.stack.top();
            state.stack.pop();
            auto nom = state.stack.top();
            state.stack.pop();
            if (den == 0){
                throw div_by_zero{std::string{"division by zero!"}};} 
            else{
                state.stack.push(nom/den);};
            return true;});

    register_instruction(state, "EQ", [](vm_state& state, const item_t){
        if(state.stack.size() < 2){throw vm_stackfail{std::string{"optional message"}};};
        auto first = state.stack.top();
        state.stack.pop();
        auto second = state.stack.top();
        state.stack.pop();
        if (first == second){
            state.stack.push(1);} 
        else{state.stack.push(0);};
        return true;});

    register_instruction(state, "NEQ", [](vm_state& state, const item_t){
        if(state.stack.size() < 2){throw vm_stackfail{std::string{"optional message"}};};
        auto first = state.stack.top();
        state.stack.pop();
        auto second = state.stack.top();
        state.stack.pop();
        if (first == second){
            state.stack.push(0);
        } else {
            state.stack.push(1);};
            return true;});

    register_instruction(state, "DUP", [](vm_state& state, const item_t){
        if(state.stack.size() < 1){throw vm_stackfail{std::string{"optional message"}};};
        state.stack.push(state.stack.top());
        return true;});

    register_instruction(state, "JMP", [](vm_state& state, const item_t addr){
        if(addr > (state.len-1) || addr < 0){throw vm_segfault{std::string{"whatever"}};};
        state.pc = addr;
        return true;});

    register_instruction(state, "JMPZ", [](vm_state& state, const item_t addr){
        if(state.stack.size() < 1){throw vm_stackfail{std::string{"optional message"}};};
        if(addr > (state.len-1) || addr < 0){throw vm_segfault{std::string{"whatever"}};};
        if(state.stack.top() == 0){
            state.stack.pop();
            state.pc = addr;}
        else{state.stack.pop();};
        return true;});

    register_instruction(state, "WRITE", [](vm_state& state, const item_t){
        if(state.stack.size() < 1){throw vm_stackfail{std::string{"optional message"}};};
        state.output_string.append(std::to_string(state.stack.top()));
        return true;});

    register_instruction(state, "WRITE_CHAR", [](vm_state& state, const item_t){
        if(state.stack.size() < 1){throw vm_stackfail{std::string{"optional message"}};};
        state.output_string.push_back(char(state.stack.top()));
        return true;});

    return state;
}


void register_instruction(vm_state& state, std::string_view name,
                          const op_action_t& action) {
    size_t op_id = state.next_op_id;
    state.instruction_ids.emplace(name, op_id); //maps instruction name to operation id
    state.instruction_names.emplace(op_id, name); //maps operation ids back to instruction names (for debugging)
    state.instruction_actions.emplace(op_id, action); //instruction_actions map op_id to action
    state.next_op_id += 1;
    // TODO make instruction available to vm
}


code_t assemble(const vm_state& state, std::string_view input_program) {
    code_t code;

    // convert each line separately
    for (auto& line : util::split(input_program, '\n')) {

        auto line_words = util::split(line, ' ');

        // only support instruction and one argument
        if (line_words.size() >= 3) {
            throw invalid_instruction{std::string{"more than one instruction argument: "} + line};
        }

        // look up instruction id
        auto& op_name = line_words[0];
        auto find_op_id = state.instruction_ids.find(op_name);
        if (find_op_id == std::end(state.instruction_ids)) {
            throw invalid_instruction{std::string{"unknown instruction: "} + op_name};
        }
        op_id_t op_id = find_op_id->second;

        // parse the argument
        item_t argument{0};
        if (line_words.size() == 2) {
            argument = std::stoll(line_words[1]);
        }

        // and save the instruction to the code store
        code.emplace_back(op_id, argument);
    }
    return code;
}


std::tuple<item_t, std::string> run(vm_state& vm, const code_t& code) {
    // to help you debugging the code!
    if (vm.debug) {
        std::cout << "=== running vm ======================" << std::endl;
        std::cout << "disassembly of run code:" << std::endl;
        for (const auto &[op_id, arg] : code) {
            if (not vm.instruction_names.contains(op_id)) {
                std::cout << "could not disassemble - op_id unknown..." << std::endl;
                std::cout << "turning off debug mode." << std::endl;
                vm.debug = false;
                break;
            }
            std::cout << vm.instruction_names[op_id] << " " << arg << std::endl;
        }
        std::cout << "=== end of disassembly" << std::endl << std::endl;
    }
    vm.len = code.size();
    // execution loop for the machine
    while (true) {

        auto& [op_id, arg] = code[vm.pc];

        if (vm.debug) {
            std::cout << "-- exec " << vm.instruction_names[op_id] << " arg=" << arg << " at pc=" << vm.pc << std::endl;
        }

        // increase the program counter here so its value can be overwritten
        // by the instruction when it executes!
        vm.pc += 1;

        // TODO execute instruction and stop if the action returns false.
        auto num = vm.instruction_actions.find(op_id);
        op_action_t action = num->second;
        if(action(vm, arg) == false){
            break;}
    }

    return {vm.stack.top(), vm.output_string}; // TODO: return tuple(exit value, output text)
}
} // namespace vm
