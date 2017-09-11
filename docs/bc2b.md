# BashClass to Bash

## How it works?
### Introduction
BashClass syntax is close to other programming languages like C++ and Java, however, instead of compiling to machine code or bytecode, BashClass compiles to Bash.

### Classes
Creating classes in Bash is not possible, but since Bash 4.0, the language added associative arrays which can be used as an alternative way to store a structure of an object. Using this feature, one can keep track of "objects" by assigning them a unique id as part of the key. A key is usually composed of two parts: the object id and the member name of the object. 

### Functions
Functions in BashClass are converted to Bash functions in the generated Bash script. Each statement in a BashClass function is also converted to its corresponding Bash representation while maintaining the original order.

### Type checking
BashClass performs type checking at compile time. An expression can be forced to be of a specific type by casting it. The type of an object can also be checked at runtime (Please refer to the example files in the [example](example) page for further details).

### Reference
An object variable that is initialized in BashClass (using the `new` keyword) stores the unique id of the created object.

### Expressions
Expressions in BashClass are generated step by step following the same operator precedence used in bash.

### Return
BashClass function can be of any type or void. In the generated bash script, the function call passes an additional argument by reference which will hold the "returned" value.

### Arrays
BashClass achieves  multi-dimensional arrays using associative arrays in bash where a key is a combination of the indices concatenated by a delimiter.

## Generated code example
### Code in BashClass
```
class Square {
    var int side;
    constructor Square(var int side=0) {
        this.side = side;
    }

    function int getArea() {
        return side * side;
    }
}

function void print(var int num) {
    ># echo $1
}

function int main() {
    var Square sq = new Square(10);
    print(sq.getArea());
    return 0;
}
```

### Generated BASH script
```
#!/bin/bash
#
# Bash version 4.4+ required!
# This code is auto generated by bashc, do not make manual changes
# -----------------------------------------------------------------
# Initialize array counter
_array_uid_=1

# Initialize class object counter
_object_uid_=1
declare -A _object_

############# BASH FUNCTIONS ##############
# Create array
function _bash_createArray() {
    declare -n _return_=${1}
    declare -g -A "_array_${_array_uid_}"
    _return_=${_array_uid_}
    _array_uid_=$(( ${_array_uid_} + 1 ))
}
# Convert strings into char array
function _bash_StrToCharArray() {
    declare string="${1}"
    declare -n _return_=${2}
    _bash_createArray _tmp_return_
    _return_=${_tmp_return_}
    declare index=0
    declare -n indexValue="_array_${_return_}"
    while (( ${index} < ${#string} )); do
        indexValue[${index}]=${string:${index}:1}
        index=$(( ${index} + 1 ))
    done
}
###########################################

# Create function
function _c_Square_f_Square() {
    declare _args_counter_=1

    # Create argument with a default value
    if (( 0 < $(( ${#} - 0 )) )); then
    declare _c_Square_f_Square_p_side="${1}"
    _args_counter_=$(( ${_args_counter_} + 1 ))
    else
    declare _c_Square_f_Square_p_side=0
    declare -n _result_0=_c_Square_f_Square_p_side
    _result_0=0
    declare _expression_0="${_result_0:-0}"
    _c_Square_f_Square_p_side=${_expression_0:-0}
    fi

    # Configure 'this'
    declare _this_=${!_args_counter_}
    _args_counter_=$(( ${_args_counter_} + 1 ))

    # Configure return statement
    declare -n _return_=${!_args_counter_}

    # Declare member
    _object_[${_this_},"_c_Square_v_side"]=0

    # Header info
    [[ -v _object_[${_this_},"__type__"] ]] || _object_[${_this_},"__type__"]="Square"
    # Evaluate expression
    declare _result_1=${_this_}
    declare -n _result_2=_object_[${_result_1},"_c_Square_v_side"]
    declare -n _result_3=_c_Square_f_Square_p_side
    _result_2=${_result_3:-0}
    declare _expression_1="${_result_2:-0}"

    # Return statement
    _return_=${_this_}
}

# Create function
function _c_Square_f_getArea() {
    declare _args_counter_=1

    # Configure 'this'
    declare _this_=${!_args_counter_}
    _args_counter_=$(( ${_args_counter_} + 1 ))

    # Configure return statement
    declare -n _return_=${!_args_counter_}

    # Return statement
    declare -n _result_4=_object_[${_this_},"_c_Square_v_side"]
    declare -n _result_5=_object_[${_this_},"_c_Square_v_side"]
    declare _expression_2=$(( ${_result_4:-0} * ${_result_5:-0} ))
    _return_=${_expression_2:-0}
    return
}

# Create function
function _f_print() {
    declare _args_counter_=1

    # Create argument
    declare _f_print_p_num="${1}"
    _args_counter_=$(( ${_args_counter_} + 1 ))

    # Run BASH code
     echo $1
}

# Create function
function _f_main() {
    declare _args_counter_=1

    # Configure return statement
    declare -n _return_=${!_args_counter_}

    # Create variable
    declare _f_main_v_sq=0

    # Initialize variable
    declare -n _result_6=_f_main_v_sq
    declare _result_7
    _c_Square_f_Square "10" $(( _object_uid_++ )) _tmp_return_
    _result_7=${_tmp_return_}
    _result_6=${_result_7:-0}
    declare _expression_3="${_result_6:-0}"

    # Evaluate expression
    declare -n _result_8=_f_main_v_sq
    declare _result_9
    _c_Square_f_getArea ${_result_8} _tmp_return_
    _result_9=${_tmp_return_}
    _f_print "${_result_9:-0}"

    # Return statement
    _return_=0
    return
}

# Run main function
_f_main _main_return_
exit ${_main_return_}

# -----------------------------------------------------------------
```