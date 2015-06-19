/* 
 * File:   FractalMachine.cpp
 * Author: harveybc
 *
 * Executes the following instruction from a program from an instance, from a 
 * instances deque. If the program ends execution, itÂ´s instance is removed 
 * from the deque.
 * 
 * Created on 22 de junio de 2014, 08:37 PM
 */

#include "FractalMachine.h"
#include "DataSet.h"


void FractalMachine::add_instruction(FractalInstruction instr){
    instances.front().add_instruction(instr);
}

void FractalMachine::run_instruction(FractalInstruction instr){
    instances.front().add_instruction(instr);
    iterate();
}

void FractalMachine::run_program(){
    while (!instances.empty())
        iterate();
}

 std::vector<int> FractalMachine::get_conn_list(int node_target_id){
     return(conn_list[node_target_id]);
}

Connection FractalMachine::get_connection(int conn_id){
    return(connections[conn_id]);
}

Node FractalMachine::get_node(int node_id){
    return(nodes[node_id]);
}

Instance FractalMachine::get_instance(int instance_id){
    return(instances[instance_id]);
}
 
void FractalMachine::reset() { ///< Erases all nodes, conex and instances, create instance 0
    Instance tmp_instance(0,0);
    int i;
    // clears all nodes, conex and instances
    nodes.clear();
    connections.clear();
    instances.clear();
    // adds a new instance
    instances.push_back(tmp_instance);
    // sets program counter to 0
    instances[0].set_program_counter(0);
    // reset conn list
    for (i=0;i<conn_list.size();i++){
        conn_list[i].clear();
    }
    conn_list.clear();
}

void FractalMachine::iterate() { ///< Executes next instruction from the instance's queue
    Node tmp_node;
    FractalInstruction tmp_instruction;
    Connection tmp_connection;
    std::deque <FractalInstruction> tmp_instructions;
    Instance tmp_instance(0,0);
    std::vector <int> tmp_conn_index;
    tmp_conn_index.push_back(0); // adds an element to tmp_conn_index
    if (instances.size() > 0) {
        // Executes every command in the tape,later it is removed from instances         
        if (instances.front().fetch(tmp_instruction)) { 
            switch (tmp_instruction->id) {
                case 0: /// Wait(milliseconds)
                    std::this_thread::sleep_for(std::chrono::milliseconds(tmp_instruction->parameters_i[0]));
                break;
                case 1: /// CreateNode(int source_id, int recursive, int interfaces, bool evaluated, bool active)
                    tmp_node.set_node(
                        nodes.size(),                        // id
                        tmp_instruction->parameters_i[0],    // source_id  
                        tmp_instruction->parameters_i[1],    // recursive  
                        tmp_instruction->parameters_i[2],     // interfaces
                        tmp_instruction->parameters_b[0],    // evaluated
                        tmp_instruction->parameters_b[1]     // active
                    );
                    // adds the node
                    nodes.push_back(tmp_node);
                    // if node is recursive, a new instance is added
                    if (tmp_instruction->parameters_i[1] > 0){ // 0 evaluations
                        tmp_instance.set_instance(instances.size(), tmp_instruction->parameters_i[0]);
                        instances.push_back(tmp_instance);
                    }
                break;
                case 2: /// NodeSetActive(int node_id, bool act)
                    nodes[tmp_instruction->parameters_i[0]].set_active(tmp_instruction->parameters_b[0]);
                break;
                case 3: /// TODO:: REMOVER NodeSetEvaluated(int node_id, bool evaluated)
                    nodes[tmp_instruction->parameters_i[0]].set_evaluated(tmp_instruction->parameters_b[0]);
                break;
                case 4: /// NodeAddInterface(int node_id, int num, double init_val)
                    nodes[tmp_instruction->parameters_i[0]].add_interface(tmp_instruction->parameters_i[1],
                            tmp_instruction->parameters_d[0]);
                break;
                case 5: /// NodeSetRecursive(int node_id, int recursive)
                    // if node is not-evaluated, a new instance is added
                    if (tmp_instruction->parameters_i[1] > 0){ // >0 evaluations
                        // if the previous recursive was 0, adds an instance
                        if (nodes[tmp_instruction->parameters_i[0]].get_recursive()==0){
                            tmp_instance.set_instance(instances.size(), tmp_instruction->parameters_i[0]);
                            instances.push_back(tmp_instance);
                        }
                    }
                    nodes[tmp_instruction->parameters_i[0]].set_recursive(tmp_instruction->parameters_i[1]);
                    
                break;
                case 6: /// CreateConnection(node_id_source, node_id_target, src_if, length,active)
                    tmp_connection.set_conn(
                        connections.size(),                 // connid
                        tmp_instruction->parameters_i[0],   // source_id (remote)
                        tmp_instruction->parameters_i[1],   // target_id (local)
                        tmp_instruction->parameters_i[2],   // source interface
                        tmp_instruction->parameters_d[0],   // weight    
                        tmp_instruction->parameters_d[1],   // length
                        tmp_instruction->parameters_d[2],   // speed
                        tmp_instruction->parameters_b[0]    // active
                    );
                    // adds the conn_index for easy ANN evaluation
                    conn_list[tmp_instruction->parameters_i[1]].push_back(connections.size());
                    // calculates the segment
                    tmp_connection.calculate_segment();
                    // verify the source interface size and increases it if needed
                    if (nodes[tmp_instruction->parameters_i[0]].interfaces[0].get_buffer_size()
                            <= tmp_connection.get_segment()){
                        nodes[tmp_instruction->parameters_i[0]].interfaces[0].resize_buffer(tmp_connection.get_segment()+1);
                    }
                    // adds connection to the connections deque
                    connections.push_back(tmp_connection);
                    
                break;
                case 7: /// ConnectionSetWeight(int conn_id, double weight)
                    connections[tmp_instruction->parameters_i[0]].set_weight(tmp_instruction->parameters_d[0]);
                break;
                case 8: /// ConnectionSetLength(int conn_id, double length)
                    connections[tmp_instruction->parameters_i[0]].set_length(tmp_instruction->parameters_d[1]);
                    // verify the source interface size and increases it if needed
                    if (nodes[tmp_instruction->parameters_i[0]].interfaces[0].get_buffer_size()
                            <= tmp_connection.get_segment()){
                        nodes[tmp_instruction->parameters_i[0]].interfaces[0].resize_buffer(tmp_connection.get_segment()+1);
                    }
               break;
                case 9: /// ConnectionSetSpeed(int conn_id, double speed)
                    connections[tmp_instruction->parameters_i[0]].set_speed(tmp_instruction->parameters_d[2]);
                    // verify the source interface size and increases it if needed
                    if (nodes[tmp_instruction->parameters_i[0]].interfaces[0].get_buffer_size()
                            <= tmp_connection.get_segment()){
                        nodes[tmp_instruction->parameters_i[0]].interfaces[0].resize_buffer(tmp_connection.get_segment()+1);
                    }
                break;
                case 10: /// ConnectionSetActive(int conn_id, bool active)
                    connections[tmp_instruction->parameters_i[0]].set_active(tmp_instruction->parameters_b[0]);
                break;                
                default:
                break;
            }
        }
        else{
            // if the front instance's base node has recursive > 0, 
            if (nodes[instances.front().get_base_node_id()].get_recursive()>0)
            // decreases recursive
            nodes[instances.front().get_base_node_id()].set_recursive(nodes[instances.front().get_base_node_id()].get_recursive()-1);    
            // sets front instance's program counter to 0
            instances.front().set_program_counter(0);
            // push the instance in the back of the deque
            instances.push_back(instances.front());
            // pop instance from front
            instances.pop_front();
            if (instances.size() > 0) {
                instances.front().set_program_counter(0);
            }
        }
    }
}

int FractalMachine::num_nodes(){    ///< returns the number of nodes
    return(nodes.size());
}

double FractalMachine::read_message(int node_id, int interface_id, int segment){   ///< reads a message from a interface
    return(nodes[node_id].interfaces[interface_id].read_msg(segment));
}
    
void FractalMachine::push_message(int node_id, int interface_id, double msg){    ///< puts a message in a interface and deletes the oldest one
    nodes[node_id].interfaces[interface_id].push_msg(msg);
}

void FractalMachine::set_node_evaluated(int node_id, bool eval){
    nodes[node_id].evaluated = eval;
}

void FractalMachine::reset_nodes(int num_inputs){
    int acum1 = num_inputs+1;
    int n_nodes = num_nodes();
    int i;
    for (i = acum1; i < n_nodes; i++){
        nodes[i].evaluated=false;
    }
    // sets the input and bias (num_inputs) neurons as evaluated
    for (i = 0; i <= num_inputs; i++){
        nodes[i].evaluated=true;
    }
}

bool FractalMachine::get_node_eval(int node_id){
    return (nodes[node_id].evaluated);
}

FractalMachine::FractalMachine() {
    /// reset
    reset();
}

FractalMachine::FractalMachine(const FractalMachine& orig) {
}

FractalMachine::~FractalMachine() {
}


//template class FractalMachine<Expert<double >,double >;