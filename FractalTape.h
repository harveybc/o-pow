/** 
 * @author      Harvey D. Bastidas C. <harveybc@ingeni-us.com>
 * @brief       Fractaltape class
 * @par Description @parblock 
 *      Cinta compuesta por instrucciones que ejecutan los objetos del fractal.
 * @endparblock
 * @copyright @parblock
 *    This file is part of Singularity.
 * 
 *    Singularity is free software; you can redistribute it and/or modify it under
 *    the terms of the GNU General Public License as published by the Free
 *    Software Foundation; either version 3, or (at your option) any later
 *    version.
 * 
 *    Singularity is distributed in the hope that it will be useful, but WITHOUT ANY
 *    WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *    for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with GCC; see the file COPYING3.  If not see
 *    <http://www.gnu.org/licenses/>. 
 * @endparblock
 * @file        FractalMachine.h
 * @version     0.1
 * @date        23 de junio de 2014, 01:12 AM
*/

#ifndef FRACTALTAPE_H
#define	FRACTALTAPE_H
#include <vector>
#include <queue>
#include <string>
class FractalTape {
public:
    // Typedefs y structs
    struct FractalCmd {
        char id;                            ///< La instrucción a ejecutar 
        std::vector <int> parameters_i;     ///< Parámetros enteros
        std::vector <double> parameters_d;  ///< Parámetros double
    };
    int push_instruction(FractalCmd instr);  ///< Adiciona una instrucción al final de la cinta, retorna el tamaño de la cinta
    int pop_instruction(FractalCmd &output);  ///< Adiciona una instrucción al final de la cinta, retorna el tamaño de la cinta
    int get_tape(std::deque <FractalCmd> &output_tape); ///< Obtiene la cinta de instrucciones, retorna el número de instrucciones leídas
    int get_size(); ///< Obtiene el número de instrucciones en la cinta.
    int file_export(std::string file_path); ///< Exporta la cinta a un archivo en el formato JSON
    int file_import(std::string file_path); ///< Importa la cinta desde un archivo JSON   
    FractalTape();
    FractalTape(const FractalTape& orig);
    virtual ~FractalTape();
private:
    std::deque <FractalCmd> instructions; ///< Una cinta fractal es una cola FIFO de caldas de instrucciones que se ejecutan en cada iteración en el estado de la máquina.
};
/*** FractalTape JSON
 * {
 *  "full_tape": [{"id":0,"parameters":[{"value":0}]}]
 * }
 */
#endif	/* FRACTALTAPE_H */

